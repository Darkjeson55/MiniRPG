#include <windows.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_keycode.h>
#include "src/GyoUtils/gyoutils.h"
#include "src/platform.h"
#include "src/input.cpp"
#include "src/render.cpp"

#undef main

inline FILETIME Win32_get_last_write_time(char* file_name){
    FILETIME last_write_time = {};
    WIN32_FIND_DATA find_data;
    HANDLE find_handle = FindFirstFileA(file_name, &find_data);
    if(find_handle != INVALID_HANDLE_VALUE){
        last_write_time = find_data.ftLastWriteTime;
        FindClose(find_handle);
    }
    return (last_write_time);
}

game_code load_game_code(char* dll_name, char* temp_dll_name){
    game_code resoult = {};
    resoult.dll_last_write_time = Win32_get_last_write_time(dll_name);
    while(1){
        if(CopyFile(dll_name, temp_dll_name, FALSE)) break;
        if(GetLastError() == ERROR_FILE_NOT_FOUND) break;
    }

    resoult.game_code_dll = LoadLibraryA(temp_dll_name);
    if(resoult.game_code_dll){
        resoult.update = (GameUpdate*)
            GetProcAddress(resoult.game_code_dll, "game_update_and_render");

        resoult.is_valid = resoult.update;
    }

    if(!resoult.is_valid){
        resoult.update = game_update_stub;
    }

    return resoult;
}

void unload_game_code(game_code* code){
    if(code->game_code_dll){
        FreeLibrary(code->game_code_dll);
        code->game_code_dll = 0;
    }

    code->is_valid = false;
    code->update = game_update_stub;
}

//WARNING(Jason): this is not good because the memory is allocated randomly in the memory and never freed but for a simple demo is perfect
LOAD_TEXTURE(load_texture_from_file){
    SDL_Surface* surface = IMG_Load(path);
    Texture tex;
    ASSERT(surface != NULL, "image not found to path: %", path);
    const int array_size = surface->w * surface->h * 4;

    u8* pixel_data = (u8*)malloc(array_size);
    for(int x = 0; x < array_size; x+=4){
        u8* pix = (u8*)surface->pixels;
        pixel_data[x] = pix[x];
        pixel_data[x + 1] = pix[x + 1];
        pixel_data[x + 2] = pix[x + 2];
        pixel_data[x + 3] = pix[x + 3];
    }

    tex.data = pixel_data;
    tex.w = surface->w;
    tex.h = surface->h;
    SDL_FreeSurface(surface);
    return tex;
}

int main(){
    //Window releted stuff
    SDL_Window *window = nullptr;
    SDL_Event event;
    bool window_initialized = true;

    //render releted stuff
    SDL_Renderer* render = nullptr;
    SDL_Texture* frame_buffer = nullptr;
    u32* pixel_buffer;
    int pitch;

    if(SDL_Init(SDL_INIT_VIDEO != 0)){
        printf("SDL FAILED TO INITIALIZE");
        return 1;
    }

    window = SDL_CreateWindow("MiniRPG",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED, 
                WINDOW_W, 
                WINDOW_H, 
                0);


    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    frame_buffer = SDL_CreateTexture(render, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, RENDER_W, RENDER_H);

    for(int i = 0; i < 255; i++){
        key_list[i] = KeyState::KeyRelease;
    }

    for(int i = 0; i < 32; i++){
        mouse_button_list[i] = KeyState::KeyRelease;
    }

    char game_dll_source[] = "game.dll";
    char full_path_game_dell_source[MAX_PATH];
    GetFullPathName(game_dll_source, MAX_PATH, full_path_game_dell_source, nullptr);

    char temp_dll_source[] = "game_temp.dll";
    char full_path_temp_dell_source[MAX_PATH];
    GetFullPathName(temp_dll_source, MAX_PATH, full_path_temp_dell_source, nullptr);

    game_code game = load_game_code(full_path_game_dell_source, full_path_temp_dell_source);

    u32 old_time = 0;
    Bump static_game_memory = make_bump_allocator(Megabytes(256));

    Input g_input;
    Platform g_platform;

    g_input.mouse_x = mouse_x;
    g_input.mouse_y = mouse_y;

    g_input.is_key_up = is_key_up;
    g_input.is_key_down = is_key_down;
    g_input.is_key_press = is_key_press;

    g_input.is_mouse_button_up = is_mouse_button_up;
    g_input.is_mouse_button_down = is_mouse_button_down;
    g_input.is_mouse_button_press = is_mouse_button_press;

    g_platform.load_texture = load_texture_from_file;

    while(window_initialized){
        FILETIME new_dll_write_time = Win32_get_last_write_time(full_path_game_dell_source);
        if(CompareFileTime(&new_dll_write_time, &game.dll_last_write_time)){
            game.dll_last_write_time = new_dll_write_time;
            unload_game_code(&game);
            game = load_game_code(full_path_game_dell_source, full_path_temp_dell_source);
        }

        pool_events(&event, window_initialized);

        SDL_LockTexture(frame_buffer, NULL, (void**)&pixel_buffer, &pitch);
    
        u32 current_time = SDL_GetTicks();
        double frame_time = (current_time - old_time) / 1000.0;
        if(frame_time >= 0.016){
            memset(pixel_buffer, 0, RENDER_W * RENDER_H);
            game.update(pixel_buffer, &static_game_memory, &g_input, &g_platform, frame_time);
            old_time = current_time;
        }

        reset_input();
        SDL_UnlockTexture(frame_buffer);
        SDL_RenderCopy(render, frame_buffer, NULL, NULL);
        SDL_RenderPresent(render);
    }

    SDL_Quit();

    return 0;
}

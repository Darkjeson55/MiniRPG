#include <windows.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_keycode.h>
#include "src/input.cpp"
#include "src/GyoUtils/gyoutils.h"
#include "src/platform.h"
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

    for(int i = 0; i < 600; i++){
        key_list[i] = KeyState::KeyRelease;
    }

    char game_dll_source[] = "game.dll";
    char full_path_game_dell_source[MAX_PATH];
    GetFullPathName(game_dll_source, MAX_PATH, full_path_game_dell_source, nullptr);

    char temp_dll_source[] = "game_temp.dll";
    char full_path_temp_dell_source[MAX_PATH];
    GetFullPathName(temp_dll_source, MAX_PATH, full_path_temp_dell_source, nullptr);

    game_code game = load_game_code(full_path_game_dell_source, full_path_temp_dell_source);


    while(window_initialized){
        FILETIME new_dll_write_time = Win32_get_last_write_time(full_path_game_dell_source);
        if(CompareFileTime(&new_dll_write_time, &game.dll_last_write_time)){
            game.dll_last_write_time = new_dll_write_time;
            unload_game_code(&game);
            game = load_game_code(full_path_game_dell_source, full_path_temp_dell_source);
        }

        pool_events(&event, window_initialized);

        SDL_LockTexture(frame_buffer, NULL, (void**)&pixel_buffer, &pitch);
        memset(pixel_buffer, 0, RENDER_W * RENDER_H);
    
        game.update(pixel_buffer, 0);

        reset_input();
        SDL_UnlockTexture(frame_buffer);
        SDL_RenderCopy(render, frame_buffer, NULL, NULL);
        SDL_RenderPresent(render);
    }

    SDL_Quit();

    return 0;
}

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include "src/math.h"
#include <vector>

#undef main

#define WINDOW_W 800
#define WINDOW_H 600

#define RENDER_W 10
#define RENDER_H 10

enum class KeyState{KeyDown, KeyUp, KeyRelease, KeyHold};
KeyState key_list[255];
KeyState mouse_button_list[32];
vec2i mouse_pos;

//******** INPUT *******
void pool_events(SDL_Event* ev, bool& app_running){
    while(SDL_PollEvent(ev) != 0){
        if(ev->type == SDL_QUIT){
            app_running = false;
        }

        switch(ev->type){
            case SDL_KEYDOWN:{
                uint32_t key_id = ev->key.keysym.sym;
                if(key_id < 255){
                    key_list[key_id] =  (key_list[key_id] == KeyState::KeyHold) ? KeyState::KeyHold : KeyState::KeyDown;
                }
                break;

            }
            case SDL_KEYUP:{
                uint32_t key_id = ev->key.keysym.sym;
                if(key_id < 255){
                    key_list[key_id] = KeyState::KeyUp;
                }
                break;
            }
            
            case SDL_MOUSEBUTTONDOWN: {
                uint32_t key_id = ev->button.button;
                if(key_id < 32){
                    mouse_button_list[key_id] =  (mouse_button_list[key_id] == KeyState::KeyHold) ? KeyState::KeyHold : KeyState::KeyDown;
                }
                

                }break;
            case SDL_MOUSEBUTTONUP: {
                uint32_t key_id = ev->button.button;

                if(key_id < 32){
                    mouse_button_list[key_id] = KeyState::KeyUp;
                }
                }break;
        }
    }
    
    SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
}

bool is_key_down(uint32_t key_code){
    return key_list[key_code] == KeyState::KeyDown;
}

bool is_key_up(uint32_t key_code){
    return key_list[key_code] == KeyState::KeyUp;
}

bool is_key_press(uint32_t key_code){
    return (key_list[key_code] == KeyState::KeyDown || key_list[key_code] == KeyState::KeyHold) ? true : false;
}

bool is_mouse_button_down(uint32_t key_code){
    return mouse_button_list[key_code] == KeyState::KeyDown;
}

bool is_mouse_button_up(uint32_t key_code){
    return mouse_button_list[key_code] == KeyState::KeyUp;
}

bool is_mouse_button_press(uint32_t key_code){
    return (mouse_button_list[key_code] == KeyState::KeyDown || mouse_button_list[key_code] == KeyState::KeyHold) ? true : false;
}

void reset_input(){
    for(int i = 0; i < 255; i++){
        if(key_list[i] == KeyState::KeyDown)
            key_list[i] = KeyState::KeyHold;
        if(key_list[i] == KeyState::KeyUp)
            key_list[i] = KeyState::KeyRelease;
    }

    for(int i = 0; i < 32; i++){
        if(mouse_button_list[i] == KeyState::KeyDown)
            mouse_button_list[i] = KeyState::KeyHold;
        if(mouse_button_list[i] == KeyState::KeyUp)
            mouse_button_list[i] = KeyState::KeyRelease;
    }
}


//************** RENDER ******************* 
void set_pixel(uint32_t* buffer ,vec2 pos, uint32_t color){
   buffer[int(pos.x + pos.y * RENDER_W)] = color; 
}


int main(){

    //Window releted stuff
    SDL_Window *window = nullptr;
    SDL_Event event;
    bool window_initialized = true;

    //render releted stuff
    SDL_Renderer* render = nullptr;
    SDL_Texture* frame_buffer = nullptr;
    uint32_t* pixel_buffer;
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


    while(window_initialized){
        pool_events(&event, window_initialized);

        SDL_LockTexture(frame_buffer, NULL, (void**)&pixel_buffer, &pitch);


        set_pixel(pixel_buffer, vec2{0,0}, 0xff0000);



        //for(int x = 0; x < RENDER_W; x++){
        //    for(int y = 0; y < RENDER_H; y++){
        //        pixel_buffer[x+y*RENDER_W] = (int)((x / (float)RENDER_W) * 255) << 16 |  (int)((y / (float)RENDER_H) * 255) << 8;
        //    }
        //}


        reset_input();
        SDL_UnlockTexture(frame_buffer);
        SDL_RenderCopy(render, frame_buffer, NULL, NULL);
        SDL_RenderPresent(render);
    }

    SDL_Quit();

    return 0;
}

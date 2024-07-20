#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include "src/math.h"
#include <vector>

#undef main

#define RENDER_W 480
#define RENDER_H 320

#define WINDOW_SCALE 3
#define WINDOW_W RENDER_W * WINDOW_SCALE
#define WINDOW_H RENDER_H * WINDOW_SCALE


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
void draw_pixel(uint32_t* buffer ,vec2 pos, uint32_t color){
    if(pos.x >= 0 && pos.x < RENDER_W && pos.y >= 0 && pos.y < RENDER_H)
        buffer[int(pos.x + pos.y * RENDER_W)] = color; 
}

void draw_line(uint32_t* buffer ,vec2 pos1, vec2 pos2, uint32_t color){
    bool steep = false;
    if(abs(pos1.x - pos2.x) < abs(pos1.y - pos2.y)){
        std::swap(pos1.x, pos1.y);
        std::swap(pos2.x, pos2.y);
        steep = true;
    }

    if(pos1.x > pos2.x){
        std::swap(pos1.x, pos2.x);
        std::swap(pos1.y, pos2.y);
    }


    int dx = pos2.x - pos1.x;
    int dy = pos2.y - pos1.y;

    int derror2 = abs(dy) * 2;
    int error2 = 0;

    int y = pos1.y;

    for(int x = pos1.x; x <= pos2.x; x++){
        if(steep){
            draw_pixel(buffer, vec2{(float)y,(float)x}, color);
        }else{
            draw_pixel(buffer, vec2{(float)x,(float)y}, color);
        }
        error2 += derror2;
        if(error2 > dx){
            y += pos2.y > pos1.y ? 1 : -1;
            error2 -= dx*2;
        }
    }
}

float a = 0;



#define MAP_SIZE_X 5
#define MAP_SIZE_Y 5

int map[25] = {
    1,1,1,1,1,
    1,0,0,0,1,
    1,1,0,1,1,
    1,0,0,0,1,
    1,1,1,1,1
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

        a += 0.001f;
        pool_events(&event, window_initialized);

        SDL_LockTexture(frame_buffer, NULL, (void**)&pixel_buffer, &pitch);
        memset(pixel_buffer, 0, RENDER_W * RENDER_H);
    

        //set_pixel(pixel_buffer, vec2{0,0}, 0xff0000);
        //set_pixel(pixel_buffer, vec2{1,1}, 0xff0000);

        //draw_line(pixel_buffer, vec2{10,10}, vec2{50 + (float)sin(a) * 15,20 + (float)cos(a) * 15}, 0xffffff);
        //draw_line(pixel_buffer, vec2{20,13}, vec2{40,80}, 0xffff00);
        //draw_line(pixel_buffer, vec2{80,40}, vec2{13,20}, 0xff0000);



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

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <vector>

#include "src/math.h"
#include "src/game.h"

#include "src/input.cpp"
#include "src/render.cpp"
#include "src/game.cpp"

#undef main


#define WINDOW_SCALE 3
#define WINDOW_W RENDER_W * WINDOW_SCALE
#define WINDOW_H RENDER_H * WINDOW_SCALE


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


    while(window_initialized){

        pool_events(&event, window_initialized);

        SDL_LockTexture(frame_buffer, NULL, (void**)&pixel_buffer, &pitch);
        memset(pixel_buffer, 0, RENDER_W * RENDER_H);
    
        update_and_render(pixel_buffer);

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

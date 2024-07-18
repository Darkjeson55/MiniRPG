#include <stdio.h>
#include <SDL2/SDL.h>
#include "src/math.h"

#undef main

#define WINDOW_W 800
#define WINDOW_H 600

#define RENDER_W 10
#define RENDER_H 10


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

    while(window_initialized){
        if(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                window_initialized = false;
            }
        }

        SDL_LockTexture(frame_buffer, NULL, (void**)&pixel_buffer, &pitch);


        set_pixel(pixel_buffer, vec2{0,0}, 0xff0000);


        //for(int x = 0; x < RENDER_W; x++){
        //    for(int y = 0; y < RENDER_H; y++){
        //        pixel_buffer[x+y*RENDER_W] = (int)((x / (float)RENDER_W) * 255) << 16 |  (int)((y / (float)RENDER_H) * 255) << 8;
        //    }
        //}


        SDL_UnlockTexture(frame_buffer);
        SDL_RenderCopy(render, frame_buffer, NULL, NULL);

        SDL_RenderPresent(render);
    }

    SDL_Quit();

    return 0;
}

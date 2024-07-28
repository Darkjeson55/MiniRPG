#include <cmath>
#include "src/GyoUtils/gyoutils.h"
#include "src/platform.h"
#include "src/game.h"
//#include "src/render.h"
#include "src/render.cpp"

void init_game(){
    memory->base_texture = platfomr->load_texture("Assets/Atlas1.png");
}

//NOTE(Jason): this gets only the rgb value and ignores the alpha
u32 get_texure_pixel(Texture* tex, u32 x, u32 y){
    u8 r = tex->data[(x+y*tex->w) * 4];
    u8 g = tex->data[(x+y*tex->w) * 4+ 1];
    u8 b = tex->data[(x+y*tex->w) * 4+ 2];
    u32 color = r << 16 | g << 8 | b;
    return color;
}

float a = 0;
float t = 0;

extern "C" GAME_UPDATE(game_update_and_render){
    memory = (GameMemory*)game_memory->data;
    input = g_input;
    platfomr = g_platform;

    if(memory->is_initialized == false){
        print("Game Initialized");
        init_game();
        memory->is_initialized = true;
    }
    
    a += 0.05f;
    t += 0.05f;
    float half_fov_angle = 90 / 2.0f;
    float camera_t_angle = 180 - (90 + half_fov_angle);
    float pixel_plane_distance = (RENDER_W / 2) * tan(camera_t_angle * DEG2RAD);

    for(int y = 0; y < RENDER_H; y++){
        float y_pos = (y  - (RENDER_H / 2) );
        float z_lenght = (((6 - y_pos)) * pixel_plane_distance + sin(t) * 300) / y_pos + pixel_plane_distance;
        float angle = atan(z_lenght / 6);
        float z_depth = z_lenght / sin(angle);
        if(y_pos < 0)
            z_lenght = ((-6 - y_pos) * pixel_plane_distance + sin(t) * 300) / y_pos + pixel_plane_distance;
        for(int x = 0; x < RENDER_W; x++){
            float x_pos = (x - RENDER_W / 2);
            int x_pix = z_lenght + a;
            int y_pix = (x_pos * z_lenght) / pixel_plane_distance + a;

            int pix_pos_x = x_pix & 15;
            int pix_pos_y = y_pix & 15;

            u32 final_color = get_texure_pixel(&memory->base_texture, pix_pos_x, pix_pos_y);

            memory->depth_buffer[x + y * RENDER_W] = z_lenght;
            draw_pixel(pixels, vec2{x, y}, final_color);
        }
    }

    //APPLY FOG
    for(int y = 0; y < RENDER_H; y++){
        for(int x = 0; x < RENDER_W; x++){
            float depth = memory->depth_buffer[x + y * RENDER_W];
            u32 color = pixels[x + y * RENDER_W];
            u32 r = ((color >> 16) & 0xff);
            u32 g = ((color >> 8) & 0xff);
            u32 b = (color & 0xff);


            float brightness = 255 - depth * 2;

            if(brightness > 255)
                brightness = 255;

            if(brightness <=0)
                brightness = 0;

            r = (int)(r / 255.0 * brightness);
            g = (int)(g / 255.0 * brightness);
            b = (int)(b / 255.0 * brightness);

            u32 final_color = r << 16 | g << 8 | b;

            draw_pixel(pixels, vec2{x, y}, final_color);
        }
    }
}

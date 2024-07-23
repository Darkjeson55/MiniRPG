#include <cmath>
#include "src/GyoUtils/gyoutils.h"
#include "src/platform.h"
//#include "src/render.h"
#include "src/render.cpp"


float a = 0;

extern "C" GAME_UPDATE(game_update_and_render){

    a += 0.002f;

    float half_fov_angle = 90 / 2.0f;
    float camera_t_angle = 180 - (90 + half_fov_angle);
    float pixel_plane_distance = (RENDER_W / 2) * tan(camera_t_angle * DEG2RAD);

    for(int y = 0; y < RENDER_H; y++){
        float y_pos = (y  - (RENDER_H / 2) );
        float z_lenght = (((6 - y_pos)) * pixel_plane_distance - sin(a) * 150) / y_pos + pixel_plane_distance;
        if(y_pos < 0)
            z_lenght = ((-6 - y_pos) * pixel_plane_distance - sin(a) * 150) / y_pos + pixel_plane_distance;

        for(int x = 0; x < RENDER_W; x++){
            float x_pos = (x - RENDER_W / 2);
            int x_pix = z_lenght;
            int y_pix = (x_pos * z_lenght) / pixel_plane_distance;
            u32 final_color = ((y_pix) & 31) * 16 << 16 | ((x_pix) & 31) * 16 << 8;
            draw_pixel(pixels, vec2{x, y}, final_color);
        }
    }
}

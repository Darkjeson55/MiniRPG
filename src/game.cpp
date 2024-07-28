


void raycast_walls(u32* buffer){

}

void raycast_floor(u32* buffer){

}

void draw_map(u32* buffer){
    for(int x = 0; x < MAP_W; x++){
        for(int y = 0; y < MAP_H; y++){
            u32 map = game_data.map[x+y*MAP_W];
            u32 quad_size = 10;
            if(map > 0){
                draw_quad(buffer, vec2{(float)x * (float)quad_size,y * (float)quad_size}, vec2{(int)quad_size, (int)quad_size}, 0xffffff);
            }
        }
    }
}

float a = 0;

void update_and_render(u32* buffer){
    if(!game_data.is_initialized){
        game_data.fov = 90;

        game_data.is_initialized = true;
    }

    float half_fov_angle = game_data.fov / 2.0f;
    float camera_t_angle = 180 - (90 + half_fov_angle);
    float pixel_plane_distance = (RENDER_W / 2) * tan(camera_t_angle * DEG2RAD);

    
    if(is_key_press(SDLK_w)){
        a+=0.002f;

    }

    if(is_key_press(SDLK_s)){
        a-=0.002f;
    }

    a+=0.002f;
    for(int y = 0; y < RENDER_H; y++){
        float y_pos = (y  - (RENDER_H / 2) );
        float z_lenght = (((6 - y_pos)) * pixel_plane_distance - sin(a) * 150) / y_pos + pixel_plane_distance;
        if(y_pos < 0)
            z_lenght = ((6 - y_pos) * pixel_plane_distance + sin(a) * 150) / y_pos + pixel_plane_distance;

        for(int x = 0; x < RENDER_W; x++){
            float x_pos = (x - RENDER_W / 2);
            int x_pix = z_lenght;
            int y_pix = (x_pos * z_lenght) / pixel_plane_distance;
            u32 final_color = (y_pix & 31) * 16 << 16 | (x_pix & 31) * 16 << 8;
            draw_pixel(buffer, vec2{x, y}, final_color);
        }
    }
}



GameData game_data = {};

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
                draw_quad(buffer, vec2{(float)x * (float)quad_size,y * (float)quad_size}, vec2i{(int)quad_size, (int)quad_size}, 0xffffff);
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
 a += 0.002;
    //DRAW MAP
    //draw_map(buffer);

    //DRAW THE PLAYER

    //DRAW FLOOR

    for(int y = 0; y < RENDER_H; y++){
        float y_pos = y - RENDER_H / 2;
        if(y_pos < 0)
            y_pos *= -1;

        float z_lenght = (((4 - y_pos) + sin(a) * 10) * pixel_plane_distance) / y_pos + pixel_plane_distance;

        //z_lenght *= 10;
        for(int x = 0; x < RENDER_W; x++){
            float x_pos = (x - RENDER_W / 2);
            int x_pix = z_lenght;
            int y_pix = (x_pos * z_lenght) / pixel_plane_distance;
            //u32 final_color = (y_pix & 15) * 15 << 8 | (x_pix & 15) * 15;
            //u32 final_color = (x_pix & 15) * 15;
            u32 final_color = (y_pix & 31) * 15 << 16 | (x_pix & 31) * 16 << 8;

            draw_pixel(buffer, vec2i{x, y}, final_color);
        }
    }


    //DRAW WALLS
}

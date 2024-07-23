#define RENDER_W 480
#define RENDER_H 320

//************** RENDER ******************* 
void draw_pixel(u32* buffer ,vec2 pos, u32 color){
    if(pos.x >= 0 && pos.x < RENDER_W && pos.y >= 0 && pos.y < RENDER_H)
        buffer[int(pos.x + pos.y * RENDER_W)] = color; 
}

void draw_quad(u32* buffer, vec2 pos, vec2 size, u32 color){
    for(int x = 0; x < size.x; x++){
        for(int y = 0; y < size.y; y++){
            draw_pixel(buffer ,vec2{(int)pos.x + x, (int)pos.y + y}, color);
        }
    }
}

void swap(float& a, float& b){
    float t = a;
    a = b;
    b = t;
}

void draw_line(u32* buffer ,vec2 pos1, vec2 pos2, u32 color){
    bool steep = false;
    if(abs(pos1.x - pos2.x) < abs(pos1.y - pos2.y)){
        swap(pos1.x, pos1.y);
        swap(pos2.x, pos2.y);
        steep = true;
    }

    if(pos1.x > pos2.x){
        swap(pos1.x, pos2.x);
        swap(pos1.y, pos2.y);
    }


    int dx = pos2.x - pos1.x;
    int dy = pos2.y - pos1.y;

    int derror2 = abs(dy) * 2;
    int error2 = 0;

    int y = pos1.y;

    for(int x = pos1.x; x <= pos2.x; x++){
        if(steep){
            draw_pixel(buffer, vec2{y,x}, color);
        }else{
            draw_pixel(buffer, vec2{x,y}, color);
        }
        error2 += derror2;
        if(error2 > dx){
            y += pos2.y > pos1.y ? 1 : -1;
            error2 -= dx*2;
        }
    }
}


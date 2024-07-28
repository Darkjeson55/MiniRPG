#define MAP_W 10
#define MAP_H 10

struct GameMemory{
    bool is_initialized = false;
    Texture base_texture;

    //RENDERING STUFF 
    float depth_buffer[RENDER_W * RENDER_H];
};

GameMemory* memory = nullptr;
Input* input = nullptr;
Platform* platfomr = nullptr;

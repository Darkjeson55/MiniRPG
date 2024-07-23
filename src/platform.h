#define RENDER_W 480
#define RENDER_H 320

#define WINDOW_SCALE 1
#define WINDOW_W RENDER_W * WINDOW_SCALE
#define WINDOW_H RENDER_H * WINDOW_SCALE


#define GAME_UPDATE(name) void name(u32* pixels, double dt)
typedef GAME_UPDATE(GameUpdate);
GAME_UPDATE(game_update_stub){}

struct game_code{
    HMODULE game_code_dll;
    FILETIME dll_last_write_time;
    GameUpdate* update;
    bool is_valid;
};

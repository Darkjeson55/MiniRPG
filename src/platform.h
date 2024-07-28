#define RENDER_W 240
#define RENDER_H 160

#define WINDOW_SCALE 4
#define WINDOW_W (RENDER_W * WINDOW_SCALE)
#define WINDOW_H (RENDER_H * WINDOW_SCALE)

#define Kilobytes(amt) ((amt) * 1024)
#define Megabytes(amt) (Kilobytes(amt) * 1024)
#define Gigabytes(amt) (Megabytes(amt) * 1024)

#define IS_KEY_DOWN(name) bool name(u32 key_code)
typedef IS_KEY_DOWN(IsKeyDown);
#define IS_KEY_UP(name) bool name(u32 key_code)
typedef IS_KEY_UP(IsKeyUp);
#define IS_KEY_PRESS(name) bool name(u32 key_code)
typedef IS_KEY_PRESS(IsKeyPress);

#define IS_MOUSE_BUTTON_DOWN(name) bool name(u32 key_code)
typedef IS_MOUSE_BUTTON_DOWN(IsMouseButtonDown);
#define IS_MOUSE_BUTTON_UP(name) bool name(u32 key_code)
typedef IS_MOUSE_BUTTON_UP(IsMouseButtonUp);
#define IS_MOUSE_BUTTON_PRESS(name) bool name(u32 key_code)
typedef IS_MOUSE_BUTTON_PRESS(IsMouseButtonPress);

struct Texture{
    u8* data;
    u32 w,h;
};

#define LOAD_TEXTURE(name) Texture name(const char* path)
typedef LOAD_TEXTURE(LoadTexture);

struct Input{
    IsKeyDown* is_key_down;
    IsKeyUp* is_key_up;
    IsKeyPress* is_key_press;

    IsMouseButtonDown* is_mouse_button_down;
    IsMouseButtonUp* is_mouse_button_up;
    IsMouseButtonPress* is_mouse_button_press;

    int mouse_x, mouse_y;
};

struct Platform{
    LoadTexture* load_texture;
};

#define GAME_UPDATE(name) void name(u32* pixels, Bump* game_memory, Input* g_input, Platform* g_platform, double dt)
typedef GAME_UPDATE(GameUpdate);
GAME_UPDATE(game_update_stub){}

struct game_code{
    HMODULE game_code_dll;
    FILETIME dll_last_write_time;
    GameUpdate* update;
    bool is_valid;
};


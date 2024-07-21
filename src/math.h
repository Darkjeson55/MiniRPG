typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

#define PI 3.14159265
#define DEG2RAD PI / 180
#define RAD2DEG 180 / PI


struct vec2{
    float x;
    float y;
};

struct vec2i{
    int x;
    int y;
};

vec2 operator-(vec2 a, vec2 b) { return vec2{a.x - b.x, a.y - b.y};}
vec2 operator+(vec2 a, vec2 b) { return vec2{a.x + b.x, a.y + b.y};}

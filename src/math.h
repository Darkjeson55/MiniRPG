struct vec2{
    float x;
    float y;
};

struct vec2i{
    int x;
    int y;
};

vec2 operator+(vec2 a, vec2 b) { return vec2{a.x + b.x, a.y + b.y};}
vec2 operator-(vec2 a, vec2 b) { return vec2{a.x - b.x, a.y - b.y};}

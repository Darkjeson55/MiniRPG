#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <windows.h>

#define SCREEN_W 640
#define SCREEN_H 480

uint32_t scrbuf[SCREEN_W * SCREEN_H];
bool quit = false;


struct Color{

};


void set_pixel(){

}

struct { 
    int width;
    int height;
    uint32_t* pixels;
} frame = {0};

static BITMAPINFO frame_bitmpa_info;
static HBITMAP frame_bitmap = 0;
static HDC frame_device_context = 0;


LRESULT window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam){

    switch(msg){
        case WM_QUIT:
        case WM_DESTROY: {
            quit = true;
        }break;

        case WM_PAINT: {
            PAINTSTRUCT paint;
            HDC device_context;
            device_context = BeginPaint(hwnd, &paint);
            BitBlt(device_context,
                    paint.rcPaint.left, paint.rcPaint.top,
                    paint.rcPaint.right - paint.rcPaint.left, 
                    paint.rcPaint.bottom - paint.rcPaint.top,
                    frame_device_context,
                    paint.rcPaint.left, paint.rcPaint.top,
                    SRCCOPY);
            EndPaint(hwnd, &paint);
        } break;

        case WM_SIZE: {
            frame_bitmpa_info.bmiHeader.biWidth = LOWORD(lparam);
            frame_bitmpa_info.bmiHeader.biHeight = HIWORD(lparam);

            if(frame_bitmap) DeleteObject(frame_bitmap);
            frame_bitmap = CreateDIBSection(NULL, &frame_bitmpa_info, DIB_RGB_COLORS, (void**)&frame.pixels, 0, 0);

            frame.width = LOWORD(lparam);
            frame.height = HIWORD(lparam);
        }break;

        default:{
            return DefWindowProc(hwnd, msg, wparam, lparam);
        }
    }
    return 0;
}

int main(int argc, char* argv[]){

    WNDCLASS wc = { CS_CLASSDC, window_proc };
    wc.lpszClassName = TEXT("WindowClass");
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    frame_bitmpa_info.bmiHeader.biSize = sizeof (frame_bitmpa_info.bmiHeader);
    frame_bitmpa_info.bmiHeader.biPlanes = 1;
    frame_bitmpa_info.bmiHeader.biBitCount = 32;
    frame_bitmpa_info.bmiHeader.biCompression = BI_RGB;
    frame_device_context = CreateCompatibleDC(0);


    HWND hwnd = CreateWindow( wc.lpszClassName, TEXT("MiniRPG"), WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_W, SCREEN_H, NULL, NULL, NULL, NULL); 

    SetTimer(NULL, 0, 16, NULL);


    while(!quit){
        MSG msg;
        while(PeekMessage(&msg, 0,0,0, PM_REMOVE)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        uint32_t p = 0;

        for(int x = 0; x < SCREEN_W; x++){
            for(int y = 0; y < SCREEN_H; y++){
                frame.pixels[x+y*SCREEN_H] = rand() % 255 << 16;

            }
        }

        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);

    }
    return 0;
}

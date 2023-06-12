#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


#define SCREEN_SIZE 512


// ピクセルを表示する座標
struct Point
{
    int x;
    int y;
};


class Application
{
private:
    SDL_Event       event;
    SDL_Window*     window;
    SDL_Renderer*   renderer;
    
    TTF_Font*       font;
    SDL_Surface*    fontSurface;
    SDL_Texture*    fontTexture;
    
    SDL_Texture*    screenTexture;
    
    int quit_flg;

    unsigned long   allCounter;
    unsigned long   inCicleCount;
    
    double pi;
    
    struct Point centerPosition;

    
    void Draw();
    bool CheckInCircle(struct Point c, struct Point p);

public:
    // コンストラクタ、デストラクタ
    Application();
    ~Application();

    // メインルーチン
    void Execute();

 
};

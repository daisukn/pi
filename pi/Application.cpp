#include "Application.h"
#include  <iostream>
#include <ctime>
#include <cstdlib>
#include <sstream>

// 初期化
Application::Application()
    : allCounter(0)
    , inCicleCount(0)
    , quit_flg(1)
{
    // SDL初期化
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL_Init(): " << SDL_GetError() << std::endl;
    }
        
    // ウインドウ生成
    window = SDL_CreateWindow("PI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_SIZE, SCREEN_SIZE, SDL_WINDOW_OPENGL);
    if(window == NULL)
    {
        std::cout << "Can not create window" << std::endl;
    }
    // レンダラー生成
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL )
    {
        std::cout << "Can not create renderer" << std::endl;
    }

    
    if (TTF_Init() < 0)
    {
        std::cout << "TTF_INit(): " << TTF_GetError() << std::endl;
     }
    // フォント生成
    font = TTF_OpenFont("rounded.ttf", 12);
    if(font == NULL)
    {
        std::cout << "can not create Font" << std::endl;
    }
    SDL_Color color = {255, 255, 255, 255};
    
    fontSurface = TTF_RenderText_Solid(font, "", color);
    fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
    
    screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_SIZE, SCREEN_SIZE);
    
    // 中心点を計算
    centerPosition.x = SCREEN_SIZE / 2;
    centerPosition.y = SCREEN_SIZE / 2;
    
    // 乱数テーブルを初期化
    std::srand((unsigned int)time(NULL));

}

// デストラクタ
Application::~Application()
{
    
    // リリース処理
    if (fontTexture) SDL_DestroyTexture(fontTexture);
    if (fontSurface) SDL_FreeSurface(fontSurface);
    if (screenTexture) SDL_DestroyTexture(screenTexture);

    if (font) TTF_CloseFont(font);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);

    SDL_Quit();
}


// 描画
void Application::Draw()
{
    // 背景クリア
    SDL_SetRenderDrawColor(renderer, 32, 32, 32, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    

    SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
    
    
    // テキスト表示
    SDL_Color color = {255, 255, 255, 255};
    std::stringstream ss;
    ss << "Trial Count = " << allCounter;
    ss << "   PI = " << pi;

    fontSurface = TTF_RenderText_Solid(font, ss.str().c_str(), color);
    fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
    
    SDL_Rect dstrect = { 5, 5, fontSurface->w, fontSurface->h };
    SDL_RenderCopy(renderer, fontTexture, NULL, &dstrect);
    
    // 描画
    SDL_RenderPresent(renderer);

}


bool Application::CheckInCircle(struct Point c, struct Point p)
{

    double w = p.x - c.x;
    double h = p.y - c.y;
    
    if( (w*w + h*h) <= (SCREEN_SIZE/2)*(SCREEN_SIZE/2))
    {
        return true;
    }
    
    return false;
}

void Application::Execute()
{
    // メインループ
    while(quit_flg) 
    {
        // 画面描画
        Draw();
        
        allCounter++;
        
        struct Point p;
        p.x = std::rand() % SCREEN_SIZE;
        p.y = std::rand() % SCREEN_SIZE;
        
        SDL_SetRenderTarget(renderer, screenTexture);

        if(CheckInCircle(centerPosition,p))
        {
            SDL_SetRenderDrawColor(renderer, 255, 64, 64, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawPoint(renderer, p.x, p.y);
            
            inCicleCount++;
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 0, 64, 64, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawPoint(renderer, p.x, p.y);
            
        }
        SDL_SetRenderTarget(renderer, NULL); // レンダーターゲットをデフォルトに戻す
        
        double area = (SCREEN_SIZE*SCREEN_SIZE) * inCicleCount / allCounter;
        pi = area / ((SCREEN_SIZE/2) * (SCREEN_SIZE/2));
        std::cout << area << std::endl;
        
        // アプリ終了条件
        while( SDL_PollEvent(&event) )
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit_flg = 0;
                break;
            }
        }
    }
}

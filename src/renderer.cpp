#include "renderer.hpp"

#include <iostream> // For debugging

#include <SDL.h>
#include <SDL_ttf.h>

#include "config.hpp"

/*
 * ====================================
 * Public methods start here
 * ====================================
 */
void Renderer::setBackgroundColor(Uint8 r, Uint8 g, Uint8 b) {
    SDL_SetRenderDrawColor(mSDLRenderer, r, g, b, 0xFF);
}

void Renderer::initialize (SDL_Window *window)
{
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    mediumFont = TTF_OpenFont("../../assets/munro-small.ttf", 30);
    bigFont = TTF_OpenFont("../../assets/munro.ttf", 50);
    #else
    mediumFont = TTF_OpenFont("../assets/munro-small.ttf", 30);
    bigFont = TTF_OpenFont("../assets/munro.ttf", 50);
    #endif
    
    if (mediumFont == nullptr || bigFont == nullptr)
    {
        std::cerr << "Could not load font! SDL_ttf error: " << TTF_GetError() << '\n';
    }
    
    mSDLRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    // 논리적 해상도 설정 - 이게 중요!
    SDL_RenderSetLogicalSize(mSDLRenderer, config::logical_window_width, config::logical_window_height);
    
    clearScreen();
}

Renderer::~Renderer ()
{
    SDL_DestroyRenderer(mSDLRenderer);
    mSDLRenderer = nullptr;
}

void Renderer::clearScreen ()
{
    // 이미 setBackgroundColor로 색 설정되었으므로 여기선 draw color 설정 생략
    SDL_RenderClear(mSDLRenderer);
}


void Renderer::renderTexture (Texture *texture, int x, int y)
{
    texture->renderCentered(x, y);
}

void Renderer::updateScreen ()
{
    SDL_RenderPresent(mSDLRenderer);
}

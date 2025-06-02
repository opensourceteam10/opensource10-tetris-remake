#include "texture.hpp"

#include <iostream>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "config.hpp"
#include "game.hpp"
#include "renderer.hpp"

Texture::Texture() {
    mTexture = nullptr;
    width = 0;
    height = 0;
}

Texture::~Texture()
{
    free();
}

void Texture::free()
{
    if (mTexture != nullptr)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
        width = 0;
        height = 0;
    }
}

// 간단한 이미지 로딩 (경로 문제 해결)
bool Texture::loadFromImage (std::string path) 
{
    bool success = true;
    free();
    
    // 여러 경로 시도
    std::vector<std::string> paths = {
        path,
        "assets/" + path,
        "../assets/" + path,
        "../../assets/" + path
    };
    
    SDL_Surface *tempSurf = nullptr;
    
    for (const auto& tryPath : paths) {
        tempSurf = IMG_Load(tryPath.c_str());
        if (tempSurf != nullptr) {
            std::cout << "Successfully loaded: " << tryPath << std::endl;
            break;
        }
    }
    
    if (tempSurf == nullptr)
    {
        std::cerr << "Texture: Could not load image from any path for: " << path << '\n';
        std::cerr << "IMG_Load error: " << IMG_GetError() << '\n';
        success = false;
    }
    else
    {
        mTexture = SDL_CreateTextureFromSurface(Game::getInstance()->mRenderer->mSDLRenderer, tempSurf);
        if (mTexture == nullptr) {
            std::cerr << "Could not create texture from surface! SDL error: " << SDL_GetError() << '\n';
            success = false;
        } else {
            width = tempSurf->w;
            height = tempSurf->h;
        }
        SDL_FreeSurface(tempSurf);
    }
    return success;
}

bool Texture::loadFromText (std::string text, TTF_Font *font, SDL_Color text_color)
{
    bool success = true;
    free();
    
    if (font == nullptr) {
        std::cerr << "Font is null!" << '\n';
        return false;
    }
    
    SDL_Surface *text_surface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), text_color, config::logical_window_width);
    if (text_surface == nullptr)
    {
        std::cerr << "Could not create surface from text! SDL_ttf error: " << TTF_GetError() << '\n';
        success = false;
    }
    else
    {
        mTexture = SDL_CreateTextureFromSurface(Game::getInstance()->mRenderer->mSDLRenderer, text_surface);
        if (mTexture == nullptr)
        {
            std::cerr << "Could not create texture from rendered text! SDL error: " << SDL_GetError() << '\n';
            success = false;
        }
        else
        {
            width = text_surface->w;
            height = text_surface->h;
        }
        SDL_FreeSurface(text_surface);
    }
    return success;
}

void Texture::render (int x, int y, SDL_Rect *clip)
{
    if (mTexture == nullptr) return;
    
    SDL_Rect r = {x, y, width, height};
    if (clip != nullptr)
    {
        r.w = clip->w;
        r.h = clip->h;
    }
    SDL_RenderCopy(Game::getInstance()->mRenderer->mSDLRenderer, mTexture, clip, &r);
}

void Texture::renderCentered (int x, int y)
{
    if (mTexture == nullptr) return;
    
    SDL_Rect r = {x-(width/2), y-(height/2), width, height};
    SDL_RenderCopy(Game::getInstance()->mRenderer->mSDLRenderer, mTexture, nullptr, &r);
}

void Texture::setAlphaMode (Uint8 alpha)
{
    if (mTexture != nullptr) {
        SDL_SetTextureAlphaMod (mTexture, alpha);
    }
}

int Texture::getWidth()
{
    return width;
}

int Texture::getHeight()
{
    return height;
}
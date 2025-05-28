#include "button.hpp"
#include "game.hpp" // Game::getInstance()->mRenderer->font 등
#include <SDL_ttf.h>

// 이미지 버튼 생성자
Button::Button(std::string path, void (*callback)(), int posX, int posY)
{
    callbackFunction = callback;
    position_x = posX;
    position_y = posY;
    texture = new Texture;
    texture->loadFromImage(path);
    width = texture->getWidth();
    height = texture->getHeight();
    isTextButton = false;
}

// 텍스트 버튼 생성자
Button::Button(const std::string& text, void (*callback)(), int posX, int posY, int w, int h)
{
    callbackFunction = callback;
    position_x = posX;
    position_y = posY;
    width = w;
    height = h;
    mText = text;
    isTextButton = true;
    texture = nullptr;
}

Button::~Button()
{
    if (texture)
        texture->free();
}

bool Button::loadTexture(std::string path)
{
    if (texture)
        texture->free();
    texture = new Texture;
    bool result = texture->loadFromImage(path);
    width = texture->getWidth();
    height = texture->getHeight();
    return result;
}

void Button::draw()
{
    if (isTextButton) {
        SDL_Renderer* renderer = Game::getInstance()->mRenderer->mSDLRenderer;
        // 버튼 배경
        SDL_Rect rect = {position_x, position_y, width, height};
        SDL_SetRenderDrawColor(renderer, 80, 80, 200, 255);
        SDL_RenderFillRect(renderer, &rect);

        // 텍스트 렌더링
        TTF_Font* font = Game::getInstance()->mRenderer->mediumFont;
        SDL_Color color = {255,255,255,255};
        SDL_Surface* surf = TTF_RenderUTF8_Blended(font, mText.c_str(), color);
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_Rect dst = {position_x + (width-surf->w)/2, position_y + (height-surf->h)/2, surf->w, surf->h};
        SDL_RenderCopy(renderer, tex, nullptr, &dst);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
    } else {
        texture->render(position_x, position_y);
    }
}

int Button::getX() { return position_x; }
int Button::getY() { return position_y; }
int Button::getWidth() { return width; }
int Button::getHeight() { return height; }


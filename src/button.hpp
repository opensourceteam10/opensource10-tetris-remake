#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <string>
#include <functional>
#include "texture.hpp"
#include <SDL_ttf.h>

// Represents a standard button with a texture and a certain position that can be selected. If selected, the callback function can be called
class Button
{
public:
    // 기존 방식 (이미지 버튼)
    Button(std::string path, void (*callback)(), int posX = 0, int posY = 0);
    // 텍스트 버튼용 오버로드
    Button(const std::string& text, void (*callback)(), int posX, int posY, int w, int h);

    ~Button();

    bool loadTexture(std::string path);
    void draw();
    void (*callbackFunction)();

    int getX();
    int getY();
    int getWidth();
    int getHeight();

private:
    int position_x;
    int position_y;
    int width;
    int height;
    Texture *texture = nullptr;
    
    bool isTextButton = false;
    std::string mText;
};

#endif // BUTTON_HPP


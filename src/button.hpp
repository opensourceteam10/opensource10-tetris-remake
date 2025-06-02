#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <string>
#include <functional>
#include "texture.hpp"

class Button {
public:
    // 이미지 버튼
    Button(std::string imagePath, std::function<void()> callback, int posX, int posY);

    // 텍스트 버튼
    Button(const std::string& text, std::function<void()> callback, int posX, int posY, int w, int h);

    ~Button();

    void draw();
    void execute(); // 클릭 시 실행
    void run(){
	if (callbackFunction) callbackFunction();
    }
    int getX();
    int getY();
    int getWidth();
    int getHeight();

private:
    std::function<void()> callbackFunction;
    std::string mText;
    Texture* texture = nullptr;

    int position_x = 0;
    int position_y = 0;
    int width = 0;
    int height = 0;

    bool isTextButton = false;
};

#endif // BUTTON_HPP

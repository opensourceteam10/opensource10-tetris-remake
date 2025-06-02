#include "optionsstate.hpp"
#include "selectioninputhandler.hpp"
#include "game.hpp"

OptionState::OptionState(InputManager* inputManager)
    : State(inputManager), index(0) {}

OptionState::~OptionState() {
    // 하위 클래스에서 exit()로 리소스 정리 강제
}

void OptionState::run() {
    update();
    draw();
}

void OptionState::draw() {
    Game::getInstance()->mRenderer->clearScreen();

    for (auto btn : mButtons) btn->draw();
    SelectionInputHandler::renderHighlight(mButtons, index);

    drawOptions();  // 서브클래스에서 그리기 구체화

    Game::getInstance()->mRenderer->updateScreen();
}

void OptionState::addButton(Button* button) {
    mButtons.push_back(button);
}

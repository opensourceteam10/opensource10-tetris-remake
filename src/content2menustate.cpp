#include <iostream>  // 맨 위에 추가
#include "content2menustate.hpp"
#include "game.hpp"
#include "config.hpp"
#include "gamestate.hpp"
#include "optionsstate.hpp"
#include "selectioninputhandler.hpp"

Content2MenuState::Content2MenuState(InputManager* manager)
    : MenuState(manager) {}

void Content2MenuState::initialize() {
    // 제목 텍스트
    title_text = new Texture();
    title_text->loadFromText("Content 2 Menu", Game::getInstance()->mRenderer->bigFont, config::default_text_color);

    // 기존 버튼 제거
    for (auto button : mButtons)
        delete button;
    mButtons.clear();

    int buttonWidth = 200;
    int buttonHeight = 50;
    int centerX = (config::logical_window_width - buttonWidth) / 2;

    // 버튼 추가
    addButton(new Button(
        "Start Content",
        []() { Game::getInstance()->pushNewState<ModeSelectState>(); },
        centerX, 130, buttonWidth, buttonHeight
    ));

    addButton(new Button(
        "Options",
        []() { 
            std::cout << "ContentOption" << std::endl;
            /*Game::getInstance()->pushNewState<Content2OptionsState>();*/ 
        },
        centerX, 190, buttonWidth, buttonHeight
    ));

    addButton(new Button(
        "Back",
        []() { Game::getInstance()->popState(); },
        centerX, 250, buttonWidth, buttonHeight
    ));
}

void Content2MenuState::draw() {
    Game::getInstance()->mRenderer->clearScreen();

    if (title_text)
        title_text->renderCentered(config::logical_window_width / 2, 50);

    for (auto button : mButtons)
        button->draw();

    SelectionInputHandler::renderHighlight(mButtons, index);

    Game::getInstance()->mRenderer->updateScreen();
}

#include "MainMenuState.hpp"
#include "game.hpp"
#include "config.hpp"
#include "ContentSelectMenuState.hpp"
#include "MainOptionState.hpp"
#include "SelectionInputHandler.hpp"
#include "exoptionstate.hpp"


MainMenuState::MainMenuState(InputManager* manager)
    : MenuState(manager) {}

void MainMenuState::initialize() {
    title_text = new Texture();
    title_text->loadFromText("PixelTetris!", Game::getInstance()->mRenderer->bigFont, config::default_text_color);

    // 기존 버튼들 정리
    for (auto button : mButtons) {
        delete button;
    }
    mButtons.clear();
    
    int buttonWidth = 200;
    int buttonHeight = 50;
    int centerX = (config::logical_window_width - buttonWidth) / 2;

    addButton(new Button(
    "PLAY",
    []() { Game::getInstance()->pushNewState<ContentSelectMenuState>(); },
    centerX, 130, buttonWidth, buttonHeight
    ));

    addButton(new Button(
    "OPTIONS",
    []() { Game::getInstance()->pushNewState<OptionsState>(); },
    centerX, 190, buttonWidth, buttonHeight
    ));

    addButton(new Button(
    "EXIT",
    []() { Game::getInstance()->popState(); },
    centerX, 250, buttonWidth, buttonHeight
    ));

      
}

void MainMenuState::draw() {
    Game::getInstance()->mRenderer->clearScreen();

    if (title_text)
        title_text->renderCentered(config::logical_window_width / 2, 50);

    for (auto button : mButtons)
        button->draw();

    SelectionInputHandler::renderHighlight(mButtons, index);

    Game::getInstance()->mRenderer->updateScreen();
}

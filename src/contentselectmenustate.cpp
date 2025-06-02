#include "content1menustate.hpp"
#include "content2menustate.hpp"
#include "content3menustate.hpp"
#include "selectioninputhandler.hpp"
#include "ContentSelectMenuState.hpp"
#include "game.hpp"
#include "config.hpp"
#include <iostream>

ContentSelectMenuState::ContentSelectMenuState(InputManager* manager)
    : MenuState(manager), descriptionTexture(nullptr) {}

void ContentSelectMenuState::initialize() {
    index = 0;

    title_text = new Texture();
    title_text->loadFromText("TETRIS", Game::getInstance()->mRenderer->bigFont, config::default_text_color);

    descriptionTexture = new Texture();
    descriptionTexture->loadFromText("Select Content", Game::getInstance()->mRenderer->mediumFont, config::default_text_color);

    int centerX = (config::logical_window_width - 200) / 2;

    addButton(new Button(
        "Content 1",
        []() {
            std::cout << "Content 1 Started!" << std::endl;
            Game::getInstance()->pushNewState<Content1MenuState>();
        },
        centerX, 150, 200, 40
    ));

    addButton(new Button(
        "Content 2",
        []() {
            std::cout << "Content 2 Started!" << std::endl;
            Game::getInstance()->pushNewState<Content2MenuState>();
        },
        centerX, 200, 200, 40
    ));

    addButton(new Button(
        "Content 3",
        []() {
            std::cout << "Content 3 Started!" << std::endl;
            Game::getInstance()->pushNewState<Content3MenuState>();
        },
        centerX, 250, 200, 40
    ));

    addButton(new Button(
        "Back",
        []() {
            Game::getInstance()->popState();
        },
        centerX, 300, 200, 40
    ));
}

void ContentSelectMenuState::draw() {
    Game::getInstance()->mRenderer->clearScreen();

    if (title_text)
        title_text->renderCentered(config::logical_window_width / 2, 50);
    
    if (descriptionTexture)
        descriptionTexture->renderCentered(config::logical_window_width / 2, 90);

    for (auto button : mButtons)
        button->draw();

    SelectionInputHandler::renderHighlight(mButtons, index);

    Game::getInstance()->mRenderer->updateScreen();
}


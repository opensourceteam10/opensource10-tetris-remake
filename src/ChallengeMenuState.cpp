#include "ChallengeMenuState.hpp"
#include "game.hpp"
#include "config.hpp"
#include "SpeedChallengeState.hpp"
#include "InvisibleChallengeState.hpp"
#include "selectioninputhandler.hpp"
#include <iostream>

ChallengeMenuState::ChallengeMenuState(InputManager* manager)
    : MenuState(manager), titleTexture(nullptr), descriptionTexture(nullptr) {}

ChallengeMenuState::~ChallengeMenuState() {
    exit();
}

void ChallengeMenuState::initialize() {
    index = 0;

    titleTexture = new Texture();
    titleTexture->loadFromText("CHALLENGE MODE", Game::getInstance()->mRenderer->bigFont, config::default_text_color);

    descriptionTexture = new Texture();
    descriptionTexture->loadFromText("Choose Your Challenge", Game::getInstance()->mRenderer->mediumFont, config::default_text_color);

    const int buttonWidth = 280;
    const int buttonHeight = 50;
    const int buttonSpacing = 70;
    const int startY = 150;
    const int buttonX = (config::logical_window_width - buttonWidth) / 2;

    addButton(new Button(
        "Speed Challenge",
        []() {
            Game::getInstance()->pushNewState<SpeedChallengeState>();
        },
        buttonX, startY, buttonWidth, buttonHeight
    ));

    addButton(new Button(
        "Invisible Challenge",
        []() {
            Game::getInstance()->pushNewState<InvisibleChallengeState>();
        },
        buttonX, startY + buttonSpacing, buttonWidth, buttonHeight
    ));

    addButton(new Button(
        "Back",
        []() {
            Game::getInstance()->popState();
        },
        buttonX, startY + 2 * buttonSpacing, buttonWidth, buttonHeight
    ));
}

void ChallengeMenuState::draw() {
    Game::getInstance()->mRenderer->clearScreen();

    if (titleTexture)
        titleTexture->renderCentered(config::logical_window_width / 2, 50);

    if (descriptionTexture)
        descriptionTexture->renderCentered(config::logical_window_width / 2, 90);

    for (auto button : mButtons)
        button->draw();

    SelectionInputHandler::renderHighlight(mButtons, index);

    Game::getInstance()->mRenderer->updateScreen();
}

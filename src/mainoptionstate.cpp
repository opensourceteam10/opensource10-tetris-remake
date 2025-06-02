#include "MainOptionState.hpp"
#include "game.hpp"
#include "renderer.hpp"
#include "config.hpp"
#include "selectioninputhandler.hpp"
#include <iostream>

MainMenuOptionState::MainMenuOptionState(InputManager* inputManager)
    : OptionState(inputManager), resolution_index(1), ghostblock_enabled(true),
      title_text(nullptr), resolution_label(nullptr), resolution_value(nullptr),
      ghostblock_label(nullptr), ghostblock_value(nullptr),
      left_arrow(nullptr), right_arrow(nullptr) {}

MainMenuOptionState::~MainMenuOptionState() {
    exit();
}

void MainMenuOptionState::initialize() {
    index = 0;

    title_text = new Texture();
    title_text->loadFromText("Options", Game::getInstance()->mRenderer->bigFont, config::default_text_color);

    resolution_label = new Texture();
    resolution_label->loadFromText("Resolution", Game::getInstance()->mRenderer->mediumFont, config::default_text_color);

    resolution_value = new Texture();
    ghostblock_label = new Texture();
    ghostblock_label->loadFromText("Ghost Block", Game::getInstance()->mRenderer->mediumFont, config::default_text_color);

    ghostblock_value = new Texture();

    left_arrow = new Texture();
    left_arrow->loadFromImage("assets/arrow-left.png");

    right_arrow = new Texture();
    right_arrow->loadFromImage("assets/arrow-right.png");

    updateResolutionText();
    updateGhostblockText();

    addButton(new Button("Back", []() {
        Game::getInstance()->popState();
    }, 330, 400));
}

void MainMenuOptionState::update() {
    SelectionInputHandler::handle(mInputManager, mButtons, index, nextStateID);

    // 해상도 및 고스트 블록 항목 처리
    Action action = mInputManager->getAction();  // getInput()이 아닌 getAction()을 직접 쓰는 구조

    if (action == Action::move_left || action == Action::move_right) {
        if (index == 0) {
            resolution_index += (action == Action::move_right) ? 1 : -1;
            resolution_index = std::max(0, std::min(2, resolution_index)); // 범위 제한
            updateResolutionText();
        } else if (index == 1) {
            ghostblock_enabled = !ghostblock_enabled;
            updateGhostblockText();
        }
    }
}

void MainMenuOptionState::exit() {
    delete title_text;
    delete resolution_label;
    delete resolution_value;
    delete ghostblock_label;
    delete ghostblock_value;
    delete left_arrow;
    delete right_arrow;

    for (auto btn : mButtons) delete btn;
    mButtons.clear();
}

void MainMenuOptionState::drawOptions() {
    Renderer* renderer = Game::getInstance()->mRenderer;

    if (title_text) title_text->render(330, 50);

    resolution_label->render(250, 150);
    resolution_value->render(450, 150);
    left_arrow->render(420, 150);
    right_arrow->render(520, 150);

    ghostblock_label->render(250, 200);
    ghostblock_value->render(450, 200);
}

void MainMenuOptionState::updateResolutionText() {
    std::string text;
    switch (resolution_index) {
        case 0: text = "Low"; break;
        case 1: text = "Medium"; break;
        case 2: text = "High"; break;
    }
    resolution_value->loadFromText(text, Game::getInstance()->mRenderer->mediumFont, config::default_text_color);
}

void MainMenuOptionState::updateGhostblockText() {
    ghostblock_value->loadFromText(ghostblock_enabled ? "ON" : "OFF", Game::getInstance()->mRenderer->mediumFont, config::default_text_color);
}

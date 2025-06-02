#include "menustate.hpp"
#include "SelectionInputHandler.hpp"
#include "game.hpp"

MenuState::MenuState(InputManager* manager)
    : State(manager), title_text(nullptr), index(0) {}

MenuState::~MenuState() {
    exit();
}

void MenuState::run() {
    update();
    draw();
}

void MenuState::update() {
    SelectionInputHandler::handle(mInputManager, mButtons, index, nextStateID);
}

void MenuState::exit() {
    for (auto button : mButtons)
        delete button;
    mButtons.clear();

    if (title_text) {
        delete title_text;
        title_text = nullptr;
    }
}

void MenuState::addButton(Button* button) {
    mButtons.push_back(button);
}

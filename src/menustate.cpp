#include "menustate.hpp"
#include "game.hpp"
#include "config.hpp"
#include "ModeSelectState.hpp"
#include "LobbyState.hpp"
#include "MultiState.hpp"

MenuState::MenuState(InputManager *manager)
{
    mManager = manager;
}

MenuState::~MenuState()
{
    for (Button *b : mButtons)
        delete b;

    mButtons.clear();
}

void MenuState::initialize()
{
#ifdef __APPLE__
    mButtons.push_back(new Button ("../../assets/button-play.png", &Game::pushModeSelect, (config::logical_window_width-80)/2, 130));
    mButtons.push_back(new Button ("../../assets/button-options.png", &Game::pushOptions, (config::logical_window_width-80)/2, 180));
    mButtons.push_back(new Button ("../../assets/button-multiplay.png", &Game::pushMulti, (config::logical_window_width-80)/2, 230));
    mButtons.push_back(new Button ("../../assets/button-exit.png", &Game::goBack, (config::logical_window_width-80)/2, 280));
#else
    mButtons.push_back(new Button ("../assets/button-play.png", &Game::pushModeSelect, (config::logical_window_width-80)/2, 130));
    mButtons.push_back(new Button ("../assets/button-options.png", &Game::pushOptions, (config::logical_window_width-80)/2, 180));
    mButtons.push_back(new Button ("../assets/button-multiplay.png", &Game::pushMulti, (config::logical_window_width-80)/2, 230));
    mButtons.push_back(new Button ("../assets/button-exit.png", &Game::goBack, (config::logical_window_width-80)/2, 280));
#endif
}

void MenuState::run()
{
    for (Button *b : mButtons)
        b->update(mManager);
}

void MenuState::draw()
{
    for (Button *b : mButtons)
        b->draw();
}

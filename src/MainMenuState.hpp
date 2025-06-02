#ifndef MAINMENUSTATE_HPP
#define MAINMENUSTATE_HPP

#include "menustate.hpp"

class MainMenuState : public MenuState {
public:
    MainMenuState(InputManager* manager);

    void initialize() override;
    void draw() override;
};

#endif // MAINMENUSTATE_HPP

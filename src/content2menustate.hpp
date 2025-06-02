#ifndef CONTENT2MENUSTATE_HPP
#define CONTENT2MENUSTATE_HPP

#include "menustate.hpp"
#include "texture.hpp"

class Content2MenuState : public MenuState {
public:
    Content2MenuState(InputManager* manager);
    void initialize() override;
    void draw() override;

private:
    Texture* title_text = nullptr;
};

#endif

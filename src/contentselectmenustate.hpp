#ifndef CONTENTSELECTMENUSTATE_HPP
#define CONTENTSELECTMENUSTATE_HPP

#include "menustate.hpp"
#include "button.hpp"
#include "texture.hpp"

class ContentSelectMenuState : public MenuState {
public:
    ContentSelectMenuState(InputManager* manager);

    void initialize() override;
    void draw() override;

private:
    Texture* descriptionTexture;
};

#endif // CONTENTSELECTMENUSTATE_HPP

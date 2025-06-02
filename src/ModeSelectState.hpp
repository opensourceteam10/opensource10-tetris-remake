#ifndef MODESELECTSTATE_HPP
#define MODESELECTSTATE_HPP

#include "menustate.hpp"
#include "button.hpp"
#include "texture.hpp"

class ModeSelectState : public MenuState {
public:
    ModeSelectState(InputManager* manager);

    void initialize() override;
    void draw() override;

private:
    Texture* descriptionTexture = nullptr;
};

#endif // MODESELECTSTATE_HPP

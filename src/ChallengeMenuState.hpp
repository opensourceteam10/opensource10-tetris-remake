#ifndef CHALLENGEMENUSTATE_HPP
#define CHALLENGEMENUSTATE_HPP

#include "menustate.hpp"
#include "texture.hpp"

class ChallengeMenuState : public MenuState {
public:
    ChallengeMenuState(InputManager* manager);
    ~ChallengeMenuState();

    void initialize() override;
    void draw() override;

private:
    Texture* titleTexture;
    Texture* descriptionTexture;
};

#endif // CHALLENGEMENUSTATE_HPP


#ifndef MAINMENUOPTIONSTATE_HPP
#define MAINMENUOPTIONSTATE_HPP

#include "optionsstate.hpp"
#include "texture.hpp"

class MainMenuOptionState : public OptionState {
public:
    MainMenuOptionState(InputManager* inputManager);
    ~MainMenuOptionState();

    void initialize() override;
    void update() override;
    void exit() override;

protected:
    void drawOptions() override;

private:
    Texture* title_text;

    Texture* resolution_label;
    Texture* resolution_value;
    Texture* ghostblock_label;
    Texture* ghostblock_value;

    Texture* left_arrow;
    Texture* right_arrow;

    int resolution_index;
    bool ghostblock_enabled;

    void updateResolutionText();
    void updateGhostblockText();
};

#endif // MAINMENUOPTIONSTATE_HPP


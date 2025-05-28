#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include <vector>

#include "button.hpp"
#include "inputmanager.hpp"
#include "renderer.hpp"
#include "state.hpp"
#include "texture.hpp"

// The main menu of the game; It is the state shown when the game starts
class MenuState: public State
{
public:
    MenuState (InputManager *manager);
    ~MenuState ();
    void initialize () override;
    void exit () override;
    
    void run () override;
    void update () override;
    void draw () override;

    void addButton (Button *button);

private:
    std::vector<Button*> mButtons;
    Texture *title_text;
    int index;                          // Shows the current button selected
};

#endif // MENUSTATE_HPP
#ifndef STATE_HPP
#define STATE_HPP

#include "inputmanager.hpp"

// 전방 선언
class Renderer;

// 상태 ID 열거형
enum StateID {
    STATE_NULL,
    STATE_EXIT,
    STATE_MENU,
    STATE_GAME,
    STATE_PAUSED,
    STATE_OPTIONS
};

class State {
public:
    State(InputManager *manager);
    virtual ~State() {}

    virtual void initialize() = 0;
    virtual void run() = 0;
    virtual void draw() = 0;
    virtual void exit() = 0;
    virtual void update() = 0;

protected:
    InputManager *mInputManager;
    Renderer *mRenderer;
    StateID nextStateID;
};

#endif // STATE_HPP
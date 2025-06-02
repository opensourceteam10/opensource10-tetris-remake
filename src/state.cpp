#include "state.hpp"
#include "game.hpp"
#include "renderer.hpp"

State::State(InputManager *manager)
{
    mInputManager = manager;
    mRenderer = Game::getInstance()->mRenderer;
    nextStateID = STATE_NULL;
}
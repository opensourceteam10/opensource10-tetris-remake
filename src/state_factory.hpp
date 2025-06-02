#ifndef STATEFACTORY_HPP
#define STATEFACTORY_HPP

#include "state.hpp"
#include "gamestate.hpp"
#include "multistate.hpp"
#include "challengemenustate.hpp"
#include "pausedstate.hpp"
#include "mainoptionstate.hpp"
#include "multiState.hpp"
#include "modeselectstate.hpp"
#include "speedchallengestate.hpp"



class InputManager;

class StateFactory {
public:
    template <typename T>
    static State* create(InputManager* manager) {
        State* state = new T(manager);
        state->initialize();
        return state;
    }
};

#endif // STATEFACTORY_HPP

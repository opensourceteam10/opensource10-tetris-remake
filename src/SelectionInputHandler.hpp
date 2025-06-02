#ifndef SELECTIONINPUTHANDLER_HPP
#define SELECTIONINPUTHANDLER_HPP

#include <vector>
#include "state.hpp"


class InputManager;
class Button;

class SelectionInputHandler {
public:
    static void handle(class InputManager* input, std::vector<class Button*>& buttons, size_t& index, StateID& nextStateID);
    static void renderHighlight(std::vector<Button*>& buttons, size_t index);
};

#endif // SELECTIONINPUTHANDLER_HPP

#include "selectioninputhandler.hpp"
#include "inputmanager.hpp"
#include "button.hpp"
#include "game.hpp"

void SelectionInputHandler::handle(InputManager* input, std::vector<Button*>& buttons, size_t& index,StateID& nextStateID) {
    
    while (input->pollAction()) {
        if (input->isGameExiting())
        {
            nextStateID = STATE_EXIT;
            break;
        }
        switch (input->getAction()) {
            case Action::move_up:
                if (index > 0) --index;
                break;
            case Action::move_down:
                if (index < buttons.size() - 1) ++index;
                break;
            case Action::select:
                buttons[index]->run();
                break;
            case Action::back:
                Game::getInstance()->popState();
                break;
            default:
                break;
        }
    }
}

void SelectionInputHandler::renderHighlight(std::vector<Button*>& buttons, size_t index) {
    if (index >= buttons.size()) return;

    SDL_Rect highlight_box = {
        buttons[index]->getX(),
        buttons[index]->getY(),
        buttons[index]->getWidth(),
        buttons[index]->getHeight()
    };

    SDL_SetRenderDrawBlendMode(Game::getInstance()->mRenderer->mSDLRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(Game::getInstance()->mRenderer->mSDLRenderer, 255, 255, 255, config::transparency_alpha - 20);
    SDL_RenderFillRect(Game::getInstance()->mRenderer->mSDLRenderer, &highlight_box);
    SDL_SetRenderDrawBlendMode(Game::getInstance()->mRenderer->mSDLRenderer, SDL_BLENDMODE_NONE);
}

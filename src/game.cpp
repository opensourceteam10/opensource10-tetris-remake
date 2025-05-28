#include "game.hpp"

#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "button.hpp"
#include "ChallengeMenuState.hpp"
#include "config.hpp"
#include "gamestate.hpp"
#include "menustate.hpp"
#include "optionsstate.hpp"
#include "pausedstate.hpp"
#include "state.hpp"
#include "LobbyState.hpp"
#include "SpeedChallengeState.hpp"  
#include "ModeSelectState.hpp"  // ← 추가된 부분

/*
 * ====================================
 * Public methods start here
 * ====================================
 */

Game *Game::getInstance()
{
    if (mInstance == nullptr)
    {
        mInstance = new Game;
    }
    return mInstance;
}

// The function called to initialize everything; Pushes the main menu state to the front
bool Game::initialize()
{
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Could not initialize SDL! SDL_Error: " << SDL_GetError() << '\n';
        success = false;
    }
    else
    {
        mWindow = SDL_CreateWindow(config::window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        config::logical_window_width, config::logical_window_height, SDL_WINDOW_SHOWN);

        if (mWindow == nullptr)
        {
            std::cerr << "Could not create window! SDL_Error: " << SDL_GetError() << '\n';
            success = false;
        }
        else
        {
            if (IMG_Init(IMG_INIT_PNG) == 0 || IMG_Init(IMG_INIT_JPG) == 0)
            {
                std::cerr << "Could not initialize SDL_image! SDL_image error: " << IMG_GetError() << '\n';
                success = false;
            }
            if (TTF_Init() == -1)
            {
                std::cerr << "Could not initialize SDL_ttf! SDL_ttf error: " << TTF_GetError() << '\n';
                success = false;
            }
        }
    }
    mRenderer = new Renderer;
    mRenderer->initialize(mWindow);

    // The logical resolution of the game never changes; We just alter the scaling
    SDL_RenderSetLogicalSize(mRenderer->mSDLRenderer, config::logical_window_width, config::logical_window_height);
    SDL_SetWindowSize(mWindow, config::logical_window_width*config::resolution_scaling, config::logical_window_height*config::resolution_scaling);
    SDL_SetWindowPosition(mWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    
    mManager = new InputManager;

    // Now load the main menu screen
    mMainMenuState = new MenuState(mManager);
    mMainMenuState->initialize();
    pushState(mMainMenuState);
    return success;
}

// Deletes all states loaded, deletes the window and closes all SDL services
void Game::exit ()
{
    for (auto i : mStates)
    {
        delete i;
    }

    delete mRenderer;

    SDL_DestroyWindow(mWindow);
    mWindow = nullptr;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

// Main loop of the entire program. Gets the current state and simply runs it
void Game::run ()
{
    if (!mStates.empty())
    {
        mStates.back()->run();
    }
}

// Deletes the current state
void Game::popState ()
{
    mStates.pop_back();
}

// Pushes a new state to the front
void Game::pushState (State *state)
{
    mStates.push_back(state);
}

void Game::pushSpeedChallenge()
{
    delete Game::getInstance()->mSpeedChallengeState;
    Game::getInstance()->mSpeedChallengeState = new SpeedChallengeState(Game::getInstance()->mManager);
    Game::getInstance()->mSpeedChallengeState->initialize();
    Game::getInstance()->pushState(Game::getInstance()->mSpeedChallengeState);
}

// Deletes the current state and replaces it with a different one
void Game::changeState (State *state)
{
    popState();
    pushState(state);
}

// Pushes a new gamestate state to the front
void Game::pushNewGame ()
{
    delete Game::getInstance()->mPlayState;
    Game::getInstance()->mPlayState = new GameState(Game::getInstance()->mManager);
    Game::getInstance()->mPlayState->initialize();
    Game::getInstance()->pushState(Game::getInstance()->mPlayState);
}

// Pushes the lobby to the front
void Game::pushLobby ()
{
    delete Game::getInstance()->mLobbyState;
    Game::getInstance()->mLobbyState = new LobbyState(Game::getInstance()->mManager);
    Game::getInstance()->mLobbyState->initialize();
    Game::getInstance()->pushState(Game::getInstance()->mLobbyState);
}

// Pushes the options to the front
void Game::pushOptions ()
{
    delete Game::getInstance()->mOptionsState;
    Game::getInstance()->mOptionsState = new OptionsState(Game::getInstance()->mManager);
    Game::getInstance()->mOptionsState->initialize();
    Game::getInstance()->pushState(Game::getInstance()->mOptionsState);
}

// Pushes the pause menu to the front
void Game::pushPaused ()
{
    delete Game::getInstance()->mPausedState;
    Game::getInstance()->mPausedState = new PausedState (Game::getInstance()->mManager);
    Game::getInstance()->mPausedState->initialize();
    Game::getInstance()->pushState(Game::getInstance()->mPausedState);
}

// Pushes the mode select to the front ← 추가된 부분
void Game::pushModeSelect ()
{
    delete Game::getInstance()->mModeSelectState;
    Game::getInstance()->mModeSelectState = new ModeSelectState(Game::getInstance()->mManager);
    Game::getInstance()->mModeSelectState->initialize();
    Game::getInstance()->pushState(Game::getInstance()->mModeSelectState);
}
void Game::pushChallengeMenu()
{
    delete Game::getInstance()->mChallengeMenuState;
    Game::getInstance()->mChallengeMenuState = new ChallengeMenuState(Game::getInstance()->mManager);
    Game::getInstance()->mChallengeMenuState->initialize();
    Game::getInstance()->pushState(Game::getInstance()->mChallengeMenuState);
}

// Goes back one state (by popping the state in the front)
void Game::goBack ()
{
    Game::getInstance()->popState();
}

// Pops the first 2 states
void Game::goDoubleBack ()
{
    Game::getInstance()->popState();
    Game::getInstance()->popState();
}

bool Game::isGameExiting ()
{
    if (mStates.empty())
    {
        return true;
    }
    else
    {
        return mStates.back()->nextStateID == STATE_EXIT;
    }
}

Game *Game::mInstance = 0;

Game::Game () {
    mChallengeMenuState = nullptr;
    mSpeedChallengeState = nullptr; 
}
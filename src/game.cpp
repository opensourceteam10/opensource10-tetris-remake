#include "game.hpp"
#include "menustate.hpp"
#include "optionsstate.hpp"
#include "pausedstate.hpp"
#include "gamestate.hpp"
#include "SpeedChallengeState.hpp"
#include "ModeSelectState.hpp"
#include "MultiState.hpp"
#include "ChallengeMenuState.hpp"
#include "mainmenustate.hpp"

Game* Game::mInstance = nullptr;

Game::Game() {
    mRenderer = nullptr;
    mWindow = nullptr;
    mManager = new InputManager();
}

Game* Game::getInstance() {
    if (!mInstance)
        mInstance = new Game();
    return mInstance;
}

bool Game::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
        return false;

    int actual_width = config::logical_window_width * config::resolution_scaling;
    int actual_height = config::logical_window_height * config::resolution_scaling;

    mWindow = SDL_CreateWindow("PixelTetris",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        actual_width, actual_height, SDL_WINDOW_SHOWN);

    if (!mWindow) return false;

    mRenderer = new Renderer();
    mRenderer->initialize(mWindow);

    if (TTF_Init() == -1) return false;

    pushNewState<MainMenuState>();
    return true;
}

void Game::run() {
    if (mPendingDeleteState) {
        delete mPendingDeleteState;
        mPendingDeleteState = nullptr;
    }

    if (!mStates.empty()) {
        mStates.back()->run();
    }
}

void Game::exit() {
    for (State* s : mStates)
        delete s;
    mStates.clear();

    if (mPendingDeleteState) {
        delete mPendingDeleteState;
        mPendingDeleteState = nullptr;
    }

    delete mRenderer;
    delete mManager;
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
    TTF_Quit();
}

void Game::popState() {
    if (!mStates.empty()) {
        mPendingDeleteState = mStates.back();
        mStates.pop_back();
    }
}

void Game::pushState(State* s) {
    mStates.push_back(s);
}

void Game::changeState(State* s) {
    if (!mStates.empty())
        popState();
    pushState(s);
}

bool Game::isGameExiting() {
    return mStates.empty() || mStates.back()->nextStateID == STATE_EXIT;
}


   

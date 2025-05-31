#include "game.hpp"

#include "menustate.hpp"
#include "optionsstate.hpp"
#include "pausedstate.hpp"
#include "gamestate.hpp"

#include "SpeedChallengeState.hpp"
#include "ModeSelectState.hpp"
#include "MultiState.hpp"
#include "ChallengeMenuState.hpp"

Game *Game::mInstance = nullptr;

Game::Game()
{
    mRenderer = nullptr;
    mWindow = nullptr;
    mManager = new InputManager();
    mChallengeMenuState = nullptr;
    mSpeedChallengeState = nullptr;
    mPlayState = nullptr;
    mMainMenuState = nullptr;
    mOptionsState = nullptr;
    mPausedState = nullptr;
    
    mModeSelectState = nullptr;
    mMultiState = nullptr;
}

Game *Game::getInstance()
{
    if (mInstance == nullptr)
        mInstance = new Game();

    return mInstance;
}

bool Game::initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
        return false;

    // 논리적 해상도 * 스케일링 팩터로 실제 윈도우 크기 계산
    int actual_width = config::logical_window_width * config::resolution_scaling;
    int actual_height = config::logical_window_height * config::resolution_scaling;

    mWindow = SDL_CreateWindow("PixelTetris", 
                               SDL_WINDOWPOS_CENTERED, 
                               SDL_WINDOWPOS_CENTERED,
                               actual_width, 
                               actual_height, 
                               SDL_WINDOW_SHOWN);
    
    if (mWindow == nullptr)
        return false;

    if (!mRenderer)
        mRenderer = new Renderer();
    
    mRenderer->initialize(mWindow);

    if (TTF_Init() == -1)
        return false;

    // 메인 메뉴 상태로 시작
    mMainMenuState = new MenuState(mManager);
    mMainMenuState->initialize();
    pushState(mMainMenuState);

    return true;
}

void Game::exit()
{
    delete mRenderer;
    delete mManager;

    for (State *s : mStates)
        delete s;

    if (mChallengeMenuState) delete mChallengeMenuState;
    if (mSpeedChallengeState) delete mSpeedChallengeState;
    if (mPlayState) delete mPlayState;
    if (mMainMenuState) delete mMainMenuState;
    if (mOptionsState) delete mOptionsState;
    if (mPausedState) delete mPausedState;
    
    if (mModeSelectState) delete mModeSelectState;
    if (mMultiState) delete mMultiState;

    SDL_DestroyWindow(mWindow);
    SDL_Quit();
    TTF_Quit();
}
void Game::run()
{
    if (!mStates.empty())
    {
        mStates.back()->run();
    }
}




void Game::popState()
{
    if (!mStates.empty())
    {
                mStates.pop_back();
    }
}

void Game::pushState(State *s)
{
    mStates.push_back(s);
}

void Game::changeState(State *s)
{
    if (!mStates.empty())
        popState();
    pushState(s);
}

void Game::pushOptions()
{
    if (Game::getInstance()->mOptionsState)
        delete Game::getInstance()->mOptionsState;
    Game::getInstance()->mOptionsState = new OptionsState(Game::getInstance()->mManager);
    Game::getInstance()->mOptionsState->initialize();
    Game::getInstance()->pushState(Game::getInstance()->mOptionsState);
}

void Game::pushPaused()
{
    if (Game::getInstance()->mPausedState)
        delete Game::getInstance()->mPausedState;
    Game::getInstance()->mPausedState = new PausedState(Game::getInstance()->mManager);
    Game::getInstance()->mPausedState->initialize();
    Game::getInstance()->pushState(Game::getInstance()->mPausedState);
}

void Game::pushNewGame()
{
    if (Game::getInstance()->mPlayState)
        delete Game::getInstance()->mPlayState;
    Game::getInstance()->mPlayState = new GameState(Game::getInstance()->mManager);
    Game::getInstance()->mPlayState->initialize();
    Game::getInstance()->pushState(Game::getInstance()->mPlayState);
}

void Game::pushSpeedChallenge()
{
    if (Game::getInstance()->mSpeedChallengeState)
        delete Game::getInstance()->mSpeedChallengeState;
    Game::getInstance()->mSpeedChallengeState = new SpeedChallengeState(Game::getInstance()->mManager);
    Game::getInstance()->mSpeedChallengeState->initialize();
    Game::getInstance()->pushState(Game::getInstance()->mSpeedChallengeState);
}

void Game::pushModeSelect()
{
    if (Game::getInstance()->mModeSelectState)
        delete Game::getInstance()->mModeSelectState;
    Game::getInstance()->mModeSelectState = new ModeSelectState(Game::getInstance()->mManager);
    Game::getInstance()->mModeSelectState->initialize();
    Game::getInstance()->pushState(Game::getInstance()->mModeSelectState);
}

void Game::pushChallengeMenu()
{
    if (Game::getInstance()->mChallengeMenuState)
        delete Game::getInstance()->mChallengeMenuState;
    Game::getInstance()->mChallengeMenuState = new ChallengeMenuState(Game::getInstance()->mManager);
    Game::getInstance()->mChallengeMenuState->initialize();
    Game::getInstance()->pushState(Game::getInstance()->mChallengeMenuState);
}
/*
void Game::pushLobby()
{
    if (Game::getInstance()->mLobbyState)
        delete Game::getInstance()->mLobbyState;
    Game::getInstance()->mLobbyState = new LobbyState(Game::getInstance()->mManager);
    Game::getInstance()->mLobbyState->initialize();
    Game::getInstance()->pushState(Game::getInstance()->mLobbyState);
}
*/
void Game::pushMulti()
{
    Game* game = Game::getInstance();
    if (game->mMultiState)
        delete game->mMultiState;
    game->mMultiState = new MultiState(game->mManager);
    game->mMultiState->initialize();
    game->pushState(game->mMultiState);
}

void Game::goBack()
{
    Game::getInstance()->popState();
}

void Game::goDoubleBack()
{
    Game::getInstance()->popState();
    Game::getInstance()->popState();
}

bool Game::isGameExiting()
{
    return mManager->isGameExiting();
}
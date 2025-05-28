#include "game.hpp"

#include "MenuState.hpp"
#include "OptionsState.hpp"
#include "PausedState.hpp"
#include "LobbyState.hpp"
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
    mLobbyState = nullptr;
    mModeSelectState = nullptr;
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

    mWindow = SDL_CreateWindow("PixelTetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               640, 720, SDL_WINDOW_SHOWN);
    if (mWindow == nullptr)
        return false;

    if (!mRenderer)
        mRenderer = new Renderer(mWindow);

    if (TTF_Init() == -1)
        return false;

    return true;
}

void Game::exit()
{
    delete mRenderer;
    delete mManager;

    for (State *s : mStates)
        delete s;

    SDL_DestroyWindow(mWindow);
    SDL_Quit();
    TTF_Quit();
}

void Game::run()
{
    while (!isGameExiting())
    {
        mManager->update();
        mStates.back()->run();
    }
}

void Game::popState()
{
    delete mStates.back();
    mStates.pop_back();
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
    delete Game::getInstance()->mOptionsState;
    Game::getInstance()->mOptionsState = new OptionsState(Game::getInstance()->mManager);
    Game::getInstance()->mOptionsState->initialize();
    Game::getInstance()->pushState(Game::getInstance()->mOptionsState);
}

void Game::pushPaused()
{
    delete Game::getInstance()->mPausedState;
    Game::getInstance()->mPausedState = new PausedState(Game::getInstance()->mManager);
    Game::getInstance()->mPausedState->initialize();
    Game::getInstance()->pushState(Game::getInstance()->mPausedState);
}

void Game::pushNewGame()
{
    delete Game::getInstance()->mPlayState;
    Game::getInstance()->mPlayState = new GameState(Game::getInstance()->mManager);
    Game::getInstance()->mPlayState->initialize();
    Game::getInstance()->pushState(Game::getInstance()->mPlayState);
}

void Game::pushSpeedChallenge()
{
    delete Game::getInstance()->mSpeedChallengeState;
    Game::getInstance()->mSpeedChallengeState = new SpeedChallengeState(Game::getInstance()->mManager);
    Game::getInstance()->mSpeedChallengeState->initialize();
    Game::getInstance()->pushState(Game::getInstance()->mSpeedChallengeState);
}

void Game::pushModeSelect()
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

void Game::pushLobby()
{
    delete Game::getInstance()->mLobbyState;
    Game::getInstance()->mLobbyState = new LobbyState(Game::getInstance()->mManager);
    Game::getInstance()->mLobbyState->initialize();
    Game::getInstance()->pushState(Game::getInstance()->mLobbyState);
}

void Game::pushMulti()
{
    Game* game = Game::getInstance(); // 싱글턴 인스턴스 얻기
    game->pushState(new MultiState(game->mManager));
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
    return mManager->hasQuit();
}

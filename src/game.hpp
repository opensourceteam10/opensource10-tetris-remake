#ifndef GAME_HPP
#define GAME_HPP

#include <vector>

#include <SDL.h>
#include <SDL_ttf.h>

#include "inputmanager.hpp"
#include "renderer.hpp"
#include "state.hpp"

class SpeedChallengeState;
class ChallengeMenuState;
class State;
class GameState;
class MenuState;
class OptionsState;
class PausedState;
class LobbyState;
class ModeSelectState;
class MultiState;

class Game
{
public:
    friend class OptionsState;
    static Game* getInstance();

    bool initialize ();
    void exit ();
    void run ();
    
    void popState ();
    void pushState (State *s);
    void changeState (State *s);

    static void pushChallengeMenu();
    static void pushOptions();
    static void pushNewGame();
    static void pushPaused();
    static void pushLobby();
    static void pushModeSelect();
    static void pushSpeedChallenge();
    static void pushMulti();

    static void goBack();
    static void goDoubleBack();

    bool isGameExiting();

    Renderer *mRenderer;
    SDL_Window *mWindow;  // public으로 이동 (OptionsState에서 접근 필요)

private:
    static Game *mInstance;
    Game();
    InputManager *mManager;
    std::vector<State*> mStates;

    ChallengeMenuState *mChallengeMenuState;
    SpeedChallengeState *mSpeedChallengeState;
    GameState *mPlayState;
    MenuState *mMainMenuState;
    OptionsState *mOptionsState;
    PausedState *mPausedState;
    LobbyState *mLobbyState;
    ModeSelectState *mModeSelectState;
    MultiState *mMultiState;
};

#endif // GAME_HPP
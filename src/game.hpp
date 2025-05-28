#ifndef GAME_HPP
#define GAME_HPP

#include <vector>

#include <SDL.h>
#include <SDL_ttf.h>

#include "inputmanager.hpp"
#include "renderer.hpp"
#include "state.hpp"

class SpeedChallengeState;     // 전방 선언
class ChallengeMenuState;      // 전방 선언
class ChallengeMenuState;
class State;
class SpeedChallengeState;
class GameState;
class MenuState;
class OptionsState;
class PausedState;
<<<<<<< HEAD
class MultiState;
=======
class LobbyState;
class ModeSelectState;  // ← 추가된 부분
>>>>>>> feature/challenge-mode

// Utilizes the "Singleton pattern" to ensure there can only be one game
class Game
{
public:
    friend class OptionsState;          // Options can change the window size            
    static Game* getInstance();

    bool initialize ();
    void exit ();
    void run ();
    
    void popState ();
    void pushState (State *s);
    void changeState (State *s);

    void pushChallengeMenu();
    static void pushOptions();
    static void pushNewGame();
    static void pushPaused();
<<<<<<< HEAD
    static void pushMulti();
=======
    static void pushLobby();
    static void pushModeSelect();  // ← 추가된 부분
>>>>>>> feature/challenge-mode
    static void goBack();
    static void goDoubleBack();
    static void pushSpeedChallenge();  

    bool isGameExiting();

    Renderer *mRenderer;                // The renderer used for all things rendering
    
private:
    static Game *mInstance;
    Game();
    SDL_Window *mWindow;
    InputManager *mManager;
    std::vector<State*> mStates;

    ChallengeMenuState *mChallengeMenuState;
    SpeedChallengeState *mSpeedChallengeState;
    GameState *mPlayState;
    MenuState *mMainMenuState;
    OptionsState *mOptionsState;
    PausedState *mPausedState;
<<<<<<< HEAD
    MultiState *mMultiState;
=======
    LobbyState *mLobbyState;
    ModeSelectState *mModeSelectState;  // ← 추가된 부분
>>>>>>> feature/challenge-mode
};

#endif // GAME_HPP
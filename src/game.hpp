#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include "inputmanager.hpp"
#include "renderer.hpp"
#include "state.hpp"
#include "state_factory.hpp"

// 상태 클래스 전방 선언
class SpeedChallengeState;
class ChallengeMenuState;
class GameState;
class MenuState;
class OptionsState;
class PausedState;
class LobbyState;
class ModeSelectState;
class MultiState;

class Game {
public:
    static Game* getInstance();

    bool initialize();
    void exit();
    void run();

    void popState();
    void pushState(State* s);
    void changeState(State* s);

    template <typename T>
    void pushNewState() {
        State* state = StateFactory::create<T>(mManager);
        state->initialize(); 
        pushState(state);
    }

    bool isGameExiting();

    Renderer* mRenderer;
    SDL_Window* mWindow;

    InputManager* getInputManager() const { return mManager; }

private:
    static Game* mInstance;
    Game();

    InputManager* mManager;
    std::vector<State*> mStates;

    State* mPendingDeleteState = nullptr;

    // 선택적 저장 포인터들 (삭제 예정이거나 추후 공유 가능)
    ChallengeMenuState* mChallengeMenuState = nullptr;
    SpeedChallengeState* mSpeedChallengeState = nullptr;
    GameState* mPlayState = nullptr;
    MenuState* mMainMenuState = nullptr;
    OptionsState* mOptionsState = nullptr;
    PausedState* mPausedState = nullptr;
    LobbyState* mLobbyState = nullptr;
    ModeSelectState* mModeSelectState = nullptr;
    MultiState* mMultiState = nullptr;
};

#endif // GAME_HPP

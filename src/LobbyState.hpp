#ifndef LOBBYSTATE_HPP
#define LOBBYSTATE_HPP

#include "state.hpp"
#include "button.hpp"
#include "texture.hpp"
#include <vector>
#include <string>

enum class LobbyFocus {
    ROOM_LIST,
    ROOM_CREATE,
    BACK_BTN
};

class LobbyState : public State {
public:
    LobbyState(InputManager *manager);
    ~LobbyState();

    void initialize() override;
    void exit() override;
    void run() override;
    void update() override;
    void draw() override;

private:
    std::vector<std::string> roomList;
    int selectedRoomIndex;
    LobbyFocus focus;

    Button* backButton;
    Button* roomCreateButton;
    Texture* titleTexture;

    // 방 생성 입력 모드
    bool inRoomCreateInput;
    std::string roomNameInput;

    void joinRoom(int index);
    void openRoomCreate();
    void handleRoomCreateInput(const SDL_Event& event);
};

#endif


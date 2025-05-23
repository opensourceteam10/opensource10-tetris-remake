#include "LobbyState.hpp"
#include "game.hpp"
#include "config.hpp"
#include <SDL.h>

LobbyState::LobbyState(InputManager *manager)
: State(manager), selectedRoomIndex(0), focus(LobbyFocus::ROOM_LIST),
  backButton(nullptr), roomCreateButton(nullptr), titleTexture(nullptr),
  inRoomCreateInput(false) {}

LobbyState::~LobbyState() { exit(); }

void LobbyState::initialize() {
    roomList = {"Room 1", "Room 2", "Room 3"};
    selectedRoomIndex = 0;
    focus = LobbyFocus::ROOM_LIST;
    inRoomCreateInput = false;
    roomNameInput.clear();

    if (titleTexture) delete titleTexture;
    titleTexture = new Texture();
    titleTexture->loadFromText("Multiplay Lobby", Game::getInstance()->mRenderer->bigFont, config::default_text_color);

    if (backButton) delete backButton;
    backButton = new Button("Back", &Game::goBack, 40, 40, 120, 40);

    if (roomCreateButton) delete roomCreateButton;
    // 오른쪽 위에 Room Create 버튼
    roomCreateButton = new Button("Room Create", nullptr, config::logical_window_width - 200, 120, 160, 40);
}

void LobbyState::exit() {
    if (titleTexture) { delete titleTexture; titleTexture = nullptr; }
    if (backButton) { delete backButton; backButton = nullptr; }
    if (roomCreateButton) { delete roomCreateButton; roomCreateButton = nullptr; }
}

void LobbyState::run() {
    update();
    draw();
}

void LobbyState::update() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            Game::getInstance()->exit();
            return;
        }
        if (inRoomCreateInput) {
            handleRoomCreateInput(event);
            return;
        }
        if (event.type == SDL_KEYDOWN) {
            switch (focus) {
                case LobbyFocus::ROOM_LIST:
                    if (event.key.keysym.sym == SDLK_UP && selectedRoomIndex > 0) {
                        selectedRoomIndex--;
                    } else if (event.key.keysym.sym == SDLK_DOWN && selectedRoomIndex < (int)roomList.size() - 1) {
                        selectedRoomIndex++;
                    } else if (event.key.keysym.sym == SDLK_LEFT) {
                        focus = LobbyFocus::BACK_BTN;
                    } else if (event.key.keysym.sym == SDLK_RIGHT) {
                        focus = LobbyFocus::ROOM_CREATE;
                    } else if (event.key.keysym.sym == SDLK_RETURN && selectedRoomIndex >= 0) {
                        joinRoom(selectedRoomIndex);
                    } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        if (backButton) backButton->callbackFunction();
                    }
                    break;
                case LobbyFocus::BACK_BTN:
                    if (event.key.keysym.sym == SDLK_RIGHT) {
                        focus = LobbyFocus::ROOM_LIST;
                    } else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_ESCAPE) {
                        if (backButton) backButton->callbackFunction();
                    }
                    break;
                case LobbyFocus::ROOM_CREATE:
                    if (event.key.keysym.sym == SDLK_LEFT) {
                        focus = LobbyFocus::ROOM_LIST;
                    } else if (event.key.keysym.sym == SDLK_RETURN) {
                        openRoomCreate();
                    } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        focus = LobbyFocus::ROOM_LIST;
                    }
                    break;
            }
        }
    }
}

void LobbyState::draw() {
    if (!mRenderer) return;
    mRenderer->clearScreen();

    // 타이틀
    if (titleTexture)
        titleTexture->renderCentered(config::logical_window_width / 2, 80);

    // Room Create 버튼 (오른쪽 위)
    if (roomCreateButton) {
        if (focus == LobbyFocus::ROOM_CREATE) {
            SDL_SetRenderDrawColor(mRenderer->mSDLRenderer, 200, 200, 100, 80);
            SDL_Rect highlight = {config::logical_window_width - 200, 120, 160, 40};
            SDL_RenderFillRect(mRenderer->mSDLRenderer, &highlight);
        }
        roomCreateButton->draw();
    }

    // 방 목록
    int base_y = 200;
    for (int i = 0; i < (int)roomList.size(); ++i) {
        Texture temp;
        temp.loadFromText(roomList[i], Game::getInstance()->mRenderer->mediumFont, config::default_text_color);
        temp.renderCentered(config::logical_window_width / 2, base_y + i * 60);
        if (focus == LobbyFocus::ROOM_LIST && i == selectedRoomIndex) {
            SDL_Rect highlight = {
                config::logical_window_width / 2 - 100, base_y + i * 60 - 25,
                200, 50
            };
            SDL_SetRenderDrawBlendMode(mRenderer->mSDLRenderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(mRenderer->mSDLRenderer, 0, 150, 255, 80);
            SDL_RenderFillRect(mRenderer->mSDLRenderer, &highlight);
            SDL_SetRenderDrawBlendMode(mRenderer->mSDLRenderer, SDL_BLENDMODE_NONE);
        }
    }

    // Back 버튼 (좌상단)
    if (backButton) {
        if (focus == LobbyFocus::BACK_BTN) {
            SDL_SetRenderDrawColor(mRenderer->mSDLRenderer, 200, 200, 100, 80);
            SDL_Rect highlight = {40, 40, 120, 40};
            SDL_RenderFillRect(mRenderer->mSDLRenderer, &highlight);
        }
        backButton->draw();
    }

    // 방 생성 입력창
    if (inRoomCreateInput) {
        SDL_Rect inputRect = {config::logical_window_width / 2 - 120, 140, 240, 40};
        SDL_SetRenderDrawColor(mRenderer->mSDLRenderer, 255, 255, 255, 255);
        SDL_RenderFillRect(mRenderer->mSDLRenderer, &inputRect);

        Texture inputText;
        inputText.loadFromText(roomNameInput, Game::getInstance()->mRenderer->mediumFont, {0,0,0,255});
        inputText.renderCentered(config::logical_window_width / 2, 160);

        Texture label;
        label.loadFromText("Room name:", Game::getInstance()->mRenderer->mediumFont, {100,100,100,255});
        label.renderCentered(config::logical_window_width / 2, 120);
    }

    mRenderer->updateScreen();
}

void LobbyState::joinRoom(int index) {
    // 실제로는 Game::getInstance()->pushRoom() 등으로 방 입장 상태로 전환
}

void LobbyState::openRoomCreate() {
    inRoomCreateInput = true;
    roomNameInput.clear();
    SDL_StartTextInput();
}

void LobbyState::handleRoomCreateInput(const SDL_Event& event) {
    if (event.type == SDL_TEXTINPUT) {
        roomNameInput += event.text.text;
    }
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_BACKSPACE && !roomNameInput.empty()) {
            roomNameInput.pop_back();
        }
        if (event.key.keysym.sym == SDLK_RETURN && !roomNameInput.empty()) {
            // 방 생성 후 목록에 추가
            roomList.push_back(roomNameInput);
            inRoomCreateInput = false;
            SDL_StopTextInput();
        }
        if (event.key.keysym.sym == SDLK_ESCAPE) {
            inRoomCreateInput = false;
            SDL_StopTextInput();
        }
    }
}


#include "MultiState.hpp"
#include "game.hpp"

MultiState::MultiState(InputManager* manager) : GameState(manager), showResult(false), winnerIdx(-1) {
    for (int i = 0; i < 2; ++i) {
        boards[i] = new Board();
        currentPieces[i] = Piece(0, 0);
        nextPieces[i] = Piece(0, 0);
        scores[i] = 0;
        isGameOver[i] = false;

        tetrominoSprites[i] = new Texture();
        playfieldFrame[i] = new Texture();

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        tetrominoSprites[i]->loadFromImage("../../assets/tetrominoSprites.png");
        playfieldFrame[i]->loadFromImage("../../assets/playfieldFrame.png");
#else
        tetrominoSprites[i]->loadFromImage("../assets/tetrominoSprites.png");
        playfieldFrame[i]->loadFromImage("../assets/playfieldFrame.png");
#endif

        for (int j = 0; j < 7; ++j) {
            tetrominoSpriteClips[i][j].x = 16 * j;
            tetrominoSpriteClips[i][j].y = 0;
            tetrominoSpriteClips[i][j].w = 16;
            tetrominoSpriteClips[i][j].h = 16;
        }
        for (int j = 0; j < 4; ++j) {
            playfieldFrameClips[i][j].x = config::frame_sprite_size * j;
            playfieldFrameClips[i][j].y = 0;
            playfieldFrameClips[i][j].w = config::frame_sprite_size;
            playfieldFrameClips[i][j].h = config::frame_sprite_size;
        }
        lastDropTime[i] = SDL_GetTicks();
    }
    winText = new Texture();
    retryButton = new Texture();
    backButton = new Texture();
}

MultiState::~MultiState() { exit(); }

void MultiState::initialize() {
    showResult = false;
    winnerIdx = -1;
    for (int i = 0; i < 2; ++i) {
        if (boards[i]) delete boards[i];
        boards[i] = new Board();
        scores[i] = 0;
        isGameOver[i] = false;
        nextPieces[i] = Piece(getRandom(0, 6), 0);
        spawnNewPiece(i);
        lastDropTime[i] = SDL_GetTicks();
    }
}

void MultiState::spawnNewPiece(int playerIdx) {
    currentPieces[playerIdx] = nextPieces[playerIdx];
    // 조각 스폰 위치를 HIDDEN_ROWS로 올림
    currentPieces[playerIdx].r = currentPieces[playerIdx].getInitialOffsetR() + HIDDEN_ROWS;
    currentPieces[playerIdx].c = (config::playfield_width / 2) + currentPieces[playerIdx].getInitialOffsetC();
    nextPieces[playerIdx] = Piece(getRandom(0, 6), 0);

    // 게임오버 판정: 스폰된 조각이 움직일 수 없을 때만
    if (!boards[playerIdx]->isPositionLegal(currentPieces[playerIdx])) {
        isGameOver[playerIdx] = true;
        if (!showResult) {
            winnerIdx = 1 - playerIdx;
            showResult = true;
            winText->loadFromText(
                winnerIdx == 0 ? "PLAYER 1 WINS!" : "PLAYER 2 WINS!",
                Game::getInstance()->mRenderer->mediumFont,
                {0, 128, 0, 255}
            );
            retryButton->loadFromText("Retry", Game::getInstance()->mRenderer->mediumFont, {0,0,0,255});
            backButton->loadFromText("Back to Menu", Game::getInstance()->mRenderer->mediumFont, {0,0,0,255});
        }
    }
}

void MultiState::run() {
    bool quit = false;
    while (!quit) {
        SDL_Event event;
        Action actionP1 = Action::stay_idle;
        Action actionP2 = Action::stay_idle;
        int mouseX = 0, mouseY = 0;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) return;
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    currentPhase = GAME_STARTED;
                    game_just_started = true;
                    Game::getInstance()->pushPaused();
                    return;
                }
                // Player 1: WASD
                switch (event.key.keysym.sym) {
                    case SDLK_a: actionP1 = Action::move_left; break;
                    case SDLK_d: actionP1 = Action::move_right; break;
                    case SDLK_s: actionP1 = Action::move_down; break;
                    case SDLK_w: actionP1 = Action::rotate; break;
                    case SDLK_LCTRL: actionP1 = Action::drop; break;
                }
                // Player 2: 방향키
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT: actionP2 = Action::move_left; break;
                    case SDLK_RIGHT: actionP2 = Action::move_right; break;
                    case SDLK_DOWN: actionP2 = Action::move_down; break;
                    case SDLK_UP: actionP2 = Action::rotate; break;
                    case SDLK_RCTRL: actionP2 = Action::drop; break;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && showResult) {
                SDL_GetMouseState(&mouseX, &mouseY);
                if (isMouseOver(320, 350, 120, 40, mouseX, mouseY)) initialize();
                if (isMouseOver(320, 400, 220, 40, mouseX, mouseY)) {
                    Game::getInstance()->popState();
                    return;
                }
            }
        }
        if (!showResult) {
            if (actionP1 != Action::stay_idle) handleEvent(0, actionP1);
            if (actionP2 != Action::stay_idle) handleEvent(1, actionP2);
            update();
        }
        draw();
        SDL_Delay(16);
    }
}

void MultiState::handleEvent(int playerIdx, Action action) {
    if (isGameOver[playerIdx]) return;
    switch (action) {
        case Action::move_left:
            currentPieces[playerIdx].c--;
            if (!boards[playerIdx]->isPositionLegal(currentPieces[playerIdx]))
                currentPieces[playerIdx].c++;
            break;
        case Action::move_right:
            currentPieces[playerIdx].c++;
            if (!boards[playerIdx]->isPositionLegal(currentPieces[playerIdx]))
                currentPieces[playerIdx].c--;
            break;
        case Action::move_down:
            currentPieces[playerIdx].r++;
            if (!boards[playerIdx]->isPositionLegal(currentPieces[playerIdx]))
                currentPieces[playerIdx].r--;
            else
                checkLineClear(playerIdx);
            break;
        case Action::rotate:
            currentPieces[playerIdx].rotation = (currentPieces[playerIdx].rotation + 1) % 4;
            if (!boards[playerIdx]->isPositionLegal(currentPieces[playerIdx]))
                currentPieces[playerIdx].rotation = (currentPieces[playerIdx].rotation + 3) % 4;
            break;
        case Action::drop:
            while (boards[playerIdx]->isPositionLegal(currentPieces[playerIdx]))
                currentPieces[playerIdx].r++;
            currentPieces[playerIdx].r--;
            boards[playerIdx]->storePiece(currentPieces[playerIdx]);
            checkLineClear(playerIdx);
            spawnNewPiece(playerIdx);
            break;
        default: break;
    }
}

void MultiState::update() {
    Uint32 now = SDL_GetTicks();
    for (int i = 0; i < 2; ++i) {
        if (isGameOver[i] || showResult) continue;
        if (now - lastDropTime[i] >= dropInterval) {
            Piece test = currentPieces[i];
            test.r++;
            if (boards[i]->isPositionLegal(test)) {
                currentPieces[i].r++;
            } else {
                boards[i]->storePiece(currentPieces[i]);
                checkLineClear(i);
                spawnNewPiece(i);
            }
            lastDropTime[i] = now;
        }
    }
}

void MultiState::checkLineClear(int playerIdx) {
    int linesCleared = boards[playerIdx]->clearFullLines();
    if (linesCleared > 0) {
        scores[playerIdx] += linesCleared * 100;
    }
}

void MultiState::draw() {
    Renderer* renderer = Game::getInstance()->mRenderer;
    renderer->setBackgroundColor(255,255,255);
    renderer->clearScreen();

    // 1P
    drawBoard(0, PLAYER1_OFFSET_X, BOARD_OFFSET_Y);
    drawCurrentPiece(0, PLAYER1_OFFSET_X, BOARD_OFFSET_Y);

    // 2P
    drawBoard(1, PLAYER2_OFFSET_X, BOARD_OFFSET_Y);
    drawCurrentPiece(1, PLAYER2_OFFSET_X, BOARD_OFFSET_Y);

    if (showResult) drawResultScreen();

    renderer->updateScreen();
}

void MultiState::drawBoard(int playerIdx, int offsetX, int offsetY) {
    // 플레이어 이름 및 점수 표시
    std::string playerText = "Player " + std::to_string(playerIdx+1) + ": " + std::to_string(scores[playerIdx]);
    Texture playerScoreTexture;
    playerScoreTexture.loadFromText(
        playerText,
        Game::getInstance()->mRenderer->mediumFont,
        {0, 0, 0, 255}
    );
    const int textX = offsetX + (config::block_size * config::playfield_width)/2 - playerScoreTexture.getWidth()/2;
    const int textY = offsetY - 40;
    playerScoreTexture.render(textX, textY);

    // 좌우 프레임
    for (int i = 0; i < 2 * config::true_playfield_height; i++) {
        playfieldFrame[playerIdx]->render(
            offsetX - config::frame_sprite_size,
            offsetY + i * config::frame_sprite_size,
            &playfieldFrameClips[playerIdx][0]
        );
        playfieldFrame[playerIdx]->render(
            offsetX + config::block_size * config::playfield_width - (config::frame_sprite_size - config::frame_width),
            offsetY + i * config::frame_sprite_size,
            &playfieldFrameClips[playerIdx][0]
        );
    }

    // 바닥 프레임 (HIDDEN_ROWS만큼 위로)
    int bottomY = offsetY + config::block_size * (config::playfield_height - HIDDEN_ROWS);
    int frameRepeat = (config::block_size * config::playfield_width) / config::frame_sprite_size;
    for (int i = 0; i < frameRepeat; i++) {
        playfieldFrame[playerIdx]->render(
            offsetX + i * config::frame_sprite_size,
            bottomY,
            &playfieldFrameClips[playerIdx][1]
        );
    }
    playfieldFrame[playerIdx]->render(
        offsetX - config::frame_sprite_size,
        bottomY - (config::frame_sprite_size - config::frame_width),
        &playfieldFrameClips[playerIdx][2]
    );
    playfieldFrame[playerIdx]->render(
        offsetX + config::block_size * config::playfield_width,
        bottomY - (config::frame_sprite_size - config::frame_width),
        &playfieldFrameClips[playerIdx][3]
    );

    // 블록 그리기 (HIDDEN_ROWS부터만 그림)
    for (int row = HIDDEN_ROWS; row < config::playfield_height; row++) {
        for (int col = 0; col < config::playfield_width; col++) {
            if (!boards[playerIdx]->isBlockFree(row, col)) {
                int type = boards[playerIdx]->getTetromino(row, col);
                tetrominoSprites[playerIdx]->render(
                    offsetX + col * config::block_size,
                    offsetY + (row - HIDDEN_ROWS) * config::block_size,
                    &tetrominoSpriteClips[playerIdx][type]
                );
            }
        }
    }
}

void MultiState::drawCurrentPiece(int playerIdx, int offsetX, int offsetY) {
    Piece& p = currentPieces[playerIdx];
    for (int dr = 0; dr < config::matrix_blocks; ++dr) {
        for (int dc = 0; dc < config::matrix_blocks; ++dc) {
            if (p.getBlockType(dr, dc) != 0) {
                int r = p.r + dr;
                int c = p.c + dc;
                if (r >= HIDDEN_ROWS && r < config::playfield_height && c >= 0 && c < config::playfield_width) {
                    tetrominoSprites[playerIdx]->render(
                        offsetX + c * config::block_size,
                        offsetY + (r - HIDDEN_ROWS) * config::block_size,
                        &tetrominoSpriteClips[playerIdx][p.piece_type]
                    );
                }
            }
        }
    }
}

void MultiState::drawResultScreen() {
    Renderer* renderer = Game::getInstance()->mRenderer;
    winText->render(config::logical_window_width/2 - winText->getWidth()/2, 200);
    retryButton->render(320, 350);
    backButton->render(320, 400);
}

bool MultiState::isMouseOver(int x, int y, int w, int h, int mx, int my) {
    return (mx >= x && mx <= x+w && my >= y && my <= y+h);
}

void MultiState::exit() {
    for (int i = 0; i < 2; ++i) {
        if (boards[i]) { delete boards[i]; boards[i] = nullptr; }
        if (tetrominoSprites[i]) { delete tetrominoSprites[i]; tetrominoSprites[i] = nullptr; }
        if (playfieldFrame[i]) { delete playfieldFrame[i]; playfieldFrame[i] = nullptr; }
    }
    if (winText) { delete winText; winText = nullptr; }
    if (retryButton) { delete retryButton; retryButton = nullptr; }
    if (backButton) { delete backButton; backButton = nullptr; }
}


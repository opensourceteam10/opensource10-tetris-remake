#ifndef MULTISTATE_HPP
#define MULTISTATE_HPP

#include "gamestate.hpp"
#include "texture.hpp"

constexpr int HIDDEN_ROWS = 2; // 논리적 보드를 한 행 올림

class MultiState : public GameState {
public:
    MultiState(InputManager* manager);
    ~MultiState();

    void initialize() override;
    void exit() override;
    void run() override;
    void update() override;
    void draw() override;

    void drawBoard(int playerIdx, int offsetX, int offsetY);
    void drawCurrentPiece(int playerIdx, int offsetX, int offsetY);

private:
    Board* boards[2];
    Piece currentPieces[2];
    Piece nextPieces[2];
    int scores[2];
    bool isGameOver[2];

    Texture* tetrominoSprites[2];
    Texture* playfieldFrame[2];
    SDL_Rect tetrominoSpriteClips[2][7];
    SDL_Rect playfieldFrameClips[2][4];

    Uint32 lastDropTime[2];
    const Uint32 dropInterval = 500; // ms

    // 승리 메시지 및 버튼
    bool showResult;
    int winnerIdx; // 0 또는 1
    Texture* winText;

    const int PLAYER1_OFFSET_X = 60;
    const int PLAYER2_OFFSET_X = 420;
    const int BOARD_OFFSET_Y = 20;

    void handleEvent(int playerIdx, Action action);
    void checkLineClear(int playerIdx);
    void spawnNewPiece(int playerIdx);

    void drawResultScreen();
    bool isMouseOver(int x, int y, int w, int h, int mx, int my);
};

#endif // MULTISTATE_HPP


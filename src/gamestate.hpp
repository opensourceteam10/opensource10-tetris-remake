#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "board.hpp"
#include "inputmanager.hpp"
#include "piece.hpp"
#include "renderer.hpp"
#include "state.hpp"
#include "texture.hpp"

// The state used when playing a Tetris game
class GameState: public State
{
public:
    GameState (InputManager *manager);
    ~GameState ();
    void initialize() override;
    void exit() override;

    void run() override;
    void update() override;
    void draw() override;

    protected:
    // 게임 상태 변수
    enum GamePhase {GAME_STARTED, GAME_PLAYING, GAME_FINISHED};
    GamePhase currentPhase;

    Board *board;
    Piece currentPiece {0, 0};
    Piece nextPiece {0, 0};
    Piece holdPiece {0, 0};
    Piece ghostPiece {0, 0};

    bool hold_block_first_time;
    bool hold_block_used;
    bool game_just_started;

    unsigned long long time_snap1;
    unsigned long long time_snap2;

    Texture *countdown_texture;
    Texture *gameover_text;
    Texture *tetrominoSprites;
    SDL_Rect tetrominoSpriteClips[7];
    Texture *playfieldFrame;
    SDL_Rect playfieldFrameClips[4];

    // Stage 관련
    int stage;
    int linesCleared;
    float dropInterval;
    float dropTimer;

    Texture *stageTextTexture;
    
    int score = 0;
    Texture* scoreTextTexture = nullptr;


    // 주요 메서드
    bool isGameOver();
    void createNewPiece();
    virtual void checkState();
    void handleEvent(Action action);
    void movePieceDown();

    void drawBoard();
    void drawCurrentPiece(Piece p);
    void drawNextPiece(Piece p);
    void drawHoldPiece(Piece p);
    void drawGhostPiece(Piece p);

    int getRandom(int lower_limit, int upper_limit);


};

#endif // GAMESTATE_HPP
#include "gamestate.hpp"

#include <iostream> // DEBUG

#include "inputmanager.hpp"
#include "game.hpp"
#include "renderer.hpp"
#include "texture.hpp"
#include "utilities.hpp"

/*
 * ====================================
 * Public methods start here
 * ====================================
 */
// 스테이지별 배경색 목록
const SDL_Color stageColors[] = {
    {249, 230, 207}, // Stage 1 - 기본 밝은 색
    {100, 149, 237}, // Stage 2 - 콘월플라워 블루
    {186, 85, 211},  // Stage 3 - 오키드 퍼플
    {255, 160, 122}, // Stage 4 - 살몬 오렌지
    {144, 238, 144}, // Stage 5 - 연두색
    {135, 206, 250}, // Stage 6 - 스카이 블루
    {255, 182, 193}, // Stage 7 - 핑크
    {255, 255, 224}, // Stage 8 - 라이트 옐로우
    {119, 136, 153}, // Stage 9 - 라이트 슬레이트 그레이
    {72, 61, 139}    // Stage 10+ - 다크 슬레이트 블루
};
const int numStageColors = sizeof(stageColors) / sizeof(stageColors[0]);


 GameState::GameState (InputManager *manager) : State(manager),
 stage(1),
 linesCleared(0),
 dropInterval(0.8f),
 dropTimer(0.0f),
 stageTextTexture(nullptr)
{ }


GameState::~GameState ()
{
    exit();
}

void GameState::initialize ()
{
    currentPhase = GAME_STARTED;
    board = new Board;
    srand(time(0));
    hold_block_first_time = true;
    hold_block_used = false;
    stageTextTexture = new Texture();
    score = 0;
    scoreTextTexture = new Texture();


    
    // Get random first piece
    nextPiece.piece_type = getRandom(0, 6);
    nextPiece.rotation = 0;                     // Pieces must always start flat according to the offical Tetris guidelines
    createNewPiece(); 
    nextPiece.r = config::next_box_y;
    nextPiece.c = config::next_box_x;

    // Load necessary textures
    countdown_texture = new Texture ();
    gameover_text = new Texture ();
    gameover_text->loadFromText("Game Over!", Game::getInstance()->mRenderer->mediumFont, config::default_text_color);
    tetrominoSprites = new Texture ();
    playfieldFrame = new Texture ();
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    tetrominoSprites->loadFromImage("../../assets/tetrominoSprites.png");
    playfieldFrame->loadFromImage("../../assets/playfieldFrame.png");
    #else
    tetrominoSprites->loadFromImage("../assets/tetrominoSprites.png");
    playfieldFrame->loadFromImage("../assets/playfieldFrame.png");
    #endif

    // Create the right clips sprites
    for (int i = 0; i < 7; i++)
    {
        tetrominoSpriteClips[i].x = 16*i;
        tetrominoSpriteClips[i].y = 0;
        tetrominoSpriteClips[i].w = 16;
        tetrominoSpriteClips[i].h = 16;
    }
    for (int i = 0; i < 4; i++)
    {
        playfieldFrameClips[i].x = config::frame_sprite_size*i;
        playfieldFrameClips[i].y = 0;
        playfieldFrameClips[i].w = config::frame_sprite_size;
        playfieldFrameClips[i].h = config::frame_sprite_size;
    }
    game_just_started = true;
}

void GameState::exit ()
{
    delete board;
    delete countdown_texture;
    delete gameover_text;
    delete tetrominoSprites;
    delete playfieldFrame;
    delete stageTextTexture;
    delete scoreTextTexture;


}

void GameState::run ()
{
    switch (currentPhase)
    {
        case GAME_STARTED:
        {
            if (game_just_started)
            {
                time_snap1 = SDL_GetTicks();
                game_just_started = false;
            }
            unsigned long long ms_passed = SDL_GetTicks() - time_snap1;
            if (ms_passed < 3000)
            {
                while (mInputManager->pollAction())
                {
                    if (mInputManager->isGameExiting())
                    {
                        nextStateID = STATE_EXIT;
                        break;
                    }
                    if (mInputManager->getAction() == Action::back)
                    {
                        Game::getInstance()->popState();
                        break;                                                  // Pop the state only once even if Action::back is pressed twice
                    }
                }
                Game::getInstance()->mRenderer->clearScreen();
                draw();
                int countdown_time = ceil((3000 - ms_passed)/1000.0);           // The time left on the countdown
                if (countdown_time >= 0)
                {
                    countdown_texture->loadFromText(std::to_string(countdown_time), Game::getInstance()->mRenderer->mediumFont, config::default_text_color);
                    Game::getInstance()->mRenderer->renderTexture(countdown_texture, config::logical_window_width/2, config::logical_window_height/2);
                }
                Game::getInstance()->mRenderer->updateScreen();
            }
            else
            {
                currentPhase = GAME_PLAYING;
                time_snap1 = SDL_GetTicks();
            }
            break;
        }

        case GAME_PLAYING:
        {
            if (mInputManager->isGameExiting())
            {
                nextStateID = STATE_EXIT;
            }
            else if (!isGameOver())
            {
                while (mInputManager->pollAction())
                {
                    if (mInputManager->getAction() == Action::back)
                    {
                        Game::getInstance()->popState();
                        break;                                                // Pop the state only once even if Action::back is pressed twice
                    }
                    else
                    {
                        handleEvent(mInputManager->getAction());
                    }
                }
                
                time_snap2 = SDL_GetTicks();
                if (time_snap2 - time_snap1 >= static_cast<unsigned long long>(dropInterval * 1000))
                {
                    movePieceDown();
                    time_snap1 = SDL_GetTicks();
                }
                Game::getInstance()->mRenderer->clearScreen();
                draw();
                Game::getInstance()->mRenderer->updateScreen();
            }
            else
            {
                // Here the game has finished
                currentPhase = GAME_FINISHED;
            }
            break;
        }

        case GAME_FINISHED:
        {
            if (!mInputManager->isGameExiting())
            {
                while (mInputManager->pollAction() != 0)
                {
                    if (mInputManager->getAction() == Action::back)
                    {
                        Game::getInstance()->popState();
                        break;                                          // Pop the state only once even if Action::back is pressed twice
                    }
                }
                Game::getInstance()->mRenderer->clearScreen();
                draw();
                Game::getInstance()->mRenderer->renderTexture(gameover_text, 3+config::logical_window_width/2, config::logical_window_height/2);
                Game::getInstance()->mRenderer->updateScreen();
            }
            else
            {
                // Here the game has exited
                nextStateID = STATE_EXIT;
            }
            break;
        }
    }
}

void GameState::update ()
{
    // We don't use this function in this state, the work is done by handleEvent() 
}



void GameState::draw ()
{
    
    

    // 현재 스테이지에 해당하는 색 선택
    int colorIndex = std::min(stage - 1, numStageColors - 1);
    SDL_Color bgColor = stageColors[colorIndex];
    Game::getInstance()->mRenderer->setBackgroundColor(bgColor.r, bgColor.g, bgColor.b);
    Game::getInstance()->mRenderer->clearScreen();

    drawBoard();
    drawCurrentPiece(currentPiece);
    if (!board->isGameOver() && config::ghost_piece_enabled)
        drawGhostPiece(currentPiece);
    if (!hold_block_first_time)
        drawHoldPiece(holdPiece);
    drawNextPiece(nextPiece);
}


/*
 * ====================================
 * Private methods start here
 * ====================================
 */

bool GameState::isGameOver ()
{
    return board->isGameOver();
}

void GameState::createNewPiece ()
{
    currentPiece.piece_type = nextPiece.piece_type;
    currentPiece.rotation = nextPiece.rotation;
    currentPiece.r = currentPiece.getInitialOffsetR();
    currentPiece.c = config::playfield_width / 2 + currentPiece.getInitialOffsetC();

    for (int i = 0; i < 2; i++)
    { 
        currentPiece.r++;
        if (!board->isPositionLegal(currentPiece))
        {
            currentPiece.r--;
        }
    }
    if (currentPiece.piece_type > 1)
    {
            currentPiece.r++;
            if (!board->isPositionLegal(currentPiece))
            {
                currentPiece.r--;
            }
    }

    nextPiece.piece_type = getRandom(0, 6);
    nextPiece.rotation = 0; // Pieces must always start flat according to the offical Tetris guidelines
}

void GameState::checkState ()
{
    board->storePiece(currentPiece);
    int cleared = board->clearFullLines();  // 줄 개수 반환된다고 가정 (int로 바꿔도 에러 없으면 OK)
    if (cleared > 0) {
        int points = 0;
        if (cleared == 1) points = 100;
        else if (cleared == 2) points = 300;
        else if (cleared == 3) points = 500;
        else if (cleared >= 4) points = 800;
    
        score += points;
    }
    
    // 스테이지 로직
    linesCleared += cleared;

    while (linesCleared >= 1) {
        stage++;
        linesCleared--;
        dropInterval = std::max(0.1f, dropInterval - 0.1f);
        std::cout << "Stage Up! Now at Stage " << stage << ", drop interval = " << dropInterval << "s\n";
    }


    if (!board->isGameOver())
    {
        createNewPiece();
    }
    hold_block_used = false;                // We can now use the hold block again
}


void GameState::handleEvent (Action action)
{
    switch(action)
    {
        case Action::move_down:
        {
            currentPiece.r++;
            if (!board->isPositionLegal(currentPiece))
            {
                currentPiece.r--;
                checkState();
            }
            break;
        }

        case Action::move_left:
        {
            currentPiece.c--;
            if (!board->isPositionLegal(currentPiece))
            {
                currentPiece.c++;
            }
            break;
        }

        case Action::move_right:
        {
            currentPiece.c++;
            if (!board->isPositionLegal(currentPiece))
            {
                currentPiece.c--;
            }
            break;
        }

        case Action::drop:
        {
            while (board->isPositionLegal(currentPiece))
            {
                currentPiece.r++;
            }
            currentPiece.r--;
            checkState();
            break;
        }

        case Action::move_up:
        case Action::rotate:
        {
            currentPiece.rotation = (currentPiece.rotation + 1) % 4;
            if (!board->isPositionLegal(currentPiece))
            {
                currentPiece.rotation = (currentPiece.rotation + 3) % 4;
            }
            break;
        }

        case Action::hold:
        {
            if (hold_block_first_time)
            {
                holdPiece = Piece(currentPiece);
                holdPiece.rotation = 0;
                createNewPiece();
                hold_block_first_time = false;
                hold_block_used = true;
            }
            else if (!hold_block_used)
            {
                swap(currentPiece, holdPiece);
                holdPiece.rotation = 0;
                currentPiece.r = currentPiece.getInitialOffsetR();
                currentPiece.c = config::playfield_width / 2 + currentPiece.getInitialOffsetC();

                for (int i = 0; i < 2; i++)
                { 
                    currentPiece.r++;
                    if (!board->isPositionLegal(currentPiece))
                    {
                        currentPiece.r--;
                    }
                }
                if (currentPiece.piece_type > 1)
                {
                        currentPiece.r++;
                        if (!board->isPositionLegal(currentPiece))
                        {
                            currentPiece.r--;
                        }
                }
                hold_block_used = true;
            }
            break;
        }

        case Action::pause:
        {
            currentPhase = GAME_STARTED;
            game_just_started = true;
            Game::getInstance()->pushPaused();
        }
    }
}

void GameState::movePieceDown ()
{
    currentPiece.r++;
    if (!board->isPositionLegal(currentPiece))
    {
        currentPiece.r--;
        checkState();
    }
}

void GameState::drawBoard ()
{
    for (int i = 0; i < 2*config::true_playfield_height; i++)
    {
        // Left frame
        playfieldFrame->render(config::width_to_playfield - config::frame_sprite_size, config::height_to_playfield + i*config::frame_sprite_size,
            &playfieldFrameClips[0]);
        // Right frame
        playfieldFrame->render(config::width_to_playfield + config::block_size * config::playfield_width - (config::frame_sprite_size -
        config::frame_width), config::height_to_playfield + i*config::frame_sprite_size, &playfieldFrameClips[0]);
    }
    // Then the 2 corners
    playfieldFrame->render(config::width_to_playfield - config::frame_sprite_size, config::height_to_playfield + 
        config::block_size*config::true_playfield_height - (config::frame_sprite_size - config::frame_width), &playfieldFrameClips[2]);
    playfieldFrame->render(config::width_to_playfield + config::block_size * config::playfield_width, config::height_to_playfield + 
        config::block_size*config::true_playfield_height - (config::frame_sprite_size - config::frame_width), &playfieldFrameClips[3]);
    
    for (int i = 0; i < 2*config::playfield_width; i++)
    {
        // And the bottom frame
        playfieldFrame->render(config::width_to_playfield + i*config::frame_sprite_size, config::height_to_playfield +
            config::block_size*config::true_playfield_height, &playfieldFrameClips[1]);
    }

    // Then draw the placed blocks
    for (int row = 0; row < config::playfield_height; row++)
    {
        for (int col = 0; col < config::playfield_width; col++)
        {
            if (!board->isBlockFree(row, col))
            {
                tetrominoSprites->render(config::width_to_playfield + col * config::block_size, config::height_to_playfield +
                (row-(config::playfield_height-config::true_playfield_height))*config::block_size, &tetrominoSpriteClips[board->getTetromino(row, col)]);
            }
        }
    }
        // 스테이지 텍스트 그리기
    std::string stageText = "Stage: " + std::to_string(stage);
    stageTextTexture->loadFromText(stageText, Game::getInstance()->mRenderer->mediumFont, config::default_text_color);
    stageTextTexture->render(20, 20);  // 좌측 상단 (x=20, y=20)에 렌더링
    std::string scoreText = "Score: " + std::to_string(score);
    scoreTextTexture->loadFromText(scoreText, Game::getInstance()->mRenderer->mediumFont, config::default_text_color);
    scoreTextTexture->render(20, 50); // 스테이지 텍스트 아래


}

void GameState::drawCurrentPiece (Piece p)
{
    for (int row = 0; row < config::matrix_blocks; row++)
    {
        for (int col = 0; col < config::matrix_blocks; col++)
        {
            if (p.getBlockType(row, col) != 0)
            {
                tetrominoSprites->render(config::width_to_playfield + (col+p.c) * config::block_size, config::height_to_playfield +
                (row+p.r-(config::playfield_height-config::true_playfield_height)) *config::block_size, &tetrominoSpriteClips[p.piece_type]);
            }
        }
    }
}

void GameState::drawNextPiece (Piece p)
{
    for (int row = 0; row < config::matrix_blocks; row++)
    {
        for (int col = 0; col < config::matrix_blocks; col++)
        {
            if (p.getBlockType(row, col) != 0)
            {
                tetrominoSprites->render(config::next_box_x + col*config::block_size, config::next_box_y + row*config::block_size,
                                        &tetrominoSpriteClips[p.piece_type]);
            }
        }
    }
}

void GameState::drawHoldPiece (Piece p)
{
    for (int row = 0; row < config::matrix_blocks; row++)
    {
        for (int col = 0; col < config::matrix_blocks; col++)
        {
            if (p.getBlockType(row, col) != 0)
            {
                tetrominoSprites->render(config::hold_box_x + col*config::block_size, config::hold_box_y + row*config::block_size,
                                        &tetrominoSpriteClips[p.piece_type]);
            }
        }
    }
}

void GameState::drawGhostPiece (Piece p)
{
    ghostPiece = p;
    while (board->isPositionLegal(ghostPiece))
    {
        ghostPiece.r++;
    }
    ghostPiece.r--;

    tetrominoSprites->setAlphaMode(config::transparency_alpha);  // Change transparency to render the ghost piece

    for (int row = 0; row < config::matrix_blocks; row++)
    {
        for (int col = 0; col < config::matrix_blocks; col++)
        {
            if (ghostPiece.getBlockType(row, col) != 0)
            {
                tetrominoSprites->render(config::width_to_playfield + (col+ghostPiece.c) * config::block_size, config::height_to_playfield +
                (row+ghostPiece.r-(config::playfield_height-config::true_playfield_height))*config::block_size, &tetrominoSpriteClips[ghostPiece.piece_type]);
            }
        }
    }

    tetrominoSprites->setAlphaMode(255); // Don't forget to change it back to normal!
}

int GameState::getRandom (int lower_limit, int upper_limit)
{
    return rand() % (upper_limit - lower_limit + 1) + lower_limit;
}

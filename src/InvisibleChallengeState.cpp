#include "InvisibleChallengeState.hpp"
#include "game.hpp"
#include "config.hpp"
#include <iostream>

const float InvisibleChallengeState::FADE_DELAY = 2.0f;
const float InvisibleChallengeState::EASY_MAX_ALPHA = 0.5f;
const float InvisibleChallengeState::NORMAL_MAX_ALPHA = 0.25f;
const float InvisibleChallengeState::HARD_MAX_ALPHA = 0.05f;
const float InvisibleChallengeState::HINT_DISPLAY_TIME = 3.0f;

InvisibleChallengeState::InvisibleChallengeState(InputManager *manager) : GameState(manager)
{
    mPlacedBlockAlpha = 1.0f;
    mLastPlaceTime = 0;
    mDifficulty = EASY;
    mShowHints = true;
    mHintStartTime = 0;
    blocksPlaced = 0;
    gameStartTime = 0;
    challengeCompleted = false;
    difficultyTexture = nullptr;
    hintTexture = nullptr;
    finalResultTexture = nullptr;
}

InvisibleChallengeState::~InvisibleChallengeState()
{
    cleanupInvisibleTextures();
}

void InvisibleChallengeState::cleanupInvisibleTextures()
{
    if (difficultyTexture) { delete difficultyTexture; difficultyTexture = nullptr; }
    if (hintTexture) { delete hintTexture; hintTexture = nullptr; }
    if (finalResultTexture) { delete finalResultTexture; finalResultTexture = nullptr; }
}

void InvisibleChallengeState::initialize()
{
    GameState::initialize();
    gameStartTime = SDL_GetTicks();
    mLastPlaceTime = gameStartTime;
    mPlacedBlockAlpha = 1.0f;
    challengeCompleted = false;
    blocksPlaced = 0;
    
    cleanupInvisibleTextures();
    difficultyTexture = new Texture();
    hintTexture = new Texture();
    finalResultTexture = new Texture();
    
    showHint("Invisible Challenge Started!");
    std::cout << "Invisible Challenge Started!" << std::endl;
}

void InvisibleChallengeState::run()
{
    if (!challengeCompleted)
    {
        updateFade();
        updateDifficulty();
        
        if (isGameOver())
        {
            endChallenge();
            return;
        }
        
        if (mInputManager->isGameExiting())
        {
            nextStateID = STATE_EXIT;
            return;
        }
        
        while (mInputManager->pollAction())
        {
            if (mInputManager->getAction() == Action::back)
            {
                Game::getInstance()->popState();
                return;
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
        while (mInputManager->pollAction())
        {
            if (mInputManager->isGameExiting())
            {
                nextStateID = STATE_EXIT;
                break;
            }
            if (mInputManager->getAction() == Action::back || mInputManager->getAction() == Action::select)
            {
                Game::getInstance()->popState();
                return;
            }
        }
        
        Game::getInstance()->mRenderer->clearScreen();
        drawFinalResults();
        Game::getInstance()->mRenderer->updateScreen();
    }
}

void InvisibleChallengeState::draw()
{
    drawInvisibleBoard();
    drawCurrentPiece(currentPiece);

    if (!board->isGameOver() && config::ghost_piece_enabled)
        drawGhostPiece(currentPiece);  // ← 추가

    if (!hold_block_first_time)
        drawHoldPiece(holdPiece);
    drawNextPiece(nextPiece);

    drawUI();
}

void InvisibleChallengeState::updateFade()
{
    unsigned long long currentTime = SDL_GetTicks();
    float timeSincePlace = (currentTime - mLastPlaceTime) / 1000.0f;
    
    if (timeSincePlace > FADE_DELAY)
    {
        float targetAlpha = getTargetAlpha();
        float fadeProgress = (timeSincePlace - FADE_DELAY) / 2.0f;
        mPlacedBlockAlpha = 1.0f - (fadeProgress * (1.0f - targetAlpha));
        mPlacedBlockAlpha = std::max(mPlacedBlockAlpha, targetAlpha);
    }
    else
    {
        mPlacedBlockAlpha = 1.0f;
    }
}

void InvisibleChallengeState::updateDifficulty()
{
    unsigned long long currentTime = SDL_GetTicks();
    float elapsedMinutes = (currentTime - gameStartTime) / 60000.0f;
    
    if (elapsedMinutes > 2.0f && mDifficulty != HARD)
    {
        mDifficulty = HARD;
        showHint("HARD MODE!");
    }
    else if (elapsedMinutes > 1.0f && mDifficulty == EASY)
    {
        mDifficulty = NORMAL;
        showHint("NORMAL MODE!");
    }
}

void InvisibleChallengeState::showHint(const std::string& message)
{
    if (mShowHints && hintTexture)
    {
        mHintStartTime = SDL_GetTicks();
        hintTexture->loadFromText(message, Game::getInstance()->mRenderer->mediumFont, 
                                 SDL_Color{255, 255, 0, 255});
    }
}

void InvisibleChallengeState::endChallenge()
{
    challengeCompleted = true;
    std::cout << "Challenge Completed! Score: " << score << std::endl;
}

void InvisibleChallengeState::drawInvisibleBoard()
{
    for (int i = 0; i < 2*config::true_playfield_height; i++)
    {
        playfieldFrame->render(config::width_to_playfield - config::frame_sprite_size, 
                              config::height_to_playfield + i*config::frame_sprite_size,
                              &playfieldFrameClips[0]);
        playfieldFrame->render(config::width_to_playfield + config::block_size * config::playfield_width - 
                              (config::frame_sprite_size - config::frame_width), 
                              config::height_to_playfield + i*config::frame_sprite_size, 
                              &playfieldFrameClips[0]);
    }
    
    for (int i = 0; i < 2*config::playfield_width; i++)
    {
        playfieldFrame->render(config::width_to_playfield + i*config::frame_sprite_size, 
                              config::height_to_playfield + config::block_size*config::true_playfield_height, 
                              &playfieldFrameClips[1]);
    }
    
    Uint8 alpha = (Uint8)(mPlacedBlockAlpha * 255);
    tetrominoSprites->setAlphaMode(alpha);
    
    for (int row = 0; row < config::playfield_height; row++)
    {
        for (int col = 0; col < config::playfield_width; col++)
        {
            if (!board->isBlockFree(row, col))
            {
                tetrominoSprites->render(config::width_to_playfield + col * config::block_size, 
                                       config::height_to_playfield + 
                                       (row-(config::playfield_height-config::true_playfield_height))*config::block_size, 
                                       &tetrominoSpriteClips[board->getTetromino(row, col)]);
            }
        }
    }
    
    tetrominoSprites->setAlphaMode(255);
}

void InvisibleChallengeState::drawUI()
{
    if (stageTextTexture)
    {
        std::string stageText = "Stage: " + std::to_string(stage);
        stageTextTexture->loadFromText(stageText, Game::getInstance()->mRenderer->mediumFont, config::default_text_color);
        stageTextTexture->render(20, 20);
    }
    
    if (scoreTextTexture)
    {
        std::string scoreText = "Score: " + std::to_string(score);
        scoreTextTexture->loadFromText(scoreText, Game::getInstance()->mRenderer->mediumFont, config::default_text_color);
        scoreTextTexture->render(20, 50);
    }
    
    if (difficultyTexture)
    {
        std::string diffText = "Difficulty: " + getDifficultyName();
        difficultyTexture->loadFromText(diffText, Game::getInstance()->mRenderer->mediumFont, config::default_text_color);
        difficultyTexture->render(450, 20);
    }
    
    // 투명도 표시 (추가할 부분)
    Texture alphaTexture;
    int alphaPercent = (int)(mPlacedBlockAlpha * 100);
    std::string alphaText = "Alpha: " + std::to_string(alphaPercent) + "%";
    alphaTexture.loadFromText(alphaText, Game::getInstance()->mRenderer->mediumFont, config::default_text_color);
    alphaTexture.render(450, 50);
    
    // 생존 시간 표시 (추가할 부분)
    unsigned long long currentTime = SDL_GetTicks();
    int survivalSeconds = (currentTime - gameStartTime) / 1000;
    Texture timeTexture;
    std::string timeText = "Time: " + std::to_string(survivalSeconds) + "s";
    timeTexture.loadFromText(timeText, Game::getInstance()->mRenderer->mediumFont, config::default_text_color);
    timeTexture.render(450, 80);
    
    if (mShowHints && hintTexture)
    {
        unsigned long long currentTime = SDL_GetTicks();
        float hintElapsed = (currentTime - mHintStartTime) / 1000.0f;
        
        if (hintElapsed < HINT_DISPLAY_TIME)
        {
            hintTexture->renderCentered(config::logical_window_width/2, 120);
        }
    }
}

void InvisibleChallengeState::drawFinalResults()
{
    if (!finalResultTexture) return;
    
    std::string resultText = "INVISIBLE CHALLENGE COMPLETED!\n\nFinal Score: " + std::to_string(score) + 
                            "\nBlocks Placed: " + std::to_string(blocksPlaced) + "\n\nPress BACK to continue";
    
    finalResultTexture->loadFromText(resultText, Game::getInstance()->mRenderer->mediumFont, config::default_text_color);
    finalResultTexture->renderCentered(config::logical_window_width/2, config::logical_window_height/2);
}

void InvisibleChallengeState::checkState()
{
    mLastPlaceTime = SDL_GetTicks();
    blocksPlaced++;
    GameState::checkState();
    std::cout << "Block #" << blocksPlaced << " placed!" << std::endl;
}

float InvisibleChallengeState::getTargetAlpha()
{
    switch (mDifficulty)
    {
        case EASY: return EASY_MAX_ALPHA;
        case NORMAL: return NORMAL_MAX_ALPHA;
        case HARD: return HARD_MAX_ALPHA;
        default: return EASY_MAX_ALPHA;
    }
}

std::string InvisibleChallengeState::getDifficultyName()
{
    switch (mDifficulty)
    {
        case EASY: return "EASY";
        case NORMAL: return "NORMAL";
        case HARD: return "HARD";
        default: return "EASY";
    }
}
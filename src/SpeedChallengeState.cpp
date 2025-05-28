#include "SpeedChallengeState.hpp"
#include "game.hpp"
#include "config.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

SpeedChallengeState::SpeedChallengeState(InputManager *manager) : GameState(manager)
{
    challengeTimeLimit = 180.0f;  // 3분
    remainingTime = challengeTimeLimit;
    timeStarted = 0;
    isTimeUp = false;
    
    baseDropInterval = 0.8f;
    speedMultiplier = 1.0f;
    
    doubleScoreActive = false;
    doubleScoreTimer = 0.0f;
    autoLineClearActive = false;
    autoLineClearTimer = 0.0f;
    
    timerTexture = nullptr;
    speedTexture = nullptr;
    powerupTexture = nullptr;
    finalScoreTexture = nullptr;
    
    challengeCompleted = false;
    finalScore = 0;
}

SpeedChallengeState::~SpeedChallengeState()
{
    if (timerTexture) delete timerTexture;
    if (speedTexture) delete speedTexture;
    if (powerupTexture) delete powerupTexture;
    if (finalScoreTexture) delete finalScoreTexture;
}

void SpeedChallengeState::initialize()
{
    // 부모 클래스 초기화
    GameState::initialize();
    
    // Speed Challenge 전용 초기화
    timeStarted = SDL_GetTicks();
    remainingTime = challengeTimeLimit;
    dropInterval = baseDropInterval;
    
    // 텍스처 초기화
    timerTexture = new Texture();
    speedTexture = new Texture();
    powerupTexture = new Texture();
    finalScoreTexture = new Texture();
    
    std::cout << "🏃‍♂️ Speed Challenge Started! Get maximum score in 3 minutes!" << std::endl;
}

void SpeedChallengeState::run()
{
    if (!challengeCompleted && !isTimeUp)
    {
        updateTimer();
        updateSpeed();
        updatePowerups();
        
        // 시간이 다 되었으면 챌린지 종료
        if (remainingTime <= 0)
        {
            endChallenge();
            return;
        }
        
        // 게임오버 체크
        if (isGameOver())
        {
            endChallenge();
            return;
        }
        
        // 부모 클래스의 게임 로직 실행
        GameState::run();
    }
    else
    {
        // 챌린지 완료 후 결과 화면
        while (mInputManager->pollAction())
        {
            if (mInputManager->isGameExiting())
            {
                nextStateID = STATE_EXIT;
                break;
            }
            if (mInputManager->getAction() == Action::back || mInputManager->getAction() == Action::select)
            {
                Game::getInstance()->popState(); // 챌린지 메뉴로 돌아가기
                break;
            }
        }
        
        Game::getInstance()->mRenderer->clearScreen();
        drawFinalResults();
        Game::getInstance()->mRenderer->updateScreen();
    }
}

void SpeedChallengeState::draw()
{
    if (!challengeCompleted)
    {
        // 기본 게임 화면 그리기
        GameState::draw();
        
        // Speed Challenge 전용 UI 그리기
        drawTimer();
        drawPowerupStatus();
    }
}

void SpeedChallengeState::updateTimer()
{
    unsigned long long currentTime = SDL_GetTicks();
    float elapsedSeconds = (currentTime - timeStarted) / 1000.0f;
    remainingTime = challengeTimeLimit - elapsedSeconds;
    
    if (remainingTime <= 0)
    {
        remainingTime = 0;
        isTimeUp = true;
    }
}

void SpeedChallengeState::updateSpeed()
{
    // 시간이 지날수록 속도 증가 (최대 3배까지)
    float timeProgress = 1.0f - (remainingTime / challengeTimeLimit);
    speedMultiplier = 1.0f + (timeProgress * 2.0f); // 1배 → 3배
    dropInterval = baseDropInterval / speedMultiplier;
}

void SpeedChallengeState::updatePowerups()
{
    float deltaTime = 1.0f / 60.0f; // 대략적인 프레임 시간
    
    // 2배 점수 파워업 타이머
    if (doubleScoreActive)
    {
        doubleScoreTimer -= deltaTime;
        if (doubleScoreTimer <= 0)
        {
            doubleScoreActive = false;
            std::cout << "💫 Double Score ended!" << std::endl;
        }
    }


    
    // 자동 라인 클리어 파워업 타이머
    if (autoLineClearActive)
{
    autoLineClearTimer -= deltaTime;
    if (autoLineClearTimer <= 0)
    {
        autoLineClearActive = false;
        std::cout << "Auto Line Clear ended!" << std::endl;
    }
    else
    {
        // 2초마다 자동 클리어 실행
        static float autoLineClearCooldown = 0;
        autoLineClearCooldown -= deltaTime;
        if (autoLineClearCooldown <= 0)
        {
            std::cout << "Auto clear triggered!" << std::endl;  // 디버그 메시지
            
            // 방법 1: 아래쪽 줄들 중에서 가장 많이 찬 줄 클리어
            performAutoLineClear();
            
            autoLineClearCooldown = 2.0f;
        }
    }
}
}

void SpeedChallengeState::performAutoLineClear()
{
    // 아래쪽 5줄 중에서 가장 많이 찬 줄 찾기
    int bestRow = -1;
    int maxBlocks = 0;
    
    for (int row = config::playfield_height - 5; row < config::playfield_height; row++)
    {
        if (row < 0) continue;  // 경계 체크
        
        int filledBlocks = 0;
        for (int col = 0; col < config::playfield_width; col++)
        {
            if (!board->isBlockFree(row, col))
            {
                filledBlocks++;
            }
        }
        
        if (filledBlocks > maxBlocks && filledBlocks >= 5)  // 최소 5개 이상 찬 줄
        {
            maxBlocks = filledBlocks;
            bestRow = row;
        }
    }
    
    if (bestRow != -1)
    {
        std::cout << "Auto clearing row " << bestRow << " with " << maxBlocks << " blocks" << std::endl;
        
        // 해당 줄을 강제로 완전히 채우기 (Board 클래스 수정 없이)
        // 임시 방법: clearFullLines() 호출 후 보너스 점수만 주기
        int clearedBefore = board->clearFullLines();
        score += 300;  // 자동 클리어 보너스
        
        std::cout << "Auto clear bonus: +300 points!" << std::endl;
    }
    else
    {
        // 클리어할 줄이 없으면 작은 보너스만
        score += 50;
        std::cout << "Auto clear bonus: +50 points!" << std::endl;
    }
}




void SpeedChallengeState::spawnPowerup()
{
    // 라인 클리어 시 일정 확률로 파워업 생성
    if (rand() % 100 < POWERUP_SPAWN_CHANCE)
    {
        int powerupType = rand() % 2; // 0: 2배 점수, 1: 자동 라인 클리어
        
        if (powerupType == 0)
        {
            activateDoubleScore();
        }
        else
        {
            activateAutoLineClear();
        }
    }
}

void SpeedChallengeState::activateDoubleScore()
{
    doubleScoreActive = true;
    doubleScoreTimer = 10.0f; // 10초간 지속
    std::cout << "Double Score activated for 10 seconds!" << std::endl;
}

void SpeedChallengeState::activateAutoLineClear()
{
    autoLineClearActive = true;
    autoLineClearTimer = 15.0f; // 15초간 지속
    std::cout << "Auto Line Clear activated for 15 seconds!" << std::endl;
}

void SpeedChallengeState::endChallenge()
{
    challengeCompleted = true;
    finalScore = score;
    
    std::cout << "🏁 Speed Challenge Completed!" << std::endl;
    std::cout << "Final Score: " << finalScore << std::endl;
    
    // 최종 점수 등급 계산
    std::string grade;
    if (finalScore >= 50000) grade = "S";
    else if (finalScore >= 30000) grade = "A";
    else if (finalScore >= 20000) grade = "B";
    else if (finalScore >= 10000) grade = "C";
    else grade = "D";
    
    std::cout << "Grade: " << grade << std::endl;
}

void SpeedChallengeState::drawTimer()
{
    // 남은 시간을 MM:SS 형식으로 표시
    int minutes = (int)remainingTime / 60;
    int seconds = (int)remainingTime % 60;
    
    std::stringstream ss;
    ss << "Time: " << std::setfill('0') << std::setw(2) << minutes 
       << ":" << std::setfill('0') << std::setw(2) << seconds;
    
    timerTexture->loadFromText(ss.str(), Game::getInstance()->mRenderer->mediumFont, 
                              remainingTime < 30 ? SDL_Color{255, 0, 0, 255} : config::default_text_color);
    timerTexture->render(450, 20);
}

void SpeedChallengeState::drawPowerupStatus()
{
    std::string powerupStatus = "";
    
    if (doubleScoreActive)
    {
        powerupStatus += "DOUBLE SCORE ";
    }
    if (autoLineClearActive)
    {
        powerupStatus += "AUTO CLEAR ";
    }
    
    if (!powerupStatus.empty())
    {
        powerupTexture->loadFromText(powerupStatus, Game::getInstance()->mRenderer->mediumFont, 
                                   SDL_Color{255, 255, 0, 255});
        powerupTexture->render(20, 100);
    }
    
    // 속도 표시
    std::stringstream speedSS;
    speedSS << "Speed: x" << std::fixed << std::setprecision(1) << speedMultiplier;
    speedTexture->loadFromText(speedSS.str(), Game::getInstance()->mRenderer->mediumFont, config::default_text_color);
    speedTexture->render(450, 50);
}

void SpeedChallengeState::checkState()
{
    board->storePiece(currentPiece);
    int cleared = board->clearFullLines();
    
    if (cleared > 0) {
        // 기본 점수 계산
        int points = 0;
        if (cleared == 1) points = 100;
        else if (cleared == 2) points = 300;
        else if (cleared == 3) points = 500;
        else if (cleared >= 4) points = 800;
        
        // 속도 보너스 적용
        points = (int)(points * speedMultiplier);
        
        // 2배 점수 파워업 적용
        if (doubleScoreActive) {
            points *= 2;
        }
        
        score += points;
        
        // 파워업 스폰 체크 (15% 확률)
        spawnPowerup();
        
        std::cout << "Score: +" << points << " (Total: " << score << ")" << std::endl;
    }
    
    // 스테이지 로직 (기존 코드)
    linesCleared += cleared;
    while (linesCleared >= 1) {
        stage++;
        linesCleared--;
        // Speed Challenge에서는 dropInterval을 speedMultiplier로 관리
        std::cout << "Stage Up! Now at Stage " << stage << std::endl;
    }

    if (!board->isGameOver())
    {
        createNewPiece();
    }
    hold_block_used = false;
}

void SpeedChallengeState::drawFinalResults()
{
    // 최종 결과 화면
    std::stringstream resultSS;
    resultSS << "SPEED CHALLENGE COMPLETED!\n\n";
    resultSS << "Final Score: " << finalScore << "\n\n";
    
    std::string grade;
    if (finalScore >= 50000) grade = "S";
    else if (finalScore >= 30000) grade = "A";  
    else if (finalScore >= 20000) grade = "B";
    else if (finalScore >= 10000) grade = "C";
    else grade = "D";
    
    resultSS << "Grade: " << grade << "\n\n";
    resultSS << "Press BACK or ENTER to continue";
    
    finalScoreTexture->loadFromText(resultSS.str(), Game::getInstance()->mRenderer->mediumFont, config::default_text_color);
    finalScoreTexture->renderCentered(config::logical_window_width/2, config::logical_window_height/2);
}
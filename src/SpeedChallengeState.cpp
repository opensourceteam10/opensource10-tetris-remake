#include "SpeedChallengeState.hpp"
#include "game.hpp"
#include "config.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

SpeedChallengeState::SpeedChallengeState(InputManager *manager) : GameState(manager)
{
    // 기본값 설정
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
    
    // 텍스처 포인터들을 nullptr로 초기화
    timerTexture = nullptr;
    speedTexture = nullptr;
    powerupTexture = nullptr;
    finalScoreTexture = nullptr;
    
    challengeCompleted = false;
    finalScore = 0;
}

SpeedChallengeState::~SpeedChallengeState()
{
    // Speed Challenge 전용 텍스처들만 해제
    // 부모 클래스는 건드리지 않음
    cleanupSpeedChallengeTextures();
}

void SpeedChallengeState::cleanupSpeedChallengeTextures()
{
    if (timerTexture) {
        delete timerTexture;
        timerTexture = nullptr;
    }
    if (speedTexture) {
        delete speedTexture;
        speedTexture = nullptr;
    }
    if (powerupTexture) {
        delete powerupTexture;
        powerupTexture = nullptr;
    }
    if (finalScoreTexture) {
        delete finalScoreTexture;
        finalScoreTexture = nullptr;
    }
}

void SpeedChallengeState::initialize()
{
    // 부모 클래스 초기화 먼저 호출
    GameState::initialize();
    
    // Speed Challenge 전용 초기화
    timeStarted = SDL_GetTicks();
    remainingTime = challengeTimeLimit;
    dropInterval = baseDropInterval;
    challengeCompleted = false;
    finalScore = 0;
    isTimeUp = false;
    
    doubleScoreActive = false;
    doubleScoreTimer = 0.0f;
    autoLineClearActive = false;
    autoLineClearTimer = 0.0f;
    speedMultiplier = 1.0f;
    
    // 기존 Speed Challenge 텍스처들만 정리
    cleanupSpeedChallengeTextures();
    
    // 새로 할당
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
        
        // 입력 처리를 직접 관리 (부모 클래스 run() 호출 대신)
        if (mInputManager->isGameExiting())
        {
            nextStateID = STATE_EXIT;
            return;
        }
        
        bool shouldReturn = false;
        while (mInputManager->pollAction())
        {
            if (mInputManager->getAction() == Action::back)
            {
                Game::getInstance()->popState();
                return;  // 즉시 반환
            }
            else if (mInputManager->getAction() == Action::pause)
            {
                currentPhase = GAME_STARTED;
                game_just_started = true;
                Game::getInstance()->pushPaused();
                return;  // 즉시 반환
            }
            else
            {
                handleEvent(mInputManager->getAction());
            }
        }
        
        // 자동 드롭 처리
        time_snap2 = SDL_GetTicks();
        if (time_snap2 - time_snap1 >= static_cast<unsigned long long>(dropInterval * 1000))
        {
            movePieceDown();
            time_snap1 = SDL_GetTicks();
        }
        
        // 화면 그리기
        Game::getInstance()->mRenderer->clearScreen();
        draw();
        Game::getInstance()->mRenderer->updateScreen();
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
                Game::getInstance()->popState();
                return;  // 즉시 반환
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
                performAutoLineClear();
                autoLineClearCooldown = 2.0f;
            }
        }
    }
}

void SpeedChallengeState::performAutoLineClear()
{
    // 간단한 보너스 점수만 주기 (복잡한 라인 조작 없이)
    score += 50;
    std::cout << "Auto clear bonus: +50 points!" << std::endl;
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
    std::cout << "💫 Double Score activated for 10 seconds!" << std::endl;
}

void SpeedChallengeState::activateAutoLineClear()
{
    autoLineClearActive = true;
    autoLineClearTimer = 15.0f; // 15초간 지속
    std::cout << "🚀 Auto Line Clear activated for 15 seconds!" << std::endl;
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
    if (!timerTexture) return;
    
    // 남은 시간을 MM:SS 형식으로 표시
    int minutes = (int)remainingTime / 60;
    int seconds = (int)remainingTime % 60;
    
    std::stringstream ss;
    ss << "Time: " << std::setfill('0') << std::setw(2) << minutes 
       << ":" << std::setfill('0') << std::setw(2) << seconds;
    
    SDL_Color textColor = remainingTime < 30 ? SDL_Color{255, 0, 0, 255} : config::default_text_color;
    timerTexture->loadFromText(ss.str(), Game::getInstance()->mRenderer->mediumFont, textColor);
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
    
    if (!powerupStatus.empty() && powerupTexture)
    {
        powerupTexture->loadFromText(powerupStatus, Game::getInstance()->mRenderer->mediumFont, 
                                   SDL_Color{255, 255, 0, 255});
        powerupTexture->render(20, 100);
    }
    
    // 속도 표시
    if (speedTexture)
    {
        std::stringstream speedSS;
        speedSS << "Speed: x" << std::fixed << std::setprecision(1) << speedMultiplier;
        speedTexture->loadFromText(speedSS.str(), Game::getInstance()->mRenderer->mediumFont, config::default_text_color);
        speedTexture->render(450, 50);
    }
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
    if (!finalScoreTexture) return;
    
    // 최종 결과 화면
    std::string grade;
    if (finalScore >= 50000) grade = "S";
    else if (finalScore >= 30000) grade = "A";  
    else if (finalScore >= 20000) grade = "B";
    else if (finalScore >= 10000) grade = "C";
    else grade = "D";
    
    std::stringstream resultSS;
    resultSS << "SPEED CHALLENGE COMPLETED!" << std::endl << std::endl;
    resultSS << "Final Score: " << finalScore << std::endl << std::endl;
    resultSS << "Grade: " << grade << std::endl << std::endl;
    resultSS << "Press BACK or ENTER to continue";
    
    finalScoreTexture->loadFromText(resultSS.str(), Game::getInstance()->mRenderer->mediumFont, config::default_text_color);
    finalScoreTexture->renderCentered(config::logical_window_width/2, config::logical_window_height/2);
}
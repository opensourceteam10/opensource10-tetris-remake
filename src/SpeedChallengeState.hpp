#ifndef SPEEDCHALLENGESTATE_HPP
#define SPEEDCHALLENGESTATE_HPP

#include "gamestate.hpp"
#include "texture.hpp"

// 3분 스피드 챌린지 모드
class SpeedChallengeState : public GameState
{
public:
    SpeedChallengeState(InputManager *manager);
    ~SpeedChallengeState();
    
    void initialize() override;
    void run() override;
    void draw() override;
    void checkState() override;

private:
    // 타이머 관련
    float challengeTimeLimit;        // 3분 (180초)
    float remainingTime;             // 남은 시간
    unsigned long long timeStarted;  // 시작 시간
    bool isTimeUp;                   // 시간 종료 여부
    
    // 스피드 관련
    float baseDropInterval;          // 기본 속도
    float speedMultiplier;           // 속도 배수 (시간이 지날수록 증가)
    
    // 파워업 관련
    bool doubleScoreActive;          // 2배 점수 활성화
    float doubleScoreTimer;          // 2배 점수 지속 시간
    bool autoLineClearActive;        // 자동 라인 클리어 활성화
    float autoLineClearTimer;        // 자동 라인 클리어 지속 시간
    
    // UI 텍스처들
    Texture *timerTexture;           // 시간 표시
    Texture *speedTexture;           // 속도 표시
    Texture *powerupTexture;         // 파워업 표시
    Texture *finalScoreTexture;      // 최종 점수
    
    // 게임 상태
    bool challengeCompleted;         // 챌린지 완료 여부
    int finalScore;                  // 최종 점수
    
    // 메서드들
    void updateTimer();              // 타이머 업데이트
    void updateSpeed();              // 속도 업데이트
    void updatePowerups();           // 파워업 업데이트
    void spawnPowerup();             // 파워업 생성 (랜덤)
    void activateDoubleScore();      // 2배 점수 활성화
    void activateAutoLineClear();    // 자동 라인 클리어 활성화
    void performAutoLineClear();     // 자동 라인 클리어 실행
    void endChallenge();             // 챌린지 종료
    void drawTimer();                // 타이머 그리기
    void drawPowerupStatus();        // 파워업 상태 그리기
    void drawFinalResults();         // 최종 결과 그리기
    
    // 파워업 확률 (%)
    static const int POWERUP_SPAWN_CHANCE = 15; // 라인 클리어 시 15% 확률
};

#endif // SPEEDCHALLENGESTATE_HPP
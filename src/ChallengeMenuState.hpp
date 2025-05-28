#ifndef CHALLENGEMENUSTATE_HPP
#define CHALLENGEMENUSTATE_HPP

#include "state.hpp"
#include "button.hpp"
#include "texture.hpp"
#include <vector>

// Challenge Mode 메뉴 선택 화면
class ChallengeMenuState : public State
{
public:
    ChallengeMenuState(InputManager *manager);
    ~ChallengeMenuState();
    
    void initialize() override;
    void exit() override;
    void run() override;
    void update() override;
    void draw() override;

private:
    std::vector<Button*> mButtons;
    Texture *titleTexture;
    Texture *descriptionTexture;
    int index;  // 현재 선택된 버튼 인덱스
    
    // 콜백 함수들
    static void startSpeedChallenge();    // 3분 스피드 챌린지
    static void startMasterChallenge();   // 40라인 마스터 챌린지  
    static void startInvisibleChallenge(); // 투명 블록 챌린지
    static void goBack();                 // 뒤로가기
};

#endif // CHALLENGEMENUSTATE_HPP
#ifndef CHALLENGEMENUSTATE_HPP
#define CHALLENGEMENUSTATE_HPP

#include "gamestate.hpp"
#include "state.hpp"
#include "button.hpp"
#include "texture.hpp"
#include <vector>

// Challenge Mode 메뉴 선택 화면
class ChallengeMenuState : public GameState
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
    static void startSpeedChallenge();      // 스피드 챌린지 시작
    static void startInvisibleChallenge();  // 투명 블록 챌린지 시작
    static void goBack();                   // 뒤로 가기
};

#endif // CHALLENGEMENUSTATE_HPP

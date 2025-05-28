#ifndef MODESELECTSTATE_HPP
#define MODESELECTSTATE_HPP

#include "state.hpp"
#include "button.hpp"
#include "texture.hpp"
#include <vector>

// 모드 선택 화면 클래스
class ModeSelectState : public State
{
public:
    ModeSelectState(InputManager *manager);
    ~ModeSelectState();
    
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
    static void startMode1();  // 기본 테트리스
    static void startMode2();  // 멀티플레이
    static void startMode3();  // 챌린지 모드 (미구현)
    static void goBack();      // 뒤로가기
};

#endif // MODESELECTSTATE_HPP
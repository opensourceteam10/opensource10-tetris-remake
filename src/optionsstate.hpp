#ifndef OPTIONSTATE_HPP
#define OPTIONSTATE_HPP

#include <vector>
#include "button.hpp"
#include "inputmanager.hpp"
#include "state.hpp"

class OptionState : public State {
public:
    OptionState(InputManager* inputManager);
    virtual ~OptionState();

    virtual void initialize() override = 0;
    virtual void update() override = 0;
    virtual void exit() override = 0;

    void run() override;   
    void draw() override;  // 공통 draw는 구현, drawOptions만 하위에서 담당

    void addButton (Button *button);

protected:
    size_t index;
    std::vector<Button*> mButtons;

    virtual void drawOptions() = 0; // 옵션 항목 렌더링은 하위 클래스에 위임
};

#endif // OPTIONSTATE_HPP

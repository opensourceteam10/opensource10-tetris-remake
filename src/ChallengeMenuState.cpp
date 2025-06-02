#include "ChallengeMenuState.hpp"
#include "game.hpp"
#include "config.hpp"
#include <iostream>

ChallengeMenuState::ChallengeMenuState(InputManager *manager) : GameState(manager), index(0)
{
    titleTexture = nullptr;
    descriptionTexture = nullptr;
}

ChallengeMenuState::~ChallengeMenuState()
{
    exit();
}

void ChallengeMenuState::initialize()
{
    index = 0;

    // 텍스처 초기화
    titleTexture = new Texture();
    titleTexture->loadFromText("CHALLENGE MODE", Game::getInstance()->mRenderer->bigFont, config::default_text_color);

    descriptionTexture = new Texture();
    descriptionTexture->loadFromText("Choose Your Challenge", Game::getInstance()->mRenderer->mediumFont, config::default_text_color);

    // 버튼 설정 (운영체제 무관)
    const int buttonWidth = 280;
    const int buttonHeight = 50;
    const int buttonSpacing = 70;
    const int startY = 150;
    const int buttonX = (config::logical_window_width - buttonWidth) / 2;

    mButtons.push_back(new Button("Speed Challenge", &ChallengeMenuState::startSpeedChallenge, buttonX, startY, buttonWidth, buttonHeight));
    mButtons.push_back(new Button("Invisible Challenge", &ChallengeMenuState::startInvisibleChallenge, buttonX, startY + buttonSpacing, buttonWidth, buttonHeight));
    mButtons.push_back(new Button("Back", &ChallengeMenuState::goBack, buttonX, startY + 2 * buttonSpacing, buttonWidth, buttonHeight));
}



void ChallengeMenuState::exit()
{
    // 버튼들 정리
    for (auto button : mButtons)
    {
        delete button;
    }
    mButtons.clear();
    
    // 텍스처들 정리
    if (titleTexture)
    {
        delete titleTexture;
        titleTexture = nullptr;
    }
    if (descriptionTexture)
    {
        delete descriptionTexture;
        descriptionTexture = nullptr;
    }
}

void ChallengeMenuState::run()
{
    update();
    draw();
}

void ChallengeMenuState::update()
{
    while (mInputManager->pollAction() != 0)
    {
        if (mInputManager->isGameExiting())
        {
            nextStateID = STATE_EXIT;
            break;
        }
        
        switch (mInputManager->getAction())
        {
            case Action::select:
            {
                mButtons[index]->callbackFunction();
                break;
            }
            
            case Action::move_up:
            {
                if (index > 0)
                {
                    --index;
                }
                break;
            }
            
            case Action::move_down:
            {
                if (index < mButtons.size()-1)
                {
                    ++index;
                }
                break;
            }
            
            case Action::back:
            {
                Game::getInstance()->popState();
                break;
            }
        }
    }
}

void ChallengeMenuState::draw()
{
    Game::getInstance()->mRenderer->clearScreen();
    
    // 타이틀 그리기
    if (titleTexture)
    {
        titleTexture->renderCentered(config::logical_window_width/2, 50);
    }
    
    // 설명 텍스트 그리기
    if (descriptionTexture)
    {
        descriptionTexture->renderCentered(config::logical_window_width/2, 90);
    }
    
    // 버튼들 그리기
    for (auto button : mButtons)
    {
        button->draw();
    }
    
    // 현재 선택된 버튼 하이라이트
    if (index < mButtons.size())
    {
        SDL_Rect highlight_box = {
            mButtons[index]->getX(), 
            mButtons[index]->getY(), 
            mButtons[index]->getWidth(), 
            mButtons[index]->getHeight()
        };
        
        SDL_SetRenderDrawBlendMode(Game::getInstance()->mRenderer->mSDLRenderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(Game::getInstance()->mRenderer->mSDLRenderer, 255, 255, 255, config::transparency_alpha-20);
        SDL_RenderFillRect(Game::getInstance()->mRenderer->mSDLRenderer, &highlight_box);
        SDL_SetRenderDrawBlendMode(Game::getInstance()->mRenderer->mSDLRenderer, SDL_BLENDMODE_NONE);
    }
    
    Game::getInstance()->mRenderer->updateScreen();
}

// 콜백 함수들 구현
void ChallengeMenuState::startSpeedChallenge()
{
    std::cout << "Speed Challenge Started!" << std::endl;
    // TODO: SpeedChallengeState 연결
    Game::getInstance()->pushSpeedChallenge();
}


void ChallengeMenuState::startInvisibleChallenge()
{
    std::cout << "Invisible Challenge Started!" << std::endl;
    Game::getInstance()->pushInvisibleChallenge(); // 이 줄 추가
}
void ChallengeMenuState::goBack()
{
    Game::getInstance()->popState();
}
#include "ModeSelectState.hpp"
#include "game.hpp"
#include "config.hpp"
#include <iostream>

ModeSelectState::ModeSelectState(InputManager *manager) : State(manager), index(0)
{
    titleTexture = nullptr;
    descriptionTexture = nullptr;
}

ModeSelectState::~ModeSelectState()
{
    exit();
}

void ModeSelectState::initialize()
{
    index = 0;
    
    // 타이틀 텍스처 생성
    titleTexture = new Texture();
    titleTexture->loadFromText("TETRIS", Game::getInstance()->mRenderer->bigFont, config::default_text_color);
    
    // 설명 텍스처 생성
    descriptionTexture = new Texture();
    descriptionTexture->loadFromText("Select Game Mode", Game::getInstance()->mRenderer->mediumFont, config::default_text_color);
    
    // 버튼들 생성 (모든 버튼 폭을 200으로 통일)
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    // Windows 경로
    mButtons.push_back(new Button("Single Mode", &ModeSelectState::startMode1, (config::logical_window_width-200)/2, 150, 200, 40));
    mButtons.push_back(new Button("Multi Mode", &ModeSelectState::startMode2, (config::logical_window_width-200)/2, 200, 200, 40));
    mButtons.push_back(new Button("Challenge Mode", &ModeSelectState::startMode3, (config::logical_window_width-200)/2, 250, 200, 40));
    mButtons.push_back(new Button("Back", &ModeSelectState::goBack, (config::logical_window_width-200)/2, 300, 200, 40));
    #else
    // Mac/Linux 경로
    mButtons.push_back(new Button("Single Mode", &ModeSelectState::startMode1, (config::logical_window_width-200)/2, 150, 200, 40));
    mButtons.push_back(new Button("Multi Mode", &ModeSelectState::startMode2, (config::logical_window_width-200)/2, 200, 200, 40));
    mButtons.push_back(new Button("Challenge Mode", &ModeSelectState::startMode3, (config::logical_window_width-200)/2, 250, 200, 40));
    mButtons.push_back(new Button("Back", &ModeSelectState::goBack, (config::logical_window_width-200)/2, 300, 200, 40));
    #endif
}

void ModeSelectState::exit()
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

void ModeSelectState::run()
{
    update();
    draw();
}

void ModeSelectState::update()
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

void ModeSelectState::draw()
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
void ModeSelectState::startMode1()
{
    std::cout << "Single Mode Started!" << std::endl;
    Game::getInstance()->pushNewGame();
}

void ModeSelectState::startMode2()  
{
    std::cout << "Multi Mode Started!" << std::endl;
    Game::getInstance()->pushLobby();
}

void ModeSelectState::startMode3()
{
    std::cout << "Challenge Mode Started!" << std::endl;
    Game::getInstance()->pushChallengeMenu();  // 새로 추가할 함수
}

void ModeSelectState::goBack()
{
    Game::getInstance()->popState();
}
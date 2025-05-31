#include "menustate.hpp"
#include "game.hpp"
#include "config.hpp"

MenuState::MenuState(InputManager *manager)
    : State(manager)
{
    title_text = nullptr;
}

MenuState::~MenuState()
{
    exit();
}

void MenuState::initialize()
{
    // Title 텍스처 생성
    title_text = new Texture();
    title_text->loadFromText("PixelTetris!", Game::getInstance()->mRenderer->bigFont, config::default_text_color);

    // 기존 버튼들 정리
    for (auto button : mButtons) {
        delete button;
    }
    mButtons.clear();

    // 텍스트 버튼들 생성 - 이미지 파일 대신 텍스트 사용
    int buttonWidth = 200;
    int buttonHeight = 50;
    int centerX = (config::logical_window_width - buttonWidth) / 2;
    
    // Button(텍스트, 콜백함수, x좌표, y좌표, 너비, 높이)
    mButtons.push_back(new Button("PLAY", &Game::pushModeSelect, centerX, 130, buttonWidth, buttonHeight));
    mButtons.push_back(new Button("OPTIONS", &Game::pushOptions, centerX, 190, buttonWidth, buttonHeight));
    mButtons.push_back(new Button("EXIT", &Game::goBack, centerX, 250, buttonWidth, buttonHeight));
    
    index = 0;
}

void MenuState::exit()
{
    for (auto button : mButtons)
        delete button;
    mButtons.clear();

    if (title_text)
    {
        delete title_text;
        title_text = nullptr;
    }
}

void MenuState::run()
{
    update();
    draw();
}

void MenuState::update()
{
    while (mInputManager->pollAction() != 0)
    {
        switch (mInputManager->getAction())
        {
        case Action::move_up:
            if (index > 0)
                --index;
            break;
        case Action::move_down:
            if (index < mButtons.size() - 1)
                ++index;
            break;
        case Action::select:
            mButtons[index]->callbackFunction();
            break;
        case Action::back:
            Game::getInstance()->popState();
            break;
        default:
            break;
        }
    }
}

void MenuState::draw()
{
    Game::getInstance()->mRenderer->clearScreen();

    if (title_text)
    {
        title_text->renderCentered(config::logical_window_width / 2, 50);
    }

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
            mButtons[index]->getHeight()};

        SDL_SetRenderDrawBlendMode(Game::getInstance()->mRenderer->mSDLRenderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(Game::getInstance()->mRenderer->mSDLRenderer, 255, 255, 255, config::transparency_alpha - 20);
        SDL_RenderFillRect(Game::getInstance()->mRenderer->mSDLRenderer, &highlight_box);
        SDL_SetRenderDrawBlendMode(Game::getInstance()->mRenderer->mSDLRenderer, SDL_BLENDMODE_NONE);
    }

    Game::getInstance()->mRenderer->updateScreen();
}
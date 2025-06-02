#ifndef INVISIBLECHALLENGESTATE_HPP
#define INVISIBLECHALLENGESTATE_HPP

#include "gamestate.hpp"
#include "texture.hpp"

class InvisibleChallengeState : public GameState
{
public:
    InvisibleChallengeState(InputManager *manager);
    ~InvisibleChallengeState();
    
    void initialize() override;
    void run() override;
    void draw() override;
    void checkState() override;

private:
    float mPlacedBlockAlpha;
    unsigned long long mLastPlaceTime;
    
    enum InvisibleDifficulty { EASY, NORMAL, HARD };
    InvisibleDifficulty mDifficulty;
    
    Texture *difficultyTexture;
    Texture *hintTexture;
    Texture *finalResultTexture;
    
    bool challengeCompleted;
    int blocksPlaced;
    unsigned long long gameStartTime;
    bool mShowHints;
    unsigned long long mHintStartTime;
    
    static const float FADE_DELAY;
    static const float EASY_MAX_ALPHA;
    static const float NORMAL_MAX_ALPHA;
    static const float HARD_MAX_ALPHA;
    static const float HINT_DISPLAY_TIME;
    
    void updateFade();
    void updateDifficulty();
    void showHint(const std::string& message);
    void endChallenge();
    void drawInvisibleBoard();
    void drawUI();
    void drawFinalResults();
    void cleanupInvisibleTextures();
    float getTargetAlpha();
    std::string getDifficultyName();
};

#endif
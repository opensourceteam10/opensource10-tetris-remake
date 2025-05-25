#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

#include <SDL.h>

#include "renderer.hpp"

class Renderer;

class Texture
{
public:
    Texture ();
    ~Texture ();
    void free ();

    bool loadFromImage (std::string path);
    bool loadFromText (std::string text, TTF_Font *font, SDL_Color text_color);
    void render (int x, int y, SDL_Rect *clip = nullptr);
    void render(int x, int y, int w, int h); // 강제 사이즈 렌더
    void render(SDL_Rect dstRect);  // 맨 아래쯤에 추가


    void renderCentered (int x, int y);
    void setAlphaMode (Uint8 alpha);
    

    int getWidth ();
    int getHeight ();
    
private:
    SDL_Texture *mTexture;
    int width;
    int height;
};

#endif // TEXTURE_HPP

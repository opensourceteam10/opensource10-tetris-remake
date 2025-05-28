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

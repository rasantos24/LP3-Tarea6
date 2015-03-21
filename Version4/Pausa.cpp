#include "Pause.h"

Pause::Pause(SDL_Renderer* renderer)
{
    pausa = IMG_LoadTexture(renderer,"pause.png");
    SDL_QueryTexture(pausa, NULL, NULL, &rect.w, &rect.h);
    rect.x = 150;
    rect.y = 50;
}

void Pause::draw(SDL_Renderer* renderer){
    SDL_RenderCopy(renderer, pausa, NULL, &rect);
}

Pause::~Pause()
{
    //dtor
}

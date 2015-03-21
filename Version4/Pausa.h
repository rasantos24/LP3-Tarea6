#ifndef PAUSE_H
#define PAUSE_H
#include<SDL.h>
#include<SDL_image.h>
using namespace std;

class Pause
{
    public:
        Pause(SDL_Renderer* renderer);
        SDL_Texture *pausa;
        SDL_Rect rect;
        void draw(SDL_Renderer* renderer);
        virtual ~Pause();
    protected:
    private:
};

#endif // PAUSE_H

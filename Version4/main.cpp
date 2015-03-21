
#include<SDL.h>
#include<SDL_image.h>
#include<iostream>
#include<vector>
#include<list>
#include "Sho.h"
#include "EnemigoAzul.h"
#include "EnemigoVerde.h"
#include "EnemigoRojo.h"
#include <stdlib.h>
#include <math.h>

using namespace std;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event Event;
SDL_Texture *background;
SDL_Rect rect_background;
int opcion=0, Puntaje=0, marca=0, nm;

void loopJuego()
{
    cout<<"Puntos: "+Puntaje<<endl;
    //Init textures
    int w=0,h=0;
    background = IMG_LoadTexture(renderer,"fondito.png");
    SDL_QueryTexture(background, NULL, NULL, &w, &h);
    rect_background.x = 0;
    rect_background.y = 0;
    rect_background.w = w;
    rect_background.h = h;


    list<Personaje*> personajes;
    personajes.push_back(new Sho(renderer,&personajes));
    personajes.push_back(new EnemigoVerde(renderer,&personajes));
    personajes.push_back(new EnemigoRojo(renderer,&personajes));

    //Main Loop
    int frame=0;
    int animacion_sho = 0;
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                exit(0);
            }
            if(Event.type == SDL_KEYDOWN)
            {
                if(Event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return;
                }
            }
        }

        if(frame%1000==0)
        {
            Personaje *p = new EnemigoAzul(renderer,&personajes);
            personajes.push_back(p);
        }

        for(list<Personaje*>::iterator p=personajes.begin();
                p!=personajes.end();
                p++)
            (*p)->act();

        SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255);

        // Clear the entire screen to our selected color.
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, background, NULL, &rect_background);

        for(list<Personaje*>::iterator p=personajes.begin();
                p!=personajes.end();
                p++)
            (*p)->draw(renderer);

        for(list<Personaje*>::iterator p=personajes.begin();
                p!=personajes.end();
                p++)
            if((*p)->muerto)
            {
                personajes.erase(p);
                break;
            }

        SDL_RenderPresent(renderer);

        frame++;
    }
}

class MenuButton
{
public:
    SDL_Texture *selected;
    SDL_Texture *unselected;
    SDL_Rect rect;
    bool is_selected;
    MenuButton(string selected, string unselected, int x, int y)
    {
        this->selected = IMG_LoadTexture(renderer,selected.c_str());
        this->unselected = IMG_LoadTexture(renderer,unselected.c_str());
        SDL_QueryTexture(this->selected, NULL, NULL, &rect.w, &rect.h);
        rect.x = x;
        rect.y = y;
        is_selected = false;
    }

    void render()
    {
        if(is_selected)
            SDL_RenderCopy(renderer,selected,NULL,&rect);
        else
            SDL_RenderCopy(renderer,unselected,NULL,&rect);
    }

    void select()
    {
        is_selected = true;
    }

    void unselect()
    {
        is_selected = false;
    }
};

void Instrucciones1()
{
    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                exit(0);
            }
            if(Event.type == SDL_KEYDOWN)
            {
                if(Event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return;
                }
            }
        }
        opcion = 2;
        int w=0,h=0;
        SDL_Texture *instrucciones = IMG_LoadTexture(renderer,"Instrucciones.png");
        SDL_Rect instru;
        SDL_QueryTexture(instrucciones, NULL, NULL, &w, &h);
        instru.x = 0;
        instru.y = 0;
        instru.w = w;
        instru.h = h;
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,instrucciones,NULL,&instru);
        SDL_RenderPresent(renderer);
    }
}

void mainMenu()
{
    int opcion = 1;
    SDL_Texture *menu_fondo = IMG_LoadTexture(renderer,"MenuPrincipal.png");
    SDL_Rect menu_rect;
    vector<MenuButton*>buttons;
    buttons.push_back(new MenuButton("Boton1.png","Boton1-1.png",350,250));
    buttons.push_back(new MenuButton("Boton2.png","Boton2-2.png",350,400));
    buttons.push_back(new MenuButton("Boton3.png","Boton3-3.png",350,550));
    SDL_QueryTexture(menu_fondo, NULL, NULL, &menu_rect.w, &menu_rect.h);
    menu_rect.x = 0;
    menu_rect.y = 0;

    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                return;
            }
            if(Event.type == SDL_KEYDOWN)
            {
                if(Event.key.keysym.sym == SDLK_3)
                {
                    exit(0);
                }
                if(Event.key.keysym.sym == SDLK_2)
                {
                    Instrucciones1();
                }
                if(Event.key.keysym.sym == SDLK_1)
                {
                    loopJuego();
                }
                if(Event.key.keysym.sym == SDLK_ESCAPE)
                {
                    Puntaje=0;
                    return;
                }
                if(Event.key.keysym.sym == SDLK_DOWN)
                {
                    opcion++;
                    if(opcion > 3)
                        opcion = 3;
                }
                if(Event.key.keysym.sym == SDLK_UP)
                {
                    opcion--;
                    if(opcion < 1)
                        opcion = 1;
                }
                if(Event.key.keysym.sym == SDLK_RETURN)
                {
                    switch(opcion)
                    {
                        case 1:
                            loopJuego();
                        break;
                        case 2:
                            Instrucciones1();
                        break;
                        case 3:
                            exit(0);
                        break;
                    }
                }
            }
        }

        SDL_RenderCopy(renderer,menu_fondo,NULL,&menu_rect);


        for(int i=0;i<buttons.size();i++)
        {
            buttons[i]->unselect();
        }
        buttons[opcion-1]->select();
        for(int i=0;i<buttons.size();i++)
        {
            buttons[i]->render();
        }

        SDL_RenderPresent(renderer);
    }
}

int main( int argc, char* args[] )
{
    //Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return 10;
    }
    //Creates a SDL Window
    if((window = SDL_CreateWindow("Tarea 6 del Laboratorio", 100, 100, 1080/*WIDTH*/, 720/*HEIGHT*/, SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC)) == NULL)
    {
        return 20;
    }
    //SDL Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
    if (renderer == NULL)
    {
        std::cout << SDL_GetError() << std::endl;
        return 30;
    }

    ifstream in("Puntos.txt");
    int nm;
    in>>nm;
    marca = nm;
    cout<<marca<<endl;
    in.close();

    mainMenu();

	return 0;
}

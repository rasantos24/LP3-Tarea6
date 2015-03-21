#include "EnemigoAzul.h"
#include <stdlib.h>
#include <math.h>

EnemigoAzul::EnemigoAzul(SDL_Renderer* renderer, list<Personaje*> *personajes)
{
    ifstream in("Personaje_Azul.txt");

    if(!in)
    {
        cout<<"Error no se encontro el archivo!"<<endl;
        return;
    }

    string animacion;
    while(in>>animacion)
    {
        string llave;
        in>>llave;
        string path;
        in>>path;
        mapa_texturas[enumStringToInt(animacion)] = new vector<SDL_Texture*>();
        while(path!="}")
        {
            mapa_texturas[enumStringToInt(animacion)]->push_back(IMG_LoadTexture(renderer,path.c_str()));
            in>>path;
        }
    }

    estado_actual = DERECHA;

    rect.x = 100;
    rect.y = 350;

    estado = INICIO;
    frame_estado = 0;

    init(renderer,personajes);
}

EnemigoAzul::~EnemigoAzul()
{
    //dtor
}

int EnemigoAzul::estadoRandom()
{
    Personaje* sho = NULL;
    for(list<Personaje*>::iterator i = personajes->begin();
        i!=personajes->end();
        i++)
    {
        if((*i)->soySho())
        {
            sho = (*i);
            if(colision(sho->hitbox))
            {
                return true;
            }
        }
    }
    if(sho!=NULL)
    {
        int mi_random = rand()%2;

        if(mi_random==0)
        {
            if(sho->rect.y>rect.y)
                return ABAJO;

            if(sho->rect.y<rect.y)
                return ARRIBA;
        }
        else
        {
            if(sho->rect.x<rect.x)
            {
                return IZQUIERDA;
            }

            if(sho->rect.x>rect.x)
                return DERECHA;
        }
    }
}

bool EnemigoAzul::estoyCerca()
{
    Personaje* sho = NULL;
    for(list<Personaje*>::iterator i = personajes->begin();
        i!=personajes->end();
        i++)
    {
        if((*i)->soySho())
        {
            sho = (*i);
            if(colision(sho->hitbox))
            {
                return true;
            }
        }
    }


    return false;
}

void EnemigoAzul::cambiarEstado(int estado)
{
    this->estado = estado;
    frame_estado = 0;
}

void EnemigoAzul::act()
{
//    if(frame%100==1)
//        atacando = true;
//    else
//        atacando = false;

    frame_estado++;
    switch(estado)
    {
        case INICIO:
            atacando = false;
            if(estoyCerca())
                cambiarEstado(CERCA);
            else
                cambiarEstado(LEJOS);
        break;

        case CERCA:
            estado = ATACANDO_STARTUP;
            if(orientacion=='r')
                setAnimacion(ANIMACION_ATACANDO_STARTUP_RIGHT);
            else
                setAnimacion(ANIMACION_ATACANDO_STARTUP_LEFT);
        break;

        case ATACANDO_STARTUP:
            if(frame_estado%200==0)
            {
                atacando = true;
                cambiarEstado(ATACANDO_ACTIVE);
                if(orientacion=='r')
                    setAnimacion(ANIMACION_ATACANDO_ACTIVE_RIGHT);
                else
                    setAnimacion(ANIMACION_ATACANDO_ACTIVE_LEFT);
            }
        break;

        case ATACANDO_ACTIVE:
            if(frame_estado%200==0)
            {
                atacando = false;
                cambiarEstado(ATACANDO_RECOVERY);
                if(orientacion=='r')
                    setAnimacion(ANIMACION_ATACANDO_RECOVERY_RIGHT);
                else
                    setAnimacion(ANIMACION_ATACANDO_RECOVERY_LEFT);
            }
        break;

        case ATACANDO_RECOVERY:
            if(frame_estado%200==0)
            {
                cambiarEstado(INICIO);
                setAnimacion(ANIMACION_IDLE_RIGHT);
            }
        break;

        case LEJOS:
            estado = estadoRandom();
            switch(estado)
            {
                case ARRIBA:
                    setAnimacion(ANIMACION_WALKING_RIGHT);
                    orientacion='r';
                break;
                case ABAJO:
                    setAnimacion(ANIMACION_WALKING_LEFT);
                    orientacion='l';
                break;
                case DERECHA:
                    setAnimacion(ANIMACION_WALKING_RIGHT);
                    orientacion='r';
                break;
                case IZQUIERDA:
                    setAnimacion(ANIMACION_WALKING_LEFT);
                    orientacion='l';
                break;
            }

        break;

        case IZQUIERDA:
            rect.x--;
            if(frame_estado%100==0)
                cambiarEstado(INICIO);
        break;

        case DERECHA:
            rect.x++;
            if(frame_estado%100==0)
                cambiarEstado(INICIO);
        break;

        case ARRIBA:
            rect.y--;
            if(frame_estado%100==0)
                cambiarEstado(INICIO);
        break;

        case ABAJO:
            rect.y++;
            if(frame_estado%100==0)
                cambiarEstado(INICIO);
        break;

        case PARADO:
            if(frame_estado%50==0)
                cambiarEstado(INICIO);
        break;
    }

//    this->estado_actual = estado;

    attackCheck();
}




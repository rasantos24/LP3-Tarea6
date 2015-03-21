#include "Personaje.h"

Personaje::Personaje()
{
    hitbox.w = 150;
    hitbox.h = 100;
    atacando = false;
    muerto = false;
}

Personaje::~Personaje()
{
    //dtor
}

void Personaje::draw(SDL_Renderer* renderer)
{
    vector<SDL_Texture*> *vector_textura_actual_temp = mapa_texturas[animacion_actual];
    SDL_Texture* textura_actual_temp = (*vector_textura_actual_temp)[textura_actual_int];
    SDL_QueryTexture( textura_actual_temp, NULL, NULL, &rect.w, &rect.h);
    SDL_RenderCopy(renderer, textura_actual_temp, NULL, &rect);
    if(frame%100==0)
    {
        textura_actual_int++;
        if(textura_actual_int>=(*vector_textura_actual_temp).size())
        {
            textura_actual_int=0;
        }
    }
    hitbox.x = rect.x + rect.w/2 - hitbox.w/2;
    hitbox.y = rect.y + rect.h - hitbox.h/2;
    SDL_RenderCopy(renderer, hitbox_azul, NULL, &hitbox);
    if(atacando)
        SDL_RenderCopy(renderer, hitbox_roja, NULL, &hitbox);
    frame++;
}

void Personaje::init(SDL_Renderer* renderer, list<Personaje*> *personajes)
{
    frame = 0;
    textura_actual_int = 0;
    hitbox_azul = IMG_LoadTexture(renderer,"hitbox/azul.png");
    hitbox_roja = IMG_LoadTexture(renderer,"hitbox/roja.png");

    this->personajes = personajes;

    muerto = false;
    animacion_actual=ANIMACION_IDLE_RIGHT;
    orientacion='r';
}

bool Personaje::colision(SDL_Rect param)
{
    if(param.x + param.w < hitbox.x
            || param.x > hitbox.x + hitbox.w
            || param.y + param.h < hitbox.y
            || param.y > hitbox.y + hitbox.h)
        return false;
    else
        return true;
}

void Personaje::attackCheck()
{
    if(atacando)
    {
        //detecto colision con los demas personajes
        list<Personaje*>::iterator i;
        for(i= personajes->begin();
                i!=personajes->end();
                i++)
        {
            if((*i)!=this)
            {
                if(colision((*i)->hitbox))
                {
                    (*i)->muerto = true;
                }
            }
        }
    }
}

void Personaje::setAnimacion(int nombre)
{
    if(this->animacion_actual != nombre)
    {
        this->textura_actual_int = 0;


        vector<SDL_Texture*> *vector_textura_actual_temp = mapa_texturas[nombre];

        SDL_Texture* textura_actual_temp = (*vector_textura_actual_temp)[textura_actual_int];

        int w_ant = rect.w;
        int h_ant = rect.h;

        SDL_QueryTexture( textura_actual_temp, NULL, NULL, &rect.w, &rect.h);

        rect.x -= (rect.w-w_ant)/2;
        rect.y -= (rect.h-h_ant);

    }

    this->animacion_actual = nombre;
}

bool Personaje::soySho()
{
    return false;
}

int Personaje::enumStringToInt(string nombre)
{
    if(nombre == "ANIMACION_IDLE_RIGHT")
    {
        return 0;
    }
    if(nombre == "ANIMACION_IDLE_LEFT")
    {
        return 1;
    }
    if(nombre == "ANIMACION_WALKING_RIGHT")
    {
        return 2;
    }
    if(nombre == "ANIMACION_WALKING_LEFT")
    {
        return 3;
    }
    if(nombre == "ANIMACION_ATACANDO_STARTUP_RIGHT")
    {
        return 4;
    }
    if(nombre == "ANIMACION_ATACANDO_ACTIVE_RIGHT")
    {
        return 5;
    }
    if(nombre == "ANIMACION_ATACANDO_RECOVERY_RIGHT")
    {
        return 6;
    }
    if(nombre == "ANIMACION_ATACANDO_STARTUP_LEFT")
    {
        return 7;
    }
    if(nombre == "ANIMACION_ATACANDO_ACTIVE_LEFT")
    {
        return 8;
    }
    if(nombre == "ANIMACION_ATACANDO_RECOVERY_LEFT")
    {
        return 9;
    }
    return -1;
}

#ifndef BG_OBJECTS_H_INCLUDED
#define BG_OBJECTS_H_INCLUDED

#include "Objects.h"

///Background objects framework and AI objects


int UNIVEL=0;

//Base background objects framework
class BackgroundObj:public GameObj
{
public:

    virtual const CLASSID getClassID()const
    {
        return ID_BCKG;
    }

    virtual void move()
    {
        area.x-=(xVel);
        area.y+=yVel;
        if(y()+h()>=SCREEN_H)area.y=SCREEN_H-h(),yVel=-yVel;
        if(y()<=0)area.y=0,yVel=-yVel;
        if(isVisible())show();
    }

    const int VelX()const{return xVel;}
    const int VelY()const{return yVel;}

    virtual void show()
    {
        SDL_RenderCopy(Renderer,msurf,NULL,&area);
    }

};

//Nebula object
/*
class Nebula:public BackgroundObj
{
public:

    Nebula()
    {
        SDL_QueryTexture(msurf,NULL,NULL,&area.w,&area.h);
        msurf=nebula_imgs[rand()%4];
        area.x=lrand(128,1408)+bg_dist;
        area.y=lrand(0,600)-100;
        bg_dist+=400;
        collide=0;
        xVel=lrand(1,2)+UNIVEL;
        yVel=0;
    }

};*/

//Star object
class Star:public BackgroundObj
{
public:

    Star()
    {
        msurf=star_img;
        collide=0;
        SDL_QueryTexture(msurf,NULL,NULL,&area.w,&area.h);
        area.x=rand()%(SCREEN_W-w());
        area.y=rand()%(SCREEN_H-h());
        xVel=lrand(1,3)+UNIVEL;
        yVel=0;
    }

    void Reset()
    {
        area.y=rand()%(SCREEN_H-h());
        xVel=lrand(1,3)+UNIVEL;
        area.x=SCREEN_W;
    }

};

//Distant star object
class Distant_Star: public BackgroundObj
{

public:

    Distant_Star()
    {
        msurf=dstar_img;
        collide=0;
        SDL_QueryTexture(msurf,NULL,NULL,&area.w,&area.h);
        area.x=rand()%(SCREEN_W-w());
        area.y=rand()%(SCREEN_H-h());
        xVel=lrand(1,2)+UNIVEL;
        yVel=0;
    }

    void Reset()
    {
        area.y=rand()%(SCREEN_H-h());
        xVel=lrand(1,2)+UNIVEL;
        area.x=SCREEN_W;
    }

};

//Endpoint object
/*class Endpoint: public BackgroundObj
{
    public:

    Endpoint()
    {
        msurf=endp_img;
        collide=0;
        area.x=6500+150*250;
        SDL_QueryTexture(msurf,NULL,NULL,&area.w,&area.h);
        area.y=(SCREEN_H-area.h)/2;
        xVel=7;
        yVel=0;
    }

    //Endpoint-specific function
    void stop()
    {
        xVel=0;
        yVel=0;
    }

};*/

//Asteroid object
class Asteroid: public BackgroundObj
{
public:

    Asteroid()
    {
        msurf=asteroid_imgs[rand()%3];
        collide=1;
        area.x=lrand(2500,2600)+ast_dist;
        ast_dist+=125;
        area.y=lrand(0,600);
        SDL_QueryTexture(msurf,NULL,NULL,&area.w,&area.h);
        xVel=rand()%3+8;
        bool neg=rand()%2;
        yVel=((neg>0)?lrand(0,5):(-lrand(0,5)));
    }

    void Reset()
    {
        msurf=asteroid_imgs[rand()%3];
        collide=1;
        area.x=lrand(2500,2600)+ast_dist;
        area.y=lrand(0,600);
        SDL_QueryTexture(msurf,NULL,NULL,&area.w,&area.h);
        xVel=rand()%3+8;
        bool neg=rand()%2;
        yVel=((neg>0)?lrand(0,5):(-lrand(0,5)));
    }

};

//Object pointers ready for memory allocation and initialization
//Nebula* Nebulae=NULL;
Star* Stars=NULL;
Distant_Star* DistantStars=NULL;
//Endpoint* end=NULL;
Asteroid* Asteroids=NULL;

#endif // BG_OBJECTS_H_INCLUDED

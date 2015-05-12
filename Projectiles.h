#ifndef PROJECTILES_H_INCLUDED
#define PROJECTILES_H_INCLUDED

#include "Objects.h"

class Projectile: public GameObj
{

    public:

    Projectile(int x,int y,int xv,int yv,int nn)
    {
        msurf=projectile_imgs[nn];
        SDL_QueryTexture(msurf,NULL,NULL,&area.w,&area.h);
        area.x=x;
        area.y=y;
        collide=1;
        yVel=yv;
        xVel=xv;
    }

    virtual void move()
    {
        area.x-=xVel;
        area.y+=yVel;
    }

};

#endif // PROJECTILES_H_INCLUDED

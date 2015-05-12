#ifndef CPU_OBJECTS_H_INCLUDED
#define CPU_OBJECTS_H_INCLUDED

#include "BG_Objects.h"

///"AI" objects framework and actual "AI" objects
///"Projectiles" class under development*




//The base "AI" object
class CPU_Player: public GameObj
{
    protected:
    std::list<Projectile> projectiles;

    public:

    virtual const CLASSID getClassID()const
    {
        return ID_BCKG;
    }

    CPU_Player():GameObj()
    {
        collide=1;
    }

    //CPU_Player-specific function - determines the object's behaviour (AI)
    virtual void attack(GameObj& p)=0;

    virtual void move()
    {
        if(x()<SCREEN_W&&(x()+w()>=0))attack(*player);
        area.x-=xVel;
        area.y+=yVel;
        if(y()<=0)area.y=0,yVel=-yVel;
        if(y()+h()>=SCREEN_H)area.y=SCREEN_H-h(),yVel=-yVel;
        std::list<Projectile>::iterator it=projectiles.begin();
        for( ; it!=projectiles.end() ; ++it)it->move();
        while(!projectiles.empty()&&!projectiles.front().isVisible())projectiles.pop_front();
        show();
    }

    virtual void show()
    {
        if(isVisible())SDL_RenderCopy(Renderer,msurf,NULL,&area);
        std::list<Projectile>::iterator it=projectiles.begin();
        for( ; it!=projectiles.end() ; ++it)it->show();

    }

    bool true_collision(const GameObj& obj)const
    {
        if(collision(obj))return true;
        std::list<Projectile>::const_iterator it=projectiles.begin();
        for( ; it!=projectiles.end() ; ++it)if((*it).collision(obj))return true;
        return false;
    }

};

//The first actual "AI" object
class CPU_Player_Iques: public CPU_Player
{

    bool active;

    public:

    CPU_Player_Iques():CPU_Player(),active(0)
    {
        xVel=7;
        yVel=0;
        area.x=lrand(9000,10000)+enemy_dist;
        area.y=lrand(0,600);
        msurf=iques_img;
        SDL_QueryTexture(msurf,NULL,NULL,&area.w,&area.h);
        enemy_dist+=1500;
    }

    void Reset()
    {
        xVel=7;
        yVel=0;
        area.x=lrand(9000,10000)+enemy_dist;
        area.y=lrand(0,600);
        active=false;
    }

    virtual void attack(GameObj& p)
    {
        if(p.x()<x())
        {
            if(p.y()<y()+h()&&y()<p.y()+p.h())
            {
                if(xVel<25)xVel+=2;
                else xVel=25;
                yVel=0;
                active=true;
            }
            else if(xVel>7)xVel-=1;
            if(!yVel&&active)yVel=(p.yV()>>1);
        }
        else if(xVel>7)xVel-=1;
        if(xVel>7)
        {
            projectiles.push_back(Projectile(x()+w(), y()+22, -(3+(rand()&3)), (rand()%2)*(rand()&1?-1:1), projectile_imgs[1],8,0)),
            projectiles.push_back(Projectile(x()+w(), y()+(h()>>1), -(3+(rand()&3)), (rand()%2)*(rand()&1?-1:1), projectile_imgs[1],8,0));
            projectiles.push_back(Projectile(x()+w(), y()+h()-22, -(3+(rand()&3)), (rand()%2)*(rand()&1?-1:1), projectile_imgs[1],8,0));
        }
    }

};

class CPU_Player_Zamat: public CPU_Player
{
  double energy;
  int byVel;
  bool active;

  public:

  CPU_Player_Zamat():CPU_Player(),energy(100)
  {
      active=1;
      msurf=zamat_img;
      SDL_QueryTexture(msurf,NULL,NULL,&area.w,&area.h);
      area.x=lrand(5000,6000)+enemy_dist;
      enemy_dist+=1500;
      xVel=lrand(3,5);
      yVel=byVel=lrand(4,8);
     // mem=yVel;
      area.y=lrand(0,594);
      //energy=100;
  }

  void Reset()
  {
      active=1;
      energy=100;
      area.x=lrand(5000,6000)+enemy_dist;
      xVel=lrand(3,5);
      yVel=byVel=lrand(4,8);
     // mem=yVel;
      area.y=lrand(0,594);
  }

  virtual void attack(GameObj& p)
  {

      if(x()>=p.x()+p.w()&&x()<SCREEN_W)
      if(active&&p.y()<y()+h()&&y()<p.y()+p.h())
      projectiles.push_back(Projectile( x()-7, y()+1, 30, -1, projectile_imgs[0] )),
      projectiles.push_back(Projectile( x()-7, y()+h()-1, 30, 1, projectile_imgs[0] )),
      projectiles.push_back(Projectile( x()-2, y()+5, 30, 1, projectile_imgs[0] )),
      projectiles.push_back(Projectile( x()-2, y()+h()-5, 30, -1, projectile_imgs[0] )),
      energy-=10;
      energy+=0.75;
      if(energy>=100)energy=100,active=1;
      else if(energy<=0)energy=0,active=0;
      yVel=(yVel?yVel/SDL_abs(yVel):(rand()&1?1:-1))*byVel*energy/100.0;
      if(!active)yVel*=energy*energy/10000.0;
      while(!projectiles.empty()&&!projectiles.front().isVisible())projectiles.pop_front();

  }

};

class CPU_Player_Ghillar: public CPU_Player{};

//Pointers ready for memory allocation and initialization
CPU_Player_Iques* Iques=NULL;
CPU_Player_Zamat* Zamat=NULL;

#endif // CPU_OBJECTS_H_INCLUDED

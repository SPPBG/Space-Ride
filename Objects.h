#ifndef OBJECTS_H_INCLUDED
#define OBJECTS_H_INCLUDED

#include "Gamepad.h"
#include <list>

typedef enum{ID_BASE,ID_PRJT,ID_PLAY,ID_BCKG,ID_CPUP}CLASSID;

///Base objects framework and player object

//The most basic object - the rectangle that defines all other objects' areas

//The base game object
class GameObj
{
protected:

    SDL_Texture* msurf;
    int xVel,xAcc; //x-axis speed
    int yVel,yAcc; //y-axis speed
    SDL_Rect area;
    bool collide; //Determines whether the object collides

public:

    virtual const CLASSID getClassID()const
    {
        return ID_BASE;
    }

    //Checks collision with other objects
    bool collision(const GameObj& obj)const
    {
        bool c1=(obj.area.x<area.x+area.w&&area.x<obj.area.x+obj.area.w),
             c2=(obj.area.y<area.y+area.h&&area.y<obj.area.y+obj.area.h);
        return collide&&obj.collide&&c1&&c2;
    }
    //Change the position of the object for the next frame
    virtual void move()=0;

    //Blits the objects surface onto the screen
    virtual void show()
    {
        SDL_RenderCopy(Renderer,msurf,NULL,&area);
    }

    int& x()
    {
        return area.x;
    }
    int& y()
    {
        return area.y;
    }
    int& w()
    {
        return area.w;
    }
    int& h()
    {
        return area.h;
    }

    int& yV()
    {
        return yVel;
    }

    int& xV()
    {
        return xVel;
    }

    int& yA()
    {
        return yAcc;
    }

    int& xA()
    {
        return xAcc;
    }


    bool isVisible()
    {
        return x()<SCREEN_W&&x()+w()>0&&y()<SCREEN_H&&y()+h()>0;
    }

    virtual ~GameObj(){}

    virtual void Reset(){}
};

class Projectile: public GameObj
{

    char life;

    public:

    virtual const CLASSID getClassID()const
    {
        return ID_PRJT;
    }

    Projectile(int x,int y,int xv,int yv,SDL_Texture* tex,char l=-1,char c=1)
    {
        life=l;
        msurf=tex;
        SDL_QueryTexture(msurf,NULL,NULL,&area.w,&area.h);
        area.x=x;
        area.y=y;
        collide=1;
        yVel=yv;
        xVel=xv;
    }

    virtual void move()
    {
        if(life!=0)
        {
            area.x-=xVel;
            area.y+=yVel;
            if(life>0)life--;
        }
        else
        {
            area.x=-w();
        }
    }

};

//The player object
class Player:public GameObj
{
    typedef enum{UP,DOWN,LEFT,RIGHT}DIR;
    char pressed[4];
    std::list<Projectile> trail;
    int death_cnt_down; //Checks the frames remaining 'till the player completely dies
    bool shield; //Determines if the player has turned on the shiel or not
    double energy; //The energy on which the shield depends on

public:

    virtual const CLASSID getClassID()const
    {
        return ID_PLAY;
    }

    bool shielded()const{return shield;}

    Player():GameObj()
    {
        memset(pressed,0,sizeof(pressed));
        msurf=player_img;
        SDL_QueryTexture(msurf,NULL,NULL,&area.w,&area.h);
        shield=0;
        energy=100;
        area.x=0;
        area.y=(SCREEN_H-area.h)/2;
        xVel=yVel=0;
        collide=1;
        death_cnt_down=-1;
    }

    //Handles user input
    void handle_input(SDL_Event input)
    {
        if(input.type==SDL_KEYDOWN)
        {
            if(input.key.keysym.sym==SDLK_p)
            {
                uFMOD_TogglePause();
                if(!Clock.is_paused())Clock.pause();
                else Clock.unpause();
            }
            if(!Clock.is_paused())
            {
                if(input.key.keysym.sym==SDLK_w||input.key.keysym.sym==SDLK_UP)pressed[UP]=1;
                else if(input.key.keysym.sym==SDLK_s||input.key.keysym.sym==SDLK_DOWN)pressed[DOWN]=1;
                else if(input.key.keysym.sym==SDLK_a||input.key.keysym.sym==SDLK_LEFT)pressed[LEFT]=1;
                else if(input.key.keysym.sym==SDLK_d||input.key.keysym.sym==SDLK_RIGHT)pressed[RIGHT]=1;
                else if(input.key.keysym.sym==SDLK_m)uFMOD_TogglePause();
                else if(input.key.keysym.sym==SDLK_PLUS||input.key.keysym.sym==SDLK_KP_PLUS)uFMOD_SetVolume(uFMOD_CVOL+=0.1);
                else if(input.key.keysym.sym==SDLK_MINUS||input.key.keysym.sym==SDLK_KP_MINUS)uFMOD_SetVolume(uFMOD_CVOL-=0.1);
                else if(input.key.keysym.sym==SDLK_SPACE&&energy==100)shield=true;
                else if(input.key.keysym.sym==SDLK_RETURN)
                {
                    ptr++;
                    ptr%=sounds.size();
                    printf("Current track: %s\n",sounds[ptr].c_str());
                    uFMOD_Load(("resources/sound/"+sounds[ptr]).c_str(),FMOD_LOOP_OFF);
                    uFMOD_Play();
                }
                else if(input.key.keysym.sym==SDLK_BACKSPACE)
                {
                    if(ptr)
                    {
                        ptr--;
                        ptr%=sounds.size();
                    }
                    else ptr=sounds.size()-1;
                    printf("Current track: %s\n",sounds[ptr].c_str());
                    uFMOD_Load(("resources/sound/"+sounds[ptr]).c_str(),FMOD_LOOP_OFF);
                    uFMOD_Play();
                }
            }
        }
        if(input.type==SDL_KEYUP)
        {
            if(!pause)
            {
                if(input.key.keysym.sym==SDLK_w||input.key.keysym.sym==SDLK_UP)pressed[UP]=0;
                else if(input.key.keysym.sym==SDLK_s||input.key.keysym.sym==SDLK_DOWN)pressed[DOWN]=0;
                else if(input.key.keysym.sym==SDLK_a||input.key.keysym.sym==SDLK_LEFT)pressed[LEFT]=0;
                else if(input.key.keysym.sym==SDLK_d||input.key.keysym.sym==SDLK_RIGHT)pressed[RIGHT]=0;
            }
        }
    }

    void handle_input(GP_EVENT input)
    {
        if(input.Flags==GPK_DOWN)
        {
            if(input.VirtualKey==VK_PAD_START)
            {
                uFMOD_TogglePause();
                if(!Clock.is_paused())Clock.pause();
                else Clock.unpause();
            }
            if(!Clock.is_paused())
            {
                if(input.VirtualKey==VK_PAD_RTHUMB_DOWNLEFT)pressed[DOWN]=1,pressed[LEFT]=1;
                else if(input.VirtualKey==VK_PAD_RTHUMB_DOWNRIGHT)pressed[DOWN]=1,pressed[RIGHT]=1;
                else if(input.VirtualKey==VK_PAD_RTHUMB_UPLEFT)pressed[UP]=1,pressed[LEFT]=1;
                else if(input.VirtualKey==VK_PAD_RTHUMB_UPRIGHT)pressed[UP]=1,pressed[RIGHT]=1;
                else if(input.VirtualKey==VK_PAD_RTHUMB_UP||input.VirtualKey==VK_PAD_DPAD_UP)pressed[UP]=1;
                else if(input.VirtualKey==VK_PAD_RTHUMB_DOWN||input.VirtualKey==VK_PAD_DPAD_DOWN)pressed[DOWN]=1;
                else if(input.VirtualKey==VK_PAD_RTHUMB_LEFT||input.VirtualKey==VK_PAD_DPAD_LEFT)pressed[LEFT]=1;
                else if(input.VirtualKey==VK_PAD_RTHUMB_RIGHT||input.VirtualKey==VK_PAD_DPAD_RIGHT)pressed[RIGHT]=1;
                else if(input.VirtualKey==VK_PAD_X)uFMOD_TogglePause();
                else if(input.VirtualKey==VK_PAD_RSHOULDER)uFMOD_SetVolume(uFMOD_CVOL+=0.1);
                else if(input.VirtualKey==VK_PAD_RTRIGGER)uFMOD_SetVolume(uFMOD_CVOL-=0.1);
                else if(input.VirtualKey==VK_PAD_A&&energy==100)shield=true;
                else if(input.VirtualKey==VK_PAD_LSHOULDER)
                {
                    ptr++;
                    ptr%=sounds.size();
                    printf("Current track: %s\n",sounds[ptr].c_str());
                    uFMOD_Load(("resources/sound/"+sounds[ptr]).c_str(),FMOD_LOOP_OFF);
                    uFMOD_Play();
                }
                else if(input.VirtualKey==VK_PAD_LTRIGGER)
                {
                    if(ptr)
                    {
                        ptr--;
                        ptr%=sounds.size();
                    }
                    else ptr=sounds.size()-1;
                    printf("Current track: %s\n",sounds[ptr].c_str());
                    uFMOD_Load(("resources/sound/"+sounds[ptr]).c_str(),FMOD_LOOP_OFF);
                    uFMOD_Play();
                }
            }
        }
        if(input.Flags==GPK_UP)
        {
            if(!pause)
            {
                if(input.VirtualKey==VK_PAD_RTHUMB_UP||input.VirtualKey==VK_PAD_DPAD_UP)pressed[UP]=0;
                else if(input.VirtualKey==VK_PAD_RTHUMB_DOWN||input.VirtualKey==VK_PAD_DPAD_DOWN)pressed[DOWN]=0;
                else if(input.VirtualKey==VK_PAD_RTHUMB_LEFT||input.VirtualKey==VK_PAD_DPAD_LEFT)pressed[LEFT]=0;
                else if(input.VirtualKey==VK_PAD_RTHUMB_RIGHT||input.VirtualKey==VK_PAD_DPAD_RIGHT)pressed[RIGHT]=0;
                else if(input.VirtualKey==VK_PAD_RTHUMB_DOWNLEFT)pressed[DOWN]=0,pressed[LEFT]=0;
                else if(input.VirtualKey==VK_PAD_RTHUMB_DOWNRIGHT)pressed[DOWN]=0,pressed[RIGHT]=0;
                else if(input.VirtualKey==VK_PAD_RTHUMB_UPLEFT)pressed[UP]=0,pressed[LEFT]=0;
                else if(input.VirtualKey==VK_PAD_RTHUMB_UPRIGHT)pressed[UP]=0,pressed[RIGHT]=0;
            }
        }
    }

    void CalcVel()
    {
            yAcc=xAcc=0;
            char yDamp=1,xDamp=1;
            if(pressed[UP])yAcc-=area.h/20,yDamp=0;
            if(pressed[DOWN])yAcc+=area.h/20,yDamp=0;
            if(pressed[LEFT])xAcc-=area.w/20,xDamp=0;
            if(pressed[RIGHT])xAcc+=area.w/20,xDamp=0;
            yVel+=yAcc,xVel+=xAcc;
            if(yDamp)yVel/=2;
            if(xDamp)xVel/=2;
            xVel+=(rand()&1?1:-1)*(3*double(100-energy)/100.0);
            yVel+=(rand()&1?1:-1)*(3*double(100-energy)/100.0);
            if(abs(yVel)>area.h/5)yVel=(yVel/abs(yVel))*(area.h/5);
            if(abs(xVel)>area.w/5)xVel=(xVel/abs(xVel))*(area.w/5);
    }

    void CalcEnergy()
    {
            if(shield&&energy>0)energy--;
            if((!energy)&&shield)shield=0;
            if(energy<100&&(!shield))
            {
                if(xAcc||yAcc)energy+=0.09375;
                else energy+=0.125;
            }
            if(energy>100)energy=100;
    }

    //Move according user input and dump current status in BlackBox.txt
    virtual void move()
    {
        if(death_cnt_down<0)
        {
            CalcVel();
            CalcEnergy();
            area.y+=yVel/*+(rand()&1?1:-1)*(xVel?3*double(100-energy)/100.0:0)*/;
            area.x+=xVel/*+(rand()&1?1:-1)*(yVel?3*double(100-energy)/100.0:0)*/;
            if(y()+h()>=SCREEN_H)area.y=SCREEN_H-h(),yVel=0;
            if(y()<=0)area.y=0,yVel=0;
            if(x()+w()>=SCREEN_W)area.x=SCREEN_W-w(),xVel=0;
            if(x()<=0)area.x=0,xVel=0;
            trail.push_back(Projectile(x(), y()+3, (3+(rand()%3)),(rand()%2)*(rand()&1?-1:1),projectile_imgs[1],8,0)),
            trail.push_back(Projectile(x(), y()+h()-3, (3+(rand()%3)), (rand()%2)*(rand()&1?-1:1), projectile_imgs[1],8,0));
            if(xVel>0&&rand()&1)
            trail.push_back(Projectile(x(), y()+3, (3+(rand()%3)), (rand()%2)*(rand()&1?-1:1),projectile_imgs[1],8,0)),
            trail.push_back(Projectile(x(), y()+h()-3, (3+(rand()%3)), (rand()%2)*(rand()&1?-1:1), projectile_imgs[1],8,0));
        }
        else
        {
            std::string dir="resources\\explosions\\player\\",file="a.png";
            file[0]+=death_cnt_down;
            if(msurf!=player_img)SDL_DestroyTexture(msurf);
            msurf=TIMG_Load((dir+file).c_str());
            death_cnt_down++;
        }

        std::list<Projectile>::iterator i;
        for(i=trail.begin();i!=trail.end();i++)i->move();
        while(!trail.empty()&&!trail.front().isVisible())trail.pop_front();
    }

    virtual void show()
    {
        SDL_RenderCopy(Renderer,msurf,NULL,&area);
        if(shield&&!dying())SDL_RenderCopy(Renderer,shield_img,NULL,&area);
        std::list<Projectile>::iterator i;
        for(i=trail.begin();i!=trail.end();i++)i->show();
    }

    bool dead()
    {
        return death_cnt_down>11;
    }

    bool dying()
    {
        return death_cnt_down>-1;
    }

    //Begin death countdown
    void explode()
    {
        if(death_cnt_down<0)
        {
            std::string dir="resources\\explosions\\player\\",file="a.png";
            death_cnt_down=0;
            collide=0;
            SDL_Rect prev=area;
            msurf=TIMG_Load((dir+file).c_str());
            SDL_QueryTexture(msurf,NULL,NULL,&area.w,&area.h);
            prev.x=area.w-prev.w;
            prev.y=area.h-prev.h;
            area.x-=prev.x>>1;
            area.y-=prev.y>>1;
        }
    }

    virtual ~Player(){}
};

Player* player=NULL;

#endif // OBJECTS_H_INCLUDED

#ifndef GAME_LOOPS_H_INCLUDED
#define GAME_LOOPS_H_INCLUDED

#include "CPU_Objects.h"

#include <SDL_ttf.h>

SDL_Event input;

///The main functions of which the game consists

//Create the main menu screen
bool Main_Menu()
{
    uFMOD_Load("helion.ogg",FMOD_LOOP_NORMAL);
    uFMOD_Play();
    SDL_Texture* start_screen=TIMG_Load("resources\\stars.jpg");
    if(!start_screen)
    {
        quit=1;
        printf("Can't load main menu image\n\n");
    }
    SDL_Color col = {255,255,255,255};
    SDL_Surface* tmp=TTF_RenderText_Solid(font,"Press [ENTER] to begin",col);
    SDL_Texture* msg=SDL_CreateTextureFromSurface(Renderer,tmp);
    tmp->clip_rect.x=(SCREEN_W-tmp->w)>>1;
    tmp->clip_rect.y=(SCREEN_H-tmp->h)>>1;
    tmp->clip_rect.w=tmp->w;
    tmp->clip_rect.h=tmp->h;
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer,start_screen,NULL,NULL);
    SDL_RenderCopy(Renderer,msg,NULL,&tmp->clip_rect);
    SDL_FreeSurface(tmp);
    SDL_RenderPresent(Renderer);

    bool res=0;
    while(!res&&!quit)
    {
        while(SDL_PollEvent(&input))
        {
            if(input.type==SDL_JOYDEVICEREMOVED)
            {
                SDL_JoystickClose(js);
                js=NULL;
            }
            else if(!js&&input.type==SDL_KEYDOWN)
            {
                if(input.key.keysym.sym==SDLK_RETURN)res=1;
                else if(input.key.keysym.sym==SDLK_PLUS||input.key.keysym.sym==SDLK_KP_PLUS)uFMOD_SetVolume(uFMOD_GetVolume()+0.1);
                else if(input.key.keysym.sym==SDLK_MINUS||input.key.keysym.sym==SDLK_KP_MINUS)uFMOD_SetVolume(uFMOD_GetVolume()-0.1);
            }
            else if(input.type==SDL_QUIT)quit=1;
            else if(input.type==SDL_JOYDEVICEREMOVED)
            {
                js=SDL_JoystickOpen(0);
            }
        }
        while(test.pullGPEvent(&gpEv))
        {
            if(gpEv.Flags==GPK_DOWN)
            {
                if(gpEv.VirtualKey==VK_PAD_START)res=1;
                else if(gpEv.VirtualKey==VK_PAD_BACK)quit=1;
                else if(gpEv.VirtualKey==VK_PAD_RSHOULDER)uFMOD_SetVolume(uFMOD_CVOL+=0.1);
                else if(gpEv.VirtualKey==VK_PAD_RTRIGGER)uFMOD_SetVolume(uFMOD_CVOL-=0.1);
            }
        }
        uFMOD_Update();
    }

    SDL_DestroyTexture(start_screen);
    SDL_DestroyTexture(msg);

    return 1;
}

//Initialize temporary objects
void init_objs()
{

    player=new Player;

    Iques=new CPU_Player_Iques[40];

    enemy_dist=0;

    Zamat=new CPU_Player_Zamat[40];

    Stars=new Star[16];

    bg_dist=0;

    Asteroids=new Asteroid[250];

    DistantStars=new Distant_Star[128];

    bg_dist=0;

    //Nebulae=new Nebula[100];z

}

//Free the memory allocated for the temporary objects
void free_objs()
{
    delete player;
    delete[] Iques;
    delete[] Zamat;
    delete[] DistantStars;
    delete[] Stars;
    delete[] Asteroids;
    printf("Object memory freed.\n\n");
}

//Move all background and AI objects
void cpu_move()
{
    for(int i=0; i<128; i++)
    {
        DistantStars[i].move();
        if(!DistantStars[i].isVisible())DistantStars[i].Reset();
    }
    //for(int i=0; i<100; i++)Nebulae[i].move();
    for(int i=0; i<16; i++)
    {
        Stars[i].move();
        if(!Stars[i].isVisible())Stars[i].Reset();
    }
    for(int i=0; i<250; i++)
    {
        Asteroids[i].move();
        if(Asteroids[i].w()+Asteroids[i].x()<0)Asteroids[i].Reset();
    }
    for(int i=0; i<40; i++)
    {
        Iques[i].move();
        if(Iques[i].w()+Iques[i].x()<0)Iques[i].Reset();
    }
    for(int i=0; i<40; i++)
    {
        Zamat[i].move();
        if(Zamat[i].w()+Zamat[i].x()<0)Zamat[i].Reset();
    }
}

void handle_collision()
{
    if(!player->dying()&&!player->shielded())
        for(int i=0; i<250; i++)
            if((player->collision(Asteroids[i]))||(Iques[i%40].true_collision(*player))||(Zamat[i%40].true_collision(*player)))
                    {
                        player->explode();
                        break;
                    }
    if(player->dead())quit=true;
}

//The main game loop
void play()
{
    char tbuffer[32]={0};
    if(!load_res())
    {
        printf("Failed to load images!!!\n");
        return;
    }
    quit=0;
    bg_dist=0;
    ast_dist=0;
    enemy_dist=0;

    init_objs();

    Main_Menu();
    Uint32 t;

    printf("Current track: %s\n",sounds[ptr].c_str());
    uFMOD_Load(("resources/sound/"+sounds[ptr]).c_str(),FMOD_LOOP_OFF);
    uFMOD_Play();

    Clock.start();

    while(!quit)
    {

        while(SDL_PollEvent(&input))
        {
            if(input.type==SDL_QUIT)
            {
                quit=true;
                goto end;
            }
            else if(input.type==SDL_JOYDEVICEREMOVED)
            {
                SDL_JoystickClose(js);
                js=NULL;
            }
            else if(input.type==SDL_JOYDEVICEREMOVED)
            {
                js=SDL_JoystickOpen(0);
            }
            if(!js)player->handle_input(input);
        }
        while(test.pullGPEvent(&gpEv))
        {
            if(gpEv.Flags==GPK_DOWN&&gpEv.VirtualKey==VK_PAD_BACK)
            {
                quit=true;
                goto end;
            }
            player->handle_input(gpEv);
        }

        if(!Clock.is_paused())
        {
            SDL_RenderClear(Renderer);
            SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
            FPS.start();

            player->move();
            cpu_move();
            handle_collision();
            player->show();

            t=Clock.get_ticks();
            sprintf(tbuffer,"%.2d:%.2d:%.2d",t/3600000,t/60000,(t/1000)%60);
            SDL_Color col={255,255,255,255};
            SDL_Surface* tmp=TTF_RenderText_Solid(font,tbuffer,col);
            SDL_Texture* msg=SDL_CreateTextureFromSurface(Renderer,tmp);
            tmp->clip_rect.x=tmp->clip_rect.y=0;
            tmp->clip_rect.w=tmp->w;
            tmp->clip_rect.h=tmp->h;
            SDL_RenderCopy(Renderer,msg,NULL,&tmp->clip_rect);
            SDL_FreeSurface(tmp);
            SDL_DestroyTexture(msg);

            SDL_RenderPresent(Renderer);

        }
        uFMOD_Update();
        if(!uFMOD_IsPlaying())
        {
            ptr++;
            ptr%=sounds.size();
            printf("Current track: %s\n",sounds[ptr].c_str());
            uFMOD_Load(("resources/sound/"+sounds[ptr]).c_str(),FMOD_LOOP_OFF);
            uFMOD_Play();
        }

       FPS.regFPS();
    }
    end:
    uFMOD_Stop();
    free_objs();
}

#endif // GAME_LOOPS_H_INCLUDED

///NOW WITH JOYSTICK SUPPORT!!! :D

#include "main.h"
#include <ctime>
#include <windows.h>

int main ( int argc, char** argv )
{
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK);

    srand(time(0));

    freopen("stdout.txt","w",stdout);

    SoundList.open("resources/sound/flist.txt");
    printf("Searching sounds...\n");
    char tmp[257];
    std::string st;
    while(SoundList.good())
    {
        SoundList.getline(tmp,256);
        st=tmp;
        if(st!="")sounds.push_back(st);
    }
    random_shuffle(sounds.begin(),sounds.end());

    uFMOD_Init();
    printf("Current track: %s\n",sounds[ptr].c_str());
    uFMOD_Load(("resources/sound/"+sounds[ptr]).c_str(),FMOD_LOOP_OFF);

    if(SDL_NumJoysticks()==1)js=SDL_JoystickOpen(0);

    SDL_Surface* icon=IMG_Load("icon.png");
    TTF_Init();
    font=TTF_OpenFont("font.ttf",26);

    Window=SDL_CreateWindow("Space Ride",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_W,SCREEN_H,SDL_WINDOW_SHOWN);
    Renderer=SDL_CreateRenderer(Window,-1,SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_ACCELERATED);
    SDL_SetWindowIcon(Window,icon);
    SDL_SetWindowBrightness(Window,0.5);

    SetPriorityClass(GetCurrentProcess(),REALTIME_PRIORITY_CLASS);

    play();

    TTF_CloseFont(font);

    uFMOD_Free();
    unload_res();
    printf("Unloaded all resources.");

    if(js)SDL_JoystickClose(js);
    SDL_DestroyWindow(Window);
    SDL_FreeSurface(icon);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

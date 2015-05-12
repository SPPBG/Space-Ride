#ifndef VAF_H_INCLUDED
#define VAF_H_INCLUDED

#include "Timer.h"
#include "uFMOD.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

///(Global) Variables And Functions

//Player data dump file
std::ofstream BlackBox;
std::ifstream SoundList;

//Game loop controler
bool quit=0,pause=0;

Uint32 offset;

std::vector<std::string> sounds;
int ptr=0;

//File names
const std::string star_files[4]= {
                            "resources\\starblue.png",
                            "resources\\starred.png",
                            "resources\\stargreen.png",
                            "resources\\staryellow.png"};

const std::string nebula_files[4]= {
                              "resources\\nebula1.png",
                              "resources\\nebula2.png",
                              "resources\\nebula3.png",
                              "resources\\nebula4.png"};

const std::string dstar_files[4]= {
                             "resources\\stard1.png",
                             "resources\\stard2.png",
                             "resources\\stard3.png",
                             "resources\\stard4.png"};

const std::string proj_files[2]= {
                         "resources\\projectiles\\1.png",
                         "resources\\projectiles\\2.png"};

//Background surfaces
SDL_Texture* star_img = NULL;
SDL_Texture* nebula_imgs[4]= {0};
SDL_Texture* dstar_img=NULL;

//Player specific surfaces
SDL_Texture* player_img=NULL;
SDL_Texture* shield_img=NULL;

//CPU-controlled object surfaces
SDL_Texture* asteroid_imgs[3]={0};
SDL_Texture* iques_img=NULL;
SDL_Texture* zamat_img=NULL;

//Endpoint surface
SDL_Texture* endp_img=NULL;

//Projectile surfaces
SDL_Texture* projectile_imgs[2]= {0};

//Level generation variables
long ast_dist=0;
long bg_dist=0;
long enemy_dist=0;

//Screen surface and screen data
SDL_Window* Window=NULL;
SDL_Renderer* Renderer=NULL;
SDL_Texture *screen=NULL;
const int SCREEN_W=1280;
const int SCREEN_H=680;
const int BPP=32;

TTF_Font* font=NULL;

int DCP=99;

SDL_Joystick* js=NULL;

//Limited random function
int lrand(int lower,int upper)
{
    return std::rand()%(upper-lower)+lower;
}

SDL_Texture* TIMG_Load(const char* path)
{
    SDL_Surface* tmp=IMG_Load(path);
    SDL_Texture* a=SDL_CreateTextureFromSurface(Renderer,tmp);
    SDL_FreeSurface(tmp);
    return a;
}

//Image resources loader
bool load_res()
{

    BlackBox.open("BlackBox.txt");
    printf("Black box activated...\n");

    player_img=TIMG_Load("resources\\player1.png");
    if(!player_img)return false;
    printf("Loaded Player...\n");

    asteroid_imgs[0]=TIMG_Load("resources\\Asteroid\\Asteroid1.png");
    if(!asteroid_imgs[0])return false;
    printf("Loaded Asteroids(1)...\n");

    asteroid_imgs[1]=TIMG_Load("resources\\Asteroid\\Asteroid2.png");
    if(!asteroid_imgs[1])return false;
    printf("Loaded Asteroids(2)...\n");

    asteroid_imgs[2]=TIMG_Load("resources\\Asteroid\\Asteroid3.png");
    if(!asteroid_imgs[2])return false;
    printf("Loaded Asteroids(3)...\n");

    endp_img=TIMG_Load("resources\\endpoint.png");
    if(!endp_img)return false;
    printf("Loaded Endpoint...\n");

    iques_img=TIMG_Load("resources\\Iques.png");
    if(!iques_img)return false;
    printf("Loaded Enemies...\n");

    zamat_img=TIMG_Load("resources\\Zamat.png");
    if(!zamat_img)return false;
    printf("Loaded Enemies...\n");

    shield_img=TIMG_Load("resources\\shield.png");
    if(!shield_img)return false;
    printf("Loaded Shields...\n");

    star_img=TIMG_Load(star_files[0].c_str());
    if(!star_img)return false;
    printf("Loaded Stars...\n");

    dstar_img=TIMG_Load(dstar_files[0].c_str());
    if(!dstar_img)return false;
    printf("Loaded distant stars.\n\n");

    projectile_imgs[0]=TIMG_Load(proj_files[0].c_str());
    if(!projectile_imgs[0])return false;

    projectile_imgs[1]=TIMG_Load(proj_files[1].c_str());
    if(!projectile_imgs[1])return false;
    printf("Successfully Loaded All.\n\n");

    return true;
}

//Imagery resources unloader
void unload_res()
{
    SDL_DestroyTexture(player_img);
    SDL_DestroyTexture(endp_img);
    SDL_DestroyTexture(iques_img);
    SDL_DestroyTexture(zamat_img);
    SDL_DestroyTexture(shield_img);
    for(int i=0; i<3; i++)SDL_DestroyTexture(asteroid_imgs[i]);
    SDL_DestroyTexture(star_img);
    SDL_DestroyTexture(dstar_img);
    SDL_DestroyTexture(shield_img);
    //for(int i=0; i<4; i++)SDL_DestroyTexture(nebula_imgs[i]);
    for(int i=0; i<2; i++)SDL_DestroyTexture(projectile_imgs[i]);
}


#endif // VAF_H_INCLUDED

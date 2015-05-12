#ifndef UFMOD_H_INCLUDED
#define UFMOD_H_INCLUDED

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fmod.h>
#include <fmod_errors.h>

static FMOD_SYSTEM* uSys;
static FMOD_RESULT uStatus;

static FMOD_SOUND *uSound=NULL;
static FMOD_CHANNEL *uChannel=NULL;
static FMOD_BOOL uActive=0,uLoaded=0,uPaused=0;

static char funcpath[128];
static unsigned int procs=0;

static const float uFMOD_BVOL=0.5f;
static float uFMOD_CVOL=uFMOD_BVOL;

enum uFMOD_PMODE{uFMOD_STREAM,uFMOD_SOUND};

static void uFMOD_CheckStatus()
{
    if(uStatus!=FMOD_OK)
    {
        printf("FMOD error ( %d ) has occured from this function: %s",uStatus,funcpath);
        printf("\nAdditonal info: %s\n",FMOD_ErrorString(uStatus));
        printf("Callbacks within caller: %d\n",procs);
        exit(uStatus);
        return;
    }
    procs++;
}

static void uFMOD_Init()
{
    sprintf(funcpath,"uFMOD_Init()");
    uLoaded=uActive=false;
    uStatus=FMOD_System_Create(&uSys);
    uFMOD_CheckStatus();
    uStatus=FMOD_System_Init(uSys,32,FMOD_INIT_NORMAL,NULL);
    uFMOD_CheckStatus();
}

static void uFMOD_Free()
{
    sprintf(funcpath,"uFMOD_Free()");
    FMOD_BOOL chk=0;
    if(uChannel)FMOD_Channel_IsPlaying(uChannel,&chk);
    if(chk)
    {
        uStatus=FMOD_Channel_Stop(uChannel);
        uFMOD_CheckStatus();
    }
    if(uLoaded)
    {
        uStatus=FMOD_Sound_Release(uSound);
        uFMOD_CheckStatus();
    }
    uStatus=FMOD_System_Release(uSys);
    uFMOD_CheckStatus();
}

static void uFMOD_Load(const char* fpath,FMOD_MODE mode,uFMOD_PMODE pmode=uFMOD_STREAM)
{
    sprintf(funcpath,"uFMOD_Load(%s,%d,%d)",fpath,mode,pmode);
    FMOD_BOOL chk=0;
    if(uChannel)FMOD_Channel_IsPlaying(uChannel,&chk);
    if(chk)
    {
        uStatus=FMOD_Channel_Stop(uChannel);
        uFMOD_CheckStatus();
    }
    if(uSound)
    {
        uStatus=FMOD_Sound_Release(uSound);
        uFMOD_CheckStatus();
        uSound=NULL;
    }
    switch(pmode)
    {
        case uFMOD_STREAM: uStatus=FMOD_System_CreateStream(uSys,fpath,mode,NULL,&uSound);break;
        case uFMOD_SOUND: uStatus=FMOD_System_CreateSound(uSys,fpath,mode,NULL,&uSound);break;
    }
    uFMOD_CheckStatus();
}

static void uFMOD_Update()
{
    sprintf(funcpath,"uFMOD_Update()");
    uStatus=FMOD_System_Update(uSys);
    uFMOD_CheckStatus();
}

static void uFMOD_Play()
{
    sprintf(funcpath,"uFMOD_Play()");
    if(!uSound)return;
    uStatus=FMOD_System_PlaySound(uSys,FMOD_CHANNEL_REUSE,uSound,false,&uChannel);
    uFMOD_CheckStatus();
    uStatus=FMOD_Channel_SetVolume(uChannel,uFMOD_CVOL);
    uFMOD_CheckStatus();
}

static void uFMOD_SetVolume(float v)
{
    sprintf(funcpath,"uFMOD_SetVolume(%f)",v);
    if(v<0)v=0;
    else if(v>1)v=1;
    uStatus=FMOD_Channel_SetVolume(uChannel,v);
    uFMOD_CheckStatus();
}

static float uFMOD_GetVolume()
{
    float r;
    sprintf(funcpath,"uFMOD_GetVolume()");
    uStatus=FMOD_Channel_GetVolume(uChannel,&r);
    uFMOD_CheckStatus();
    return r;
}

static void uFMOD_TogglePause()
{
    sprintf(funcpath,"uFMOD_TogglePause()");
    uPaused=!uPaused;
    uStatus=FMOD_Channel_SetPaused(uChannel,uPaused);
    uFMOD_CheckStatus();
}

static void uFMOD_Stop()
{
    sprintf(funcpath,"uFMOD_Stop()");
    FMOD_BOOL chk;
    FMOD_Channel_IsPlaying(uChannel,&chk);
    if(chk)
    {
        uStatus=FMOD_Channel_Stop(uChannel);
        uFMOD_CheckStatus();
    }
}

static void uFMOD_ReleaseSound()
{
    sprintf(funcpath,"uFMOD_ReleaseSound()");
    if(uSound)
    {
        uStatus=FMOD_Sound_Release(uSound);
        uFMOD_CheckStatus();
        uSound=NULL;
    }
}

static void uFMOD_GetLenPos(unsigned int* len,unsigned int* pos)
{
    FMOD_Sound_GetLength(uSound,len,FMOD_TIMEUNIT_MS);
    FMOD_Channel_GetPosition(uChannel,pos,FMOD_TIMEUNIT_MS);
}

static bool uFMOD_IsPlaying()
{
    FMOD_BOOL chk;
    FMOD_Channel_IsPlaying(uChannel,&chk);
    return chk;
}

#endif // UFMOD_H_INCLUDED

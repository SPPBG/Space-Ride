#ifndef XINPUT_H_INCLUDED
#define XINPUT_H_INCLUDED

#include <XInput.h>
#include <windows.h>
#include "VAF.h"

#define GPK_DOWN   XINPUT_KEYSTROKE_KEYDOWN
#define GPK_UP     XINPUT_KEYSTROKE_KEYUP
#define GPK_REPEAT XINPUT_KEYSTROKE_REPEAT

typedef XINPUT_KEYSTROKE GP_EVENT;
typedef XINPUT_STATE GP_STATE;

class Gamepad
{
    GP_STATE gpState;
    DWORD gpNum;
    BYTE gpActive;

public:

    Gamepad(int num=1)
    {
        gpActive=1;
        memset(&gpState,0,sizeof(gpState));
        gpNum=num-1;
    }

    ~Gamepad()
    {
        gpNum=-1;
    }

    Gamepad setGPNum(int num)
    {
        gpNum=num;
        return *this;
    }

    DWORD getGPNum()const
    {
        return gpNum;
    }

    bool isGPActive()
    {
        return XInputGetState(gpNum,&gpState)==ERROR_SUCCESS;
    }

    const GP_STATE* getGPState()
    {
        memset(&gpState,0,sizeof(gpState));
        if(!isGPActive())return NULL;
        return &gpState;
    }


    void vibrate(int leftVal, int rightVal)
    {
        // Create a Vibraton State
        XINPUT_VIBRATION Vibration;

        // Zeroise the Vibration
        memset(&Vibration, 0 ,sizeof(XINPUT_VIBRATION));

        // Set the Vibration Values
        Vibration.wLeftMotorSpeed = leftVal;
        Vibration.wRightMotorSpeed = rightVal;

        // Vibrate the controller
        XInputSetState(gpNum, &Vibration);
    }

    GP_EVENT* pullGPEvent(GP_EVENT* e)
    {
        if(!isGPActive())return NULL;
        DWORD res=XInputGetKeystroke(gpNum,0,e);
        if(res==ERROR_EMPTY)return NULL;
        return e;
    }


};

Gamepad test;
GP_EVENT gpEv;


#endif // XINPUT_H_INCLUDED

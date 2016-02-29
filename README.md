#Space Ride

Space Ride is a rather simplistic side scroller game, written entirely in C++ (with the SDL, FMOD libraries and the DirectX SDK).
The goal is to kill as much time as possible in a single run of the game when you're bored.

##Compilation

The source can be compiled by using **MinGW** (GCC for Windows) with or without the **Code::Blocks** IDE with the following compiler flags:

*-Wall  -O2  -fexpensive-optimizations -Os -O3 -O2 -O1 -O -Wall -ansi -Wunknown-pragmas -Wall -std=c++11*

It should be noted that it requires **SDL2**, **FMOD** and **DXSDK** to be linked.

##Gameplay

The player controls an unarmed space ship on a trip through a very dangerous part of the galaxy - asteroids and pirates galore.
Since the ship is not very well protected all collisions are lethal, so evade other objects if you can. If you get into trouble you can use the ship's only defensive mechanism - a phase shield which makes you immaterial for a short time, but makes the ship very unstable afterwards.

A timer in the upper left corner of the screen will show you how much time you've survived.

**PS:** The previous owner of the ship has kindly supplied you with a jukebox loaded with his music collection.

##Controls

* **W, A, S, D** - Move up, left, down, right. (Alternatively use arrows).
* **Spacebar** - Phase shield (2-3 sec).
* **P** - Pause / Unpause.

* **M** - Pause / Unpause sound.
* **+, -** - Increase / Decrease volume.
* **Enter, Backspace** - Next / Previous track.

Controls configuration for a gamepad is also available.

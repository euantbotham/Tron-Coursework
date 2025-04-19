#pragma once
#include "header.h"
#include "BaseEngine.h"
#include "Psyeb10States.h"
#include "Psyeb10TileManager.h"


class Psyeb10States;

class Psyeb10Engine :
    public BaseEngine
{
public:
    Psyeb10Engine();
    void virtSetupBackgroundBuffer() override;
    void virtDrawStringsOnTop() override;
    int virtInitialiseObjects() override;
    void virtMouseDown(int iButton, int iX, int iY) override;
    void resetGame();
    void virtMainLoopDoBeforeUpdate() override;
    void virtCleanUp() override;
    void setState(Psyeb10States* state, bool keepOldState, bool initState);
    void setState();
    Psyeb10TileManager* getTileManager();
    Psyeb10States* getState(int stateNum);
    void copyAllBackgroundBuffer()override;
    void virtKeyDown(int iKeyCode) override;
    const int tronBlue = 0x57E2F9;
    void setBackgroundSurface(DrawingSurface* surface);
protected:
    Psyeb10States* currentState;
    // Used to store a second state if needed
    Psyeb10States* secondState;
};


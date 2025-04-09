#pragma once
#include "BaseEngine.h"
#include "Psyeb10TileManager.h"
#include "Psyeb10States.h"

class gameState;

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
    void virtMainLoopPreUpdate();
    void virtCleanUp() override;
    Psyeb10TileManager* getTileManager();
protected:
    gameState* currentState;
};


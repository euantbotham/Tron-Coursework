#pragma once
#include "BaseEngine.h"
#include "Psyeb10TileManager.h"
#include "MainCharacter.h"
class MainCharacter;

class Psyeb10Engine :
    public BaseEngine
{
    friend class MainCharacter;
public:
    void virtSetupBackgroundBuffer() override;
    void virtDrawStringsOnTop() override;
    int virtInitialiseObjects() override;
    void resetGame();
protected:
    Psyeb10TileManager tm;
    const int gridstartX = 350;
    const int gridstartY = 100;
    const int gridEndX = 950;
    const int gridEndY = 700;
};


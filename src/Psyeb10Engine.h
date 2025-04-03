#pragma once
#include "BaseEngine.h"
#include "Psyeb10TileManager.h"
#include "MainCharacter.h"
#include "Psyeb10Bike.h"


//TODO remove other classes when fully subclassed
class MainCharacter;
//class Psyeb10Enemy;
class Psyeb10Bike;
class Psyeb10Engine :
    public BaseEngine
{
    friend class MainCharacter;
    //friend class Psyeb10Enemy;
    friend class Psyeb10Bike;
public:
    Psyeb10Engine();
    void virtSetupBackgroundBuffer() override;
    void virtDrawStringsOnTop() override;
    int virtInitialiseObjects() override;
    void virtMouseDown(int iButton, int iX, int iY) override;
    void resetGame();
    void virtMainLoopPreUpdate();
    Psyeb10TileManager* getTileManager();
protected:
    const int gridstartX = 350;
    const int gridstartY = 100;
    const int gridEndX = 950;
    const int gridEndY = 700;
    int gameScore;
    int lastTick;
    Psyeb10TileManager tm;
};


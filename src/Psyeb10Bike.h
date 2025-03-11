#pragma once
#include "header.h"
#include "Psyeb10Engine.h"
#include "DisplayableObject.h"

class Psyeb10Engine;
class Psyeb10Bike :
    public DisplayableObject
{
public:
    Psyeb10Bike(int xStart, int yStart, BaseEngine* pEngine, int iWidth, int iHeight, bool useTopLeftFor00 = true);
    void virtDoUpdate(int iCurrentTime) override;
    void virtDraw() override;
    virtual void virtHandleDeath();
protected:
    int speedX;
    int speedY;

    //Stores last tile so bike can't kill itself the second it paints
    int lastTileX;
    int lastTileY;

    //Pointer to Subclass to access the tile manager
    Psyeb10Engine* engine;
};



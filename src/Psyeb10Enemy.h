#pragma once
#include "header.h"
#include "DisplayableObject.h"
#include"Psyeb10Engine.h"

class Psyeb10Engine;

class Psyeb10Enemy :
    public DisplayableObject
{
public:
    Psyeb10Enemy(BaseEngine* engine);
    void virtDoUpdate(int iCurrentTime) override;
    void virtDraw() override;
    bool isValidMove(int x, int y);
    void setPaused(bool paused);
protected:
    int speedX;
    int speedY;

    //Stores last tile so bike can't kill itself the second it paints
    int lastTileX;
    int lastTileY;

    //Pointer to Subclass to access the tile manager
    Psyeb10Engine* engine;

    bool isPaused;
};


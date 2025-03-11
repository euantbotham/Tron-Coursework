#pragma once
#include "header.h"
#include "DisplayableObject.h"
#include "Psyeb10Engine.h"

class Psyeb10Engine;


class MainCharacter :
    public DisplayableObject
{
public:
    MainCharacter(BaseEngine* pEngine);
    void virtDoUpdate(int iCurrentTime) override;
    void virtDraw() override;
    void virtKeyDown(int iKeyCode) override;
    
protected:
    int speedX;
    int speedY;
    
    //Stores last tile so bike can't kill itself the second it paints
    int lastTileX;
    int lastTileY;

    //Pointer to Subclass to access the tile manager
    Psyeb10Engine* engine;

};


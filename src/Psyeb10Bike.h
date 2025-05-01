#pragma once
#include "header.h"
#include "Psyeb10Engine.h"
#include "DisplayableObject.h"
#include "Psyeb10TileManager.h"
#include "ImageManager.h"
#include "ImagePixelMappingRotate90.h"
class Psyeb10Engine;

class Psyeb10Bike:
    public DisplayableObject
{
public:
    Psyeb10Bike(int xStart, int yStart, BaseEngine* pEngine, int iWidth, int iHeigh, int bikeVal);
    void virtDoUpdate(int iCurrentTime) override;
    //TODO see if this can be removed
    virtual void virtKeyDown(int iKeyCode) override;
    virtual void virtHandleDeath();
    virtual void virtPostMoveLogic();
    void setPaused(bool paused);
	void virtDraw() override;
    int getSpeedX();
    int getSpeedY();
    void setSpeed(int x, int y);
	int getLastTileX() const;
	int getLastTileY() const;
	void setLastTiles(int x, int y);   
	void changeDirection(int direction);
	int getBikeValue() const;
protected:
    int speedX;
    int speedY;

    //Stores last tile so bike can't kill itself the second it paints
    int lastTileX;
    int lastTileY;

    //Pointer to Subclass to access the tile manager
    Psyeb10Engine* engine;

    //Stores if object game is paused
    bool isPaused;

    // An integer used to assign a value to the tile manager
    int bikeValue;

    ImagePixelMappingRotate90 rotation;
    std::vector<SimpleImage> animationImages;
    int lastFrameTime;
    int currentFrame;
};





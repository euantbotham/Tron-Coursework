#pragma once
#include "header.h"
#include"Psyeb10Bike.h"
#include "DisplayableObject.h"
#include"Psyeb10Engine.h"

class Psyeb10Engine; // forward declaration.

class Psyeb10Enemy: public Psyeb10Bike
{
public:
    
    Psyeb10Enemy(BaseEngine* pEngine, int xPos, int yPos, int gridVal);
    bool isValidMove(int x, int y);
    void virtHandleDeath()override;
    void virtPostMoveLogic()override;
private:
    void decideStrategicDirection(int playerX, int playerY);
    int findBestDirection();
    int floodFill(int startMapX, int startMapY, int dirX, int dirY, int maxScope, int maxTiles);
};


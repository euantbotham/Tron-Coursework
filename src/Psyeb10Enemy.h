#pragma once
#include "header.h"
#include"Psyeb10Bike.h"
#include "DisplayableObject.h"
#include"Psyeb10Engine.h"

class Psyeb10Engine; // forward declaration.

class Psyeb10Enemy: public Psyeb10Bike
{
public:
    
    Psyeb10Enemy(BaseEngine* engine);
    void virtDraw() override;
    bool isValidMove(int x, int y);
    void virtHandleDeath()override;
    void virtPostMoveLogic()override;
    
};


#pragma once
#include "header.h"
#include "Psyeb10Bike.h"
#include "DisplayableObject.h"
#include "Psyeb10Engine.h"
#include "ImageManager.h"
#include "ImagePixelMappingRotate90.h"

class Psyeb10Engine;


class MainCharacter :
    public Psyeb10Bike
{
public:
    MainCharacter(BaseEngine* pEngine);
    void virtDraw() override;
    void virtKeyDown(int iKeyCode) override;
    int getLives();
    void setLives(int newLives);
    void virtPostMoveLogic()override;
    void virtHandleDeath() override;
protected:
    int lives;
	SimpleImage image;
    ImagePixelMappingRotate90 rotation;
};


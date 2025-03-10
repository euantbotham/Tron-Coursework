#pragma once
#include "DisplayableObject.h"
class MainCharacter :
    public DisplayableObject
{
public:
    MainCharacter(BaseEngine* pEngine);
    void virtDoUpdate(int iCurrentTime) override;
    void virtDraw() override;
    void virtKeyDown(int iKeyCode) override;
    
protected:
    double speedX;
    double speedY;
    double posX;
    double posY;

    int car_direction;

};


#pragma once
#include "header.h"
#include "DisplayableObject.h"
#include "BaseEngine.h"
#include "SimpleImage.h"

//An object that is heavily inspired by the DVD pause screen logo
class bouncingLogo : public DisplayableObject {
public:
    bouncingLogo(int xStart, int yStart, BaseEngine* pEngine,  int speedX, int speedY);
      
    void virtDraw() override;
    
    void virtDoUpdate(int iCurrentTime) override;

private:
    int m_speedX; // Horizontal speed
    int m_speedY; // Vertical speed
	SimpleImage image1, image2; // Image for the first frame
    int currentImage;
};
#pragma once
#include "header.h"
#include "SimpleImage.h"
#include "DrawingSurface.h"
class DrawingSurface;
#include "ImagePixelMapping.h"
#include <iostream>


//class ImagePixelMapping;

class ImagePixelMappingRotate90 :
    public ImagePixelMapping
{
public:
    ImagePixelMappingRotate90::ImagePixelMappingRotate90();
    bool mapCoordinates(double& x, double& y, const SimpleImage& image) override;
    enum Direction { Up, Left, Right, Down };
	enum Turn { None, Clockwise, AntiClockwise };;
    bool changePixelColour(int x, int y, int& colour, DrawingSurface* pTarget) override;
    void setDirection(Direction d);
    
    // Overload << for counterclockwise rotation
    ImagePixelMappingRotate90& operator<<(Direction newDirection);

private:
    Direction direction;

};


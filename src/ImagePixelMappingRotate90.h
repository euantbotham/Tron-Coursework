#pragma once
#include "header.h"
#include "SimpleImage.h"
#include "ImagePixelMapping.h"
#include <iostream>
class drawingSurface;
class ImagePixelMapping;

class ImagePixelMappingRotate90 :
    public ImagePixelMapping
{
public:
    ImagePixelMappingRotate90::ImagePixelMappingRotate90();
    bool mapCoordinates(double& x, double& y, const SimpleImage& image) override;
    enum Direction { Up, Left, Right, Down };
	enum Turn { None, Clockwise, AntiClockwise };;
    bool changePixelColour(int x, int y, int& colour, DrawingSurface* pTarget) override;
	void setDirection(Direction d)
	{
		direction = d;
	}
    
    // Overload << for counterclockwise rotation
    ImagePixelMappingRotate90& operator<<(Direction newDirection);

private:
    Direction direction;

};


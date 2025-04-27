#include "ImagePixelMappingRotate90.h"
#include <iostream>
ImagePixelMappingRotate90::ImagePixelMappingRotate90()
{
	direction = Up;
}

bool ImagePixelMappingRotate90::mapCoordinates(double& x, double& y, const SimpleImage& image)
{
    double oldX = x;
    double oldY = y;
    double width = image.getWidth();
    double height = image.getHeight();

    if (direction == Right)
    {
        x = oldY;
        y = width - 1 - oldX;
    }
    else if (direction == Left)
    {
        x = height - 1 - oldY;
        y = oldX;
	}
	else if (direction == Down)
	{
		x = width - 1 - oldX;
		y = height - 1 - oldY;
	}
	else if (direction == Up)
	{
		x = oldX;
		y = oldY;
	}

    if (x < 0 || y < 0 || x >= width || y >= height)
        return false;

    return true;
}

bool ImagePixelMappingRotate90::changePixelColour(int x, int y, int& colour, DrawingSurface* pTarget)
{
	if (colour == 0x000000)
		return false; // Don't draw any pixel which should be transparency colour
	return true;
}


ImagePixelMappingRotate90& ImagePixelMappingRotate90::operator<<(Direction newDirection)
{
    // Counterclockwise rotation logic
	direction = newDirection;
	return *this;
}



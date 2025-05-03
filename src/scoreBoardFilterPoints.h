#pragma once
#include "FilterPoints.h"
#include "DrawingSurface.h"

// ScoreBoardScaling - Scaling filter with zoom limits
class ScoreBoardScaling : public FilterPoints
{
public:
    // Constructor with pointer to next filter
    ScoreBoardScaling(FilterPoints* pNextFilter = nullptr)
        : pNextFilter(pNextFilter), xStretch(1), yStretch(1), xCompress(1), yCompress(1)
    {
    }

    // Constructor with initial stretch factors
    ScoreBoardScaling(int xFactor, int yFactor, FilterPoints* pNextFilter = nullptr)
        : pNextFilter(pNextFilter), xStretch(1), yStretch(1), xCompress(1), yCompress(1)
    {
        setStretch(xFactor, yFactor);
    }

    // Filter implementation
    virtual bool filter(DrawingSurface* surface, int& xVirtual, int& yVirtual, unsigned int& uiColour, bool setting) override
    {
        if (setting)
            handleXthenY(surface, xVirtual, yVirtual, uiColour);
        return false; // We already colored the pixel
    }

    // Convert from virtual to real X position
    virtual int filterConvertVirtualToRealXPosition(int xVirtual) override
    {
        if (xStretch > 1)
        {
            if (pNextFilter)
                return pNextFilter->filterConvertVirtualToRealXPosition(xVirtual * xStretch);
            else
                return xVirtual * xStretch;
        }
        else
        {
            if (pNextFilter)
                return pNextFilter->filterConvertVirtualToRealXPosition(xVirtual / xCompress);
            else
                return xVirtual / xCompress;
        }
    }

    // Convert from virtual to real Y position
    virtual int filterConvertVirtualToRealYPosition(int yVirtual) override
    {
        if (yStretch > 1)
        {
            if (pNextFilter)
                return pNextFilter->filterConvertVirtualToRealYPosition(yVirtual * yStretch);
            else
                return yVirtual * yStretch;
        }
        else
        {
            if (pNextFilter)
                return pNextFilter->filterConvertVirtualToRealYPosition(yVirtual / yCompress);
            else
                return yVirtual / yCompress;
        }
    }

    // Convert from real to virtual X position
    virtual int filterConvertRealToVirtualXPosition(int xReal) override
    {
        // Apply other filter first if it exists
        if (pNextFilter)
            xReal = pNextFilter->filterConvertRealToVirtualXPosition(xReal);

        // Apply our transformation in reverse
        if (xStretch > 1)
            return xReal / xStretch;
        else
            return xReal * xCompress;
    }

    // Convert from real to virtual Y position
    virtual int filterConvertRealToVirtualYPosition(int yReal) override
    {
        // Apply other filter first if it exists
        if (pNextFilter)
            yReal = pNextFilter->filterConvertRealToVirtualYPosition(yReal);

        // Apply our transformation in reverse
        if (yStretch > 1)
            return yReal / yStretch;
        else
            return yReal * yCompress;
    }

    // Stretch X dimension with maximum limit of 3x
    void stretchX()
    {
        if (xCompress > 1)
            --xCompress;
        else if (xStretch < 3)  // Maximum zoom of 3x
            ++xStretch;
    }

    // Stretch Y dimension with maximum limit of 3x
    void stretchY()
    {
        if (yCompress > 1)
            --yCompress;
        else if (yStretch < 3)  // Maximum zoom of 3x
            ++yStretch;
    }

    // Compress X dimension with minimum limit of 1x
    void compressX()
    {
        if (xStretch > 1)
            --xStretch;
        else if (xCompress == 1)  // Minimum zoom of 1x (no smaller)
            xCompress = 1;
    }

    // Compress Y dimension with minimum limit of 1x
    void compressY()
    {
        if (yStretch > 1)
            --yStretch;
        else if (yCompress == 1)  // Minimum zoom of 1x (no smaller)
            yCompress = 1;
    }

    // Stretch both X and Y
    void stretch()
    {
        stretchX();
        stretchY();
    }

    // Compress both X and Y
    void compress()
    {
        compressX();
        compressY();
    }

    // Set stretch factors with limits enforced
    void setStretch(int xFactor, int yFactor)
    {
        // X-direction
        if (xFactor == 0) {
            xStretch = 1; xCompress = 1;
        }
        else if (xFactor < 0) {
            xStretch = 1; xCompress = 1; // Minimum zoom of 1x
        }
        else if (xFactor > 0) {
            xStretch = (xFactor > 2) ? 3 : xFactor + 1; // Maximum zoom of 3x
            xCompress = 1;
        }

        // Y-direction
        if (yFactor == 0) {
            yStretch = 1; yCompress = 1;
        }
        else if (yFactor < 0) {
            yStretch = 1; yCompress = 1; // Minimum zoom of 1x
        }
        else if (yFactor > 0) {
            yStretch = (yFactor > 2) ? 3 : yFactor + 1; // Maximum zoom of 3x
            yCompress = 1;
        }
    }

    // Get current zoom level for X
    float getZoomX()
    {
        int v1 = xStretch; if (v1 < 1) v1 = 1;
        int v2 = xCompress; if (v2 < 1) v2 = 1;
        return (float)v1 / (float)v2;
    }

    // Get current zoom level for Y
    float getZoomY()
    {
        int v1 = yStretch; if (v1 < 1) v1 = 1;
        int v2 = yCompress; if (v2 < 1) v2 = 1;
        return (float)v1 / (float)v2;
    }

    // Reset zoom to default (1x)
    void resetZoom()
    {
        xStretch = 1;
        yStretch = 1;
        xCompress = 1;
        yCompress = 1;
    }

private:
    FilterPoints* pNextFilter;
    int xStretch, yStretch, xCompress, yCompress;

    // Handle drawing in X then Y dimensions
    void handleXthenY(DrawingSurface* surface, int xVirtual, int yVirtual, unsigned int& uiColour)
    {
        if (xStretch > 1) // Stretch it!
        {
            xVirtual *= xStretch; // First multiply the point coordinates
            for (int i = 0; i < xStretch; i++)
                handleY(surface, xVirtual + i, yVirtual, uiColour);
        }
        else // No stretching
        {
            xVirtual /= xCompress; // Shrink the coordinate by this amount
            handleY(surface, xVirtual, yVirtual, uiColour);
        }
    }

    // Handle drawing in Y dimension
    void handleY(DrawingSurface* surface, int xVirtual, int yVirtual, unsigned int& uiColour)
    {
        if (yStretch > 1) // Stretch it!
        {
            yVirtual *= yStretch; // First multiply the point coordinates
            for (int i = 0; i < yStretch; i++)
            {
                // If no following filter, or filter permits it, then color the pixel
                int yTest = yVirtual + i;
                if ((pNextFilter == nullptr) || pNextFilter->filter(surface, xVirtual, yTest, uiColour, true))
                    surface->rawSetPixel(xVirtual, yTest, uiColour);
            }
        }
        else
        {
            yVirtual /= yCompress; // Shrink the coordinate by this amount
            if ((pNextFilter == nullptr) || pNextFilter->filter(surface, xVirtual, yVirtual, uiColour, true))
                surface->rawSetPixel(xVirtual, yVirtual, uiColour);
        }
    }
};


// ScoreBoardTranslation - Translation filter with boundary limits
class ScoreBoardTranslation : public FilterPoints
{
public:
    // Constructor with pointer to next filter
    ScoreBoardTranslation(int xModifier = 0, int yModifier = 0, FilterPoints* pNextFilter = nullptr)
        : xModifier(xModifier), yModifier(yModifier), pNextFilter(pNextFilter),
        minX(-500), maxX(500), minY(-500), maxY(500) // Default boundaries
    {
    }

    // Filter implementation
    virtual bool filter(DrawingSurface* surface, int& xVirtual, int& yVirtual, unsigned int& uiColour, bool setting) override
    {
        xVirtual += xModifier;
        yVirtual += yModifier;
        return (pNextFilter == nullptr) || pNextFilter->filter(surface, xVirtual, yVirtual, uiColour, setting);
    }

    // Convert from virtual to real X position
    virtual int filterConvertVirtualToRealXPosition(int xVirtual) override
    {
        // Apply other filter first if it exists
        if (pNextFilter)
            return pNextFilter->filterConvertVirtualToRealXPosition(xVirtual + xModifier);
        return xVirtual + xModifier;
    }

    // Convert from virtual to real Y position
    virtual int filterConvertVirtualToRealYPosition(int yVirtual) override
    {
        // Apply other filter first if it exists
        if (pNextFilter)
            return pNextFilter->filterConvertVirtualToRealYPosition(yVirtual + yModifier);
        return yVirtual + yModifier;
    }

    // Convert from real to virtual X position
    virtual int filterConvertRealToVirtualXPosition(int x) override
    {
        // Apply other filter first if it exists
        if (pNextFilter)
            x = pNextFilter->filterConvertRealToVirtualXPosition(x);
        return x - xModifier;
    }

    // Convert from real to virtual Y position
    virtual int filterConvertRealToVirtualYPosition(int y) override
    {
        // Apply other filter first if it exists
        if (pNextFilter)
            y = pNextFilter->filterConvertRealToVirtualYPosition(y);
        return y - yModifier;
    }

    // Set the offset with boundary checks
    void setOffset(int offsetX, int offsetY)
    {
        // Apply boundary limits
        xModifier = constrainX(offsetX);
        yModifier = constrainY(offsetY);
    }

    // Get current X offset
    int getXOffset() { return xModifier; }

    // Get current Y offset
    int getYOffset() { return yModifier; }

    // Change the offset with boundary checks
    void changeOffset(int offsetXIncrement, int offsetYIncrement)
    {
        // Apply boundary limits
        xModifier = constrainX(xModifier + offsetXIncrement);
        yModifier = constrainY(yModifier + offsetYIncrement);
    }

    // Set boundaries for translation
    void setBoundaries(int minXValue, int maxXValue, int minYValue, int maxYValue)
    {
        minX = minXValue;
        maxX = maxXValue;
        minY = minYValue;
        maxY = maxYValue;

        // Ensure current offsets are within the new boundaries
        xModifier = constrainX(xModifier);
        yModifier = constrainY(yModifier);
    }

    // Set the boundaries based on screen dimensions and content size
    void setBoundariesForContent(int screenWidth, int screenHeight, int contentWidth, int contentHeight)
    {
        // Calculate maximum translation to keep content visible
        // Allow movement that keeps at least 1/4 of the content visible on screen
        int marginX = screenWidth / 4;
        int marginY = screenHeight / 4;

        minX = -contentWidth + marginX;
        maxX = screenWidth - marginX;
        minY = -contentHeight + marginY;
        maxY = screenHeight - marginY;

        // Ensure current offsets are within the new boundaries
        xModifier = constrainX(xModifier);
        yModifier = constrainY(yModifier);
    }

    // Reset translation to origin
    void resetTranslation()
    {
        xModifier = 0;
        yModifier = 0;
    }

private:
    int xModifier, yModifier;
    int minX, maxX, minY, maxY;
    FilterPoints* pNextFilter;

    // Constrain X translation within boundaries
    int constrainX(int x)
    {
        if (x < minX) return minX;
        if (x > maxX) return maxX;
        return x;
    }

    // Constrain Y translation within boundaries
    int constrainY(int y)
    {
        if (y < minY) return minY;
        if (y > maxY) return maxY;
        return y;
    }
};
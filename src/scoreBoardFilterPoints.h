#pragma once
#include "FilterPoints.h"

class scoreBoardFilterPoints : public FilterPoints {
private:
    int xStretch = 1, yStretch = 1; // Stretch factors
    int xCompress = 1, yCompress = 1; // Compress factors
    int xOffset = 0, yOffset = 0; // Translation offsets

public:
    // Transform the coordinates based on scaling and translation
    bool filter(DrawingSurface* surface, int& x, int& y, unsigned int& uiColour, bool setting) override {
        // Apply translation (scrolling)
        x -= xOffset;
        y -= yOffset;

        // Apply scaling
        if (setting) {
            handleXthenY(surface, x, y, uiColour);
        }
        return false; // We already colored the pixel anyway
    }

    // Convert virtual to real X position
    int filterConvertVirtualToRealXPosition(int xVirtual) override {
        if (xStretch > 1) {
            xVirtual *= xStretch;
        }
        else {
            xVirtual /= xCompress;
        }
        return xVirtual + xOffset;
    }

    // Convert virtual to real Y position
    int filterConvertVirtualToRealYPosition(int yVirtual) override {
        if (yStretch > 1) {
            yVirtual *= yStretch;
        }
        else {
            yVirtual /= yCompress;
        }
        return yVirtual + yOffset;
    }

    // Convert real to virtual X position
    int filterConvertRealToVirtualXPosition(int xReal) override {
        xReal -= xOffset;
        if (xStretch > 1) {
            return xReal / xStretch;
        }
        else {
            return xReal * xCompress;
        }
    }

    // Convert real to virtual Y position
    int filterConvertRealToVirtualYPosition(int yReal) override {
        yReal -= yOffset;
        if (yStretch > 1) {
            return yReal / yStretch;
        }
        else {
            return yReal * yCompress;
        }
    }

    // Stretch in X direction
    void stretchX() { if (xCompress > 1) --xCompress; else ++xStretch; }

    // Stretch in Y direction
    void stretchY() { if (yCompress > 1) --yCompress; else ++yStretch; }

    // Compress in X direction
    void compressX() { if (xStretch > 1) --xStretch; else ++xCompress; }

    // Compress in Y direction
    void compressY() { if (yStretch > 1) --yStretch; else ++yCompress; }

    // Compress both X and Y
    void compress() { compressX(); compressY(); }

    // Stretch both X and Y
    void stretch() { stretchX(); stretchY(); }

    // Set stretch/compress factors
    void setStretch(int xFactor, int yFactor) {
        if (xFactor == 0) { xStretch = 1; xCompress = 1; }
        else if (xFactor < 0) { xStretch = 1; xCompress = 1 - xFactor; }
        else { xStretch = 1 + xFactor; xCompress = 1; }

        if (yFactor == 0) { yStretch = 1; yCompress = 1; }
        else if (yFactor < 0) { yStretch = 1; yCompress = 1 - yFactor; }
        else { yStretch = 1 + yFactor; yCompress = 1; }
    }

    // Get zoom level for X
    float getZoomX() {
        int v1 = xStretch > 1 ? xStretch : 1;
        int v2 = xCompress > 1 ? xCompress : 1;
        return static_cast<float>(v1) / v2;
    }

    // Get zoom level for Y
    float getZoomY() {
        int v1 = yStretch > 1 ? yStretch : 1;
        int v2 = yCompress > 1 ? yCompress : 1;
        return static_cast<float>(v1) / v2;
    }

    // Change the offset for translation
    void changeOffset(int dx, int dy) {
        xOffset += dx;
        yOffset += dy;
    }

    // Set the offset for translation
    void setOffset(int x, int y) {
        xOffset = x;
        yOffset = y;
    }

    // Handle arrow key scrolling
    void handleKeyPress(int keyCode) {
        const int scrollAmount = 10; // Amount to scroll per key press
        switch (keyCode) {
        case SDLK_LEFT:
            changeOffset(scrollAmount, 0); // Scroll right
            break;
        case SDLK_RIGHT:
            changeOffset(-scrollAmount, 0); // Scroll left
            break;
        case SDLK_UP:
            changeOffset(0, scrollAmount); // Scroll down
            break;
        case SDLK_DOWN:
            changeOffset(0, -scrollAmount); // Scroll up
            break;
        case SDLK_SPACE:
            setOffset(0, 0); // Reset translation
            break;
        }
    }

private:
    void handleXthenY(DrawingSurface* surface, int xVirtual, int yVirtual, unsigned int& uiColour) {
        if (xStretch > 1) { // Stretch it!
            xVirtual *= xStretch; // First multiply the point coordinates
            for (int i = 0; i < xStretch; i++) {
                int xTest = xVirtual + i;
                // Check bounds before calling handleY
                if (xTest >= 0 && xTest < surface->getSurfaceWidth()) {
                    handleY(surface, xTest, yVirtual, uiColour);
                }
            }
        }
        else { // Shrinking, so just convert the pixel values and skip some
            xVirtual /= xCompress; // Shrink the coordinate by this amount
            // Check bounds before calling handleY
            if (xVirtual >= 0 && xVirtual < surface->getSurfaceWidth()) {
                handleY(surface, xVirtual, yVirtual, uiColour);
            }
        }
    }


    void handleY(DrawingSurface* surface, int xVirtual, int yVirtual, unsigned int& uiColour) {
        if (yStretch > 1) { // Stretch it!
            yVirtual *= yStretch; // First multiply the point coordinates
            for (int i = 0; i < yStretch; i++) {
                int yTest = yVirtual + i;
                // Check bounds before setting the pixel
                if (xVirtual >= 0 && xVirtual < surface->getSurfaceWidth() &&
                    yTest >= 0 && yTest < surface->getSurfaceHeight()) {
                    surface->rawSetPixel(xVirtual, yTest, uiColour); // Color this pixel now
                }
            }
        }
        else {
            yVirtual /= yCompress; // Shrink the coordinate by this amount
            // Check bounds before setting the pixel
            if (xVirtual >= 0 && xVirtual < surface->getSurfaceWidth() &&
                yVirtual >= 0 && yVirtual < surface->getSurfaceHeight()) {
                surface->rawSetPixel(xVirtual, yVirtual, uiColour); // Color this pixel now
            }
        }
    }

};

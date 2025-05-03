
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
        // Apply scaling first then translation for drawing
        if (setting) {
            // Store original coordinates since we'll modify them
            int originalX = x;
            int originalY = y;

            // Apply scaling through our helper method instead of modifying x and y directly
            handleXthenY(surface, originalX, originalY, uiColour);
            return false; // We already handled the pixel drawing
        }

        // Apply translation and scaling for non-drawing operations
        if (xStretch > 1) {
            x = x * xStretch;
        }
        else {
            x = x / xCompress;
        }

        if (yStretch > 1) {
            y = y * yStretch;
        }
        else {
            y = y / yCompress;
        }

        // Apply translation after scaling
        x += xOffset;
        y += yOffset;

        return true;
    }

    // Convert virtual to real X position
    int filterConvertVirtualToRealXPosition(int xVirtual) override {
        // Apply scaling
        if (xStretch > 1) {
            xVirtual = xVirtual * xStretch;
        }
        else {
            xVirtual = xVirtual / xCompress;
        }

        // Apply translation
        return xVirtual + xOffset;
    }

    // Convert virtual to real Y position
    int filterConvertVirtualToRealYPosition(int yVirtual) override {
        // Apply scaling
        if (yStretch > 1) {
            yVirtual = yVirtual * yStretch;
        }
        else {
            yVirtual = yVirtual / yCompress;
        }

        // Apply translation
        return yVirtual + yOffset;
    }

    // Convert real to virtual X position
    int filterConvertRealToVirtualXPosition(int xReal) override {
        // Remove translation first
        xReal = xReal - xOffset;

        // Then remove scaling
        if (xStretch > 1) {
            return xReal / xStretch;
        }
        else {
            return xReal * xCompress;
        }
    }

    // Convert real to virtual Y position
    int filterConvertRealToVirtualYPosition(int yReal) override {
        // Remove translation first
        yReal = yReal - yOffset;

        // Then remove scaling
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

    // Get current offsets
    int getXOffset() { return xOffset; }
    int getYOffset() { return yOffset; }

    // Handle arrow key scrolling
    void handleKeyPress(int keyCode) {
        const int scrollAmount = 10; // Amount to scroll per key press
        switch (keyCode) {
        case SDLK_LEFT:
            changeOffset(-scrollAmount, 0); // Move view left
            break;
        case SDLK_RIGHT:
            changeOffset(scrollAmount, 0); // Move view right
            break;
        case SDLK_UP:
            changeOffset(0, -scrollAmount); // Move view up
            break;
        case SDLK_DOWN:
            changeOffset(0, scrollAmount); // Move view down
            break;
        case SDLK_SPACE:
            setOffset(0, 0); // Reset translation
            break;
        }
    }

private:
    void handleXthenY(DrawingSurface* surface, int xVirtual, int yVirtual, unsigned int& uiColour) {
        // Apply translation first
        int translatedX = xVirtual + xOffset;
        int translatedY = yVirtual + yOffset;

        if (xStretch > 1) { // Stretch it!
            int baseX = translatedX * xStretch;
            for (int i = 0; i < xStretch; i++) {
                int xPixel = baseX + i;
                // Check bounds before proceeding
                if (xPixel >= 0 && xPixel < surface->getSurfaceWidth()) {
                    handleY(surface, xPixel, translatedY, uiColour);
                }
            }
        }
        else { // Shrinking
            int xPixel = translatedX / xCompress;
            // Check bounds before proceeding
            if (xPixel >= 0 && xPixel < surface->getSurfaceWidth()) {
                handleY(surface, xPixel, translatedY, uiColour);
            }
        }
    }

    void handleY(DrawingSurface* surface, int xPixel, int yVirtual, unsigned int& uiColour) {
        if (yStretch > 1) { // Stretch it!
            int baseY = yVirtual * yStretch;
            for (int i = 0; i < yStretch; i++) {
                int yPixel = baseY + i;
                // Check bounds before setting the pixel
                if (yPixel >= 0 && yPixel < surface->getSurfaceHeight()) {
                    surface->rawSetPixel(xPixel, yPixel, uiColour);
                }
            }
        }
        else { // Shrinking
            int yPixel = yVirtual / yCompress;
            // Check bounds before setting the pixel
            if (yPixel >= 0 && yPixel < surface->getSurfaceHeight()) {
                surface->rawSetPixel(xPixel, yPixel, uiColour);
            }
        }
    }
};
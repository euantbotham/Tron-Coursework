#pragma once
#include "FilterPoints.h"

class ScoreBoardFilterPoints : public FilterPoints {
private:
    int xStretch = 1, yStretch = 1;        // Stretch factors
    int xCompress = 1, yCompress = 1;      // Compress factors
    int xOffset = 0, yOffset = 0;          // Translation offsets
    FilterPoints* pNextFilter = nullptr;    // Chain to next filter

public:
    // Default constructor
    ScoreBoardFilterPoints(FilterPoints* pNextFilter = nullptr)
        : pNextFilter(pNextFilter) {
    }

    // Constructor with scaling
    ScoreBoardFilterPoints(int xFactor, int yFactor, FilterPoints* pNextFilter = nullptr)
        : pNextFilter(pNextFilter)
    {
        setStretch(xFactor, yFactor);
    }

    // Constructor with translation
    ScoreBoardFilterPoints(int xTranslation, int yTranslation, bool isTranslation, FilterPoints* pNextFilter = nullptr)
        : xOffset(xTranslation), yOffset(yTranslation), pNextFilter(pNextFilter) {
    }

    // Constructor with both scaling and translation
    ScoreBoardFilterPoints(int xFactor, int yFactor, int xTranslation, int yTranslation, FilterPoints* pNextFilter = nullptr)
        : xOffset(xTranslation), yOffset(yTranslation), pNextFilter(pNextFilter)
    {
        setStretch(xFactor, yFactor);
    }

    // Transform the coordinates based on scaling and translation
    bool filter(DrawingSurface* surface, int& x, int& y, unsigned int& uiColour, bool setting) override {
        if (setting) {
            // First apply scaling, then translation, then any chained filters
            handleXthenY(surface, x, y, uiColour);
            return false; // We already handled the pixel drawing
        }

        // Apply scaling
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

        // Apply translation
        x += xOffset;
        y += yOffset;

        // Apply chained filter if it exists
        return (pNextFilter == nullptr) || pNextFilter->filter(surface, x, y, uiColour, setting);
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
        xVirtual += xOffset;

        // Apply next filter if exists
        if (pNextFilter) {
            return pNextFilter->filterConvertVirtualToRealXPosition(xVirtual);
        }
        return xVirtual;
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
        yVirtual += yOffset;

        // Apply next filter if exists
        if (pNextFilter) {
            return pNextFilter->filterConvertVirtualToRealYPosition(yVirtual);
        }
        return yVirtual;
    }

    // Convert real to virtual X position
    int filterConvertRealToVirtualXPosition(int xReal) override {
        // Apply other filter first if it exists (since we are reversing the order)
        if (pNextFilter) {
            xReal = pNextFilter->filterConvertRealToVirtualXPosition(xReal);
        }

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
        // Apply other filter first if it exists (since we are reversing the order)
        if (pNextFilter) {
            yReal = pNextFilter->filterConvertRealToVirtualYPosition(yReal);
        }

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
    void stretchX() {
        if (xCompress > 1)
            --xCompress;
        else
            ++xStretch;
    }

    // Stretch in Y direction
    void stretchY() {
        if (yCompress > 1)
            --yCompress;
        else
            ++yStretch;
    }

    // Compress in X direction
    void compressX() {
        if (xStretch > 1)
            --xStretch;
        else
            ++xCompress;
    }

    // Compress in Y direction
    void compressY() {
        if (yStretch > 1)
            --yStretch;
        else
            ++yCompress;
    }

    // Compress both X and Y
    void compress() { compressX(); compressY(); }

    // Stretch both X and Y
    void stretch() { stretchX(); stretchY(); }

    // Set stretch/compress factors
    void setStretch(int xFactor, int yFactor) {
        if (xFactor == 0) { xStretch = 1; xCompress = 1; }
        else if (xFactor < 0) { xStretch = 1; xCompress = 1 - xFactor; }
        else if (xFactor > 0) { xStretch = 1 + xFactor; xCompress = 1; }

        if (yFactor == 0) { yStretch = 1; yCompress = 1; }
        else if (yFactor < 0) { yStretch = 1; yCompress = 1 - yFactor; }
        else if (yFactor > 0) { yStretch = 1 + yFactor; yCompress = 1; }
    }

    // Get zoom level for X
    float getZoomX() {
        int v1 = xStretch; if (v1 < 1) v1 = 1;
        int v2 = xCompress; if (v2 < 1) v2 = 1;
        return static_cast<float>(v1) / static_cast<float>(v2);
    }

    // Get zoom level for Y
    float getZoomY() {
        int v1 = yStretch; if (v1 < 1) v1 = 1;
        int v2 = yCompress; if (v2 < 1) v2 = 1;
        return static_cast<float>(v1) / static_cast<float>(v2);
    }

    // Set the offset for translation
    void setOffset(int x, int y) {
        xOffset = x;
        yOffset = y;
    }

    // Change the offset for translation
    void changeOffset(int dx, int dy) {
        xOffset += dx;
        yOffset += dy;
    }

    // Get current offsets
    int getXOffset() { return xOffset; }
    int getYOffset() { return yOffset; }

    // Reset all zoom and translation settings
    void resetView() {
        xStretch = 1;
        yStretch = 1;
        xCompress = 1;
        yCompress = 1;
        xOffset = 0;
        yOffset = 0;
    }

    // Handle arrow key scrolling - using enum or #define values for key codes
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
        case SDLK_LSHIFT:
            resetView(); // Reset all transformations
            break;
        }
    }

private:
    // Handle drawing with proper scaling and translation
    void handleXthenY(DrawingSurface* surface, int xVirtual, int yVirtual, unsigned int& uiColour) {
        // Create working copies of coordinates
        int x = xVirtual;
        int y = yVirtual;

        // Apply scaling for X
        if (xStretch > 1) { // Stretch in X direction
            x *= xStretch; // Scale the coordinate

            // Apply translation
            x += xOffset;

            // Draw multiple pixels in X direction
            for (int i = 0; i < xStretch; i++) {
                int xPixel = x + i;
                handleY(surface, xPixel, y, uiColour);
            }
        }
        else { // Compress in X direction
            x /= xCompress; // Scale the coordinate

            // Apply translation
            x += xOffset;

            // Draw a single pixel in X direction
            handleY(surface, x, y, uiColour);
        }
    }

    void handleY(DrawingSurface* surface, int xPixel, int yVirtual, unsigned int& uiColour) {
        // Apply scaling for Y
        int y = yVirtual;

        if (yStretch > 1) { // Stretch in Y direction
            y *= yStretch; // Scale the coordinate

            // Apply translation
            y += yOffset;

            // Draw multiple pixels in Y direction
            for (int i = 0; i < yStretch; i++) {
                int yPixel = y + i;

                // Check bounds before drawing
                if (xPixel >= 0 && xPixel < surface->getSurfaceWidth() &&
                    yPixel >= 0 && yPixel < surface->getSurfaceHeight()) {

                    // If there's a next filter, let it handle the pixel, otherwise draw directly
                    int xTemp = xPixel;
                    int yTemp = yPixel;
                    if ((pNextFilter == nullptr) || pNextFilter->filter(surface, xTemp, yTemp, uiColour, true)) {
                        surface->rawSetPixel(xPixel, yPixel, uiColour);
                    }
                }
            }
        }
        else { // Compress in Y direction
            y /= yCompress; // Scale the coordinate

            // Apply translation
            y += yOffset;

            // Draw a single pixel in Y direction
            // Check bounds before drawing
            if (xPixel >= 0 && xPixel < surface->getSurfaceWidth() &&
                y >= 0 && y < surface->getSurfaceHeight()) {

                // If there's a next filter, let it handle the pixel, otherwise draw directly
                int xTemp = xPixel;
                int yTemp = y;
                if ((pNextFilter == nullptr) || pNextFilter->filter(surface, xTemp, yTemp, uiColour, true)) {
                    surface->rawSetPixel(xPixel, y, uiColour);
                }
            }
        }
    }
};
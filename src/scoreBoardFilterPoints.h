#pragma once
#include "FilterPoints.h"

class scoreBoardFilterPoints : public FilterPoints
{
private:
    double offsetX = 0; // Horizontal scroll offset
    double offsetY = 0; // Vertical scroll offset
    double zoom = 1.0;  // Zoom level

public:
    // Transform the coordinates based on scrolling and zooming
    bool filter(DrawingSurface* surface, int& x, int& y, unsigned int& uiColour, bool setting) override {
        // Apply scrolling and zoom
        x = static_cast<int>((x - offsetX) * zoom);
        y = static_cast<int>((y - offsetY) * zoom);

        // Always allow drawing (you could add checks here if needed)
        return true;
    }

    // Update the scroll offsets
    void scroll(double dx, double dy) {
        offsetX += dx;
        offsetY += dy;
    }

    // Update the zoom level
    void setZoom(double zoomFactor) {
        zoom = zoomFactor;
    }
};

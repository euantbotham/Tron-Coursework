#include "gameSetupState.h"

void gameSetupState::enter()
{
    // Reset animation counters
    currentFrame = 0;

    // Set a very high animation delay (will be used as-is)
    animationDelay = 30; // Try with a reasonable value that won't be overwritten

    // Fill the background with black
    engine->fillBackground(0x000000);

    // Screen dimensions
    const int screenWidth = engine->getWindowWidth();
    const int screenHeight = engine->getWindowHeight();

    // Grid dimensions
    const int tileWidth = 50;
    const int tileHeight = 50;
    const int rows = screenHeight / tileHeight;
    const int columns = screenWidth / tileWidth;

    // Base grid color (dark blue-gray)
    const unsigned int baseColor = 0x101820;
    // Subtle highlight color (slightly lighter)
    const unsigned int highlightColor = 0x1A2635;

    // Create just 2 surfaces for a very subtle toggle effect
    for (int i = 0; i < 2; ++i) {
        DrawingSurface* surface = new DrawingSurface(engine);
        surface->createSurface(screenWidth, screenHeight);
        surface->mySDLLockSurface();

        // Fill background with base color
        surface->drawRectangle(0, 0, screenWidth, screenHeight, baseColor);

        // Animation offset for flowing effect
        int offset = i * 5; // Subtle movement

        // Draw the grid on the surface
        for (int row = 0; row < rows + 1; ++row) {
            // Horizontal lines with flowing effect
            int y = row * tileHeight;
            // Varying opacity based on position and frame
            int intensity = 60 + 20 * sin((row + i) * 0.3);
            unsigned int lineColor = (intensity << 16) | (intensity << 8) | intensity;

            // Draw horizontal line
            surface->drawLine(0, y, screenWidth, y, lineColor);
            if ((row + i) % 5 == 0) {
                // Slightly thicker line every 5 tiles
                surface->drawLine(0, y + 1, screenWidth, y + 1, lineColor);
            }
        }

        for (int col = 0; col < columns + 1; ++col) {
            // Vertical lines with flowing effect
            int x = col * tileWidth;
            // Varying opacity based on position and frame
            int intensity = 60 + 20 * sin((col + i) * 0.3);
            unsigned int lineColor = (intensity << 16) | (intensity << 8) | intensity;

            // Draw vertical line
            surface->drawLine(x, 0, x, screenHeight, lineColor);
            if ((col + i) % 5 == 0) {
                // Slightly thicker line every 5 tiles
                surface->drawLine(x + 1, 0, x + 1, screenHeight, lineColor);
            }
        }

        // Add subtle diagonal highlights using small lines instead of points
        for (int j = 0; j < screenWidth + screenHeight; j += 120) {
            int diagonalPos = (j + offset * 3) % (screenWidth + screenHeight);

            // Draw diagonal highlights using very short lines
            for (int k = 0; k < screenHeight; k += 50) {
                int x1 = diagonalPos - k;
                int y1 = k;
                if (x1 >= 0 && x1 < screenWidth) {
                    // Draw tiny line segments instead of points
                    surface->drawLine(x1, y1, x1 + 1, y1, 0x303A4A);
                }
            }
        }

        // Store the surface
        surfaces.push_back(surface);
        surface->mySDLUnlockSurface();
    }

    // Set the first surface as the background
    engine->setBackgroundSurface(surfaces[0]);
}

void gameSetupState::foreGroundStrings()
{
    // Empty implementation
}

void gameSetupState::initObjects()
{
    // Empty implementation  
}

void gameSetupState::reset()
{
    // Empty implementation
}

void gameSetupState::mouseDown(int iButton, int iX, int iY)
{
    // Empty implementation
}

void gameSetupState::mainLoopPreUpdate()
{
    // Only increment the tick counter, don't reset it



    // Only change frame when the tick count is high enough
    if (animationDelay == 0) {
        animationDelay = 30;
        currentFrame = (currentFrame + 1) % surfaces.size(); // Toggle between 0 and 1
        engine->setBackgroundSurface(surfaces[currentFrame]);
        engine->redrawDisplay();
    }
    else {
        animationDelay--; // Decrease the delay
    }
}

void gameSetupState::reEntry()
{
    // Empty implementation
}

void gameSetupState::keyPressed(int iKeyCode)
{
    // Empty implementation
}

gameSetupState::~gameSetupState()
{
    // Clean up surfaces
    for (auto surface : surfaces) {
        delete surface;
    }
    surfaces.clear();
}
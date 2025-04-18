#include "gameSetupState.h"

void gameSetupState::enter()
{
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

    // Create and initialize frames for animation
    for (int i = 0; i < 10; ++i) {
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

            // Draw horizontal line with varying thickness
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

            // Draw vertical line with varying thickness
            surface->drawLine(x, 0, x, screenHeight, lineColor);
            if ((col + i) % 5 == 0) {
                // Slightly thicker line every 5 tiles
                surface->drawLine(x + 1, 0, x + 1, screenHeight, lineColor);
            }
        }
        /*
        // Add subtle diagonal highlights that shift across frames
        for (int j = 0; j < screenWidth + screenHeight; j += 100) {
            int diagonalPos = (j + offset * 2) % (screenWidth + screenHeight);
            // Draw faint diagonal lines
            for (int k = 0; k < screenHeight; k += 5) {
                int x1 = diagonalPos - k;
                int y1 = k;
                if (x1 >= 0 && x1 < screenWidth) {
                    // Subtle highlight with low opacity
                    surface->drawPoint(x1, y1, 0x303A4A);
                }
            }
        }
        */
        // Store the surface
        surfaces.push_back(surface);
        surface->mySDLUnlockSurface();
    }

    // Set animation parameters
    animationDelay = 8;  // Slower animation for subtlety
    tick = 0;
    currentFrame = 0;

    // Set the first surface as the background
    engine->setBackgroundSurface(surfaces[0]);
}

void gameSetupState::foreGroundStrings()
{

}

void gameSetupState::initObjects()
{
}

void gameSetupState::reset()
{
}

void gameSetupState::mouseDown(int iButton, int iX, int iY)
{
}

void gameSetupState::mainLoopPreUpdate()
{
    // Increment tick and switch frame after delay
    if (++tick >= animationDelay) {
        tick = 0; // Reset tick
        currentFrame = (currentFrame + 1) % surfaces.size(); // Move to the next frame
        engine->setBackgroundSurface(surfaces[currentFrame]); // Switch background surface
        engine->redrawDisplay(); // Redraw the display
    }
}

void gameSetupState::reEntry()
{
}

void gameSetupState::keyPressed(int iKeyCode)
{

}

// Don't forget to clean up surfaces in the destructor
gameSetupState::~gameSetupState()
{
    for (auto surface : surfaces) {
        delete surface;
    }
    surfaces.clear();
}
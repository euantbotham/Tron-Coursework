#include "mainMenuState.h"

void mainMenuState::enter()
{
    // Fill the background with black
    engine->fillBackground(0x000000);

    // Define the grid dimensions
    const int gridStartX = 0;
    const int gridStartY = 0;
    const int gridEndX = 1300;
    const int gridEndY = 800;
    const int cellSize = 50; // Size of each grid cell

    // Draw the blue background
    engine->drawBackgroundRectangle(gridStartX, gridStartY, gridEndX, gridEndY, 0x000822);

    // Draw vertical grid lines
    for (int x = gridStartX; x <= gridEndX; x += cellSize) {
        engine->drawBackgroundThickLine(x, gridStartY, x, gridEndY, 0x000000, 2);
    }

    // Draw horizontal grid lines
    for (int y = gridStartY; y <= gridEndY; y += cellSize) {
        engine->drawBackgroundThickLine(gridStartX, y, gridEndX, y, 0x000000, 2);
    }
}

void mainMenuState::mainLoopPreUpdate()
{
    screenOffsetX = (screenOffsetX + 1) % engine->getWindowWidth();
    engine->redrawDisplay();
}



void mainMenuState::foreGroundStrings()
{
}

void mainMenuState::initObjects()
{
}

void mainMenuState::reset()
{
}

void mainMenuState::mouseDown(int iButton, int iX, int iY)
{
}
void mainMenuState::reEntry()
{
}


void mainMenuState::copyAllBackgroundBuffer()
{
	engine->getForegroundSurface()->copyRectangleFrom(engine->getBackgroundSurface(), 0, 0, engine->getWindowWidth(), engine->getWindowHeight(), screenOffsetX, 0);
	engine->getForegroundSurface()->copyRectangleFrom(engine->getBackgroundSurface(), engine->getWindowWidth() -screenOffsetX, 0, engine->getWindowWidth(), engine->getWindowHeight(),screenOffsetX - engine->getWindowWidth(), 0);
}
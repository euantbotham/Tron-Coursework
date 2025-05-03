#include "scoreBoardState.h"


scoreBoardState::scoreBoardState(Psyeb10Engine* engine)
    // Initialize filters - first scaling, then translation that points to scaling
    : m_filterScaling(0, 0, engine), 
      m_filterTranslation(0, 0, &m_filterScaling)
{
    this->engine = engine;
    inputName = "";
}

void scoreBoardState::enter() 
{
    // Fill the background with black
    engine->fillBackground(0x000822);
    // Screen dimensions
    const int screenWidth = engine->getWindowWidth();
    const int screenHeight = engine->getWindowHeight();

    // Grid dimensions
    const int tileWidth = 50;
    const int tileHeight = 50;


    // Draw vertical grid lines
    for (int x = 0; x <= screenWidth; x += tileWidth) {
        engine->drawBackgroundThickLine(x, 0, x, screenHeight, 0x000000,2);
    }

    // Draw horizontal grid lines
    for (int y = 0; y <= screenHeight; y += tileHeight) {
        engine->drawBackgroundThickLine(0, y, screenWidth, y, 0x000000,2);
    }
    // Set the filter chain for drawing
    engine->getForegroundSurface()->setDrawPointsFilter(&m_filterTranslation);
}

void scoreBoardState::foreGroundStrings()
{
    // Set the title
    engine->drawForegroundString(500, 50, "Score Board", engine->tronBlue);

    // Instructions moved to the bottom of the screen
    engine->drawForegroundString(400, 550, "Press 'ESC' to go back to the main menu", engine->tronBlue);

    // Placeholder scoreboard entries
    const int startX = 400; // Starting X position for the scoreboard
    const int startY = 100; // Starting Y position for the scoreboard
    const int entryHeight = 40; // Vertical spacing between entries

    // Draw column headers
    engine->drawForegroundString(startX, startY, "Rank", engine->tronBlue);
    engine->drawForegroundString(startX + 100, startY, "Name", engine->tronBlue);
    engine->drawForegroundString(startX + 300, startY, "Score", engine->tronBlue);

    // Draw a line under the headers
    engine->drawForegroundLine(startX, startY + 30, startX + 400, startY + 30, engine->tronBlue);

    // Draw the scoreboard entries
    for (int i = 0; i < 10; ++i) {
        // Entry number
        char entryNumber[4];
        sprintf(entryNumber, "%d.", i + 1);
        engine->drawForegroundString(startX, startY + 50 + i * entryHeight, entryNumber, engine->tronBlue);

        // Placeholder name
        engine->drawForegroundString(startX + 100, startY + 50 + i * entryHeight, "Player", engine->tronBlue);

        // Placeholder score
        engine->drawForegroundString(startX + 300, startY + 50 + i * entryHeight, "0000", engine->tronBlue);
    }

    // Display zoom information at the bottom right
    char zoomBuf[128];
    sprintf(zoomBuf, "Zoom: %.2fx", m_filterScaling.getZoomX());
    engine->drawForegroundString(700, 550, zoomBuf, engine->tronBlue);
}


void scoreBoardState::keyPressed(int iKeyCode) 
{
    if (iKeyCode >= SDLK_a && iKeyCode <= SDLK_z) {
        inputName += static_cast<char>(iKeyCode);
    }
    else if (iKeyCode == SDLK_SPACE) {
        inputName += ' ';
    }
    else if (iKeyCode == SDLK_BACKSPACE) {
        if (inputName.length() > 0) {
            inputName.pop_back();
        }
    }
    else {
        // Handle navigation with arrow keys
        switch (iKeyCode) {
        case SDLK_LEFT:
            m_filterTranslation.changeOffset(-10, 0);
            break;
        case SDLK_RIGHT:
            m_filterTranslation.changeOffset(10, 0);
            break;
        case SDLK_UP:
            m_filterTranslation.changeOffset(0, -10);
            break;
        case SDLK_DOWN:
            m_filterTranslation.changeOffset(0, 10);
            break;
        case SDLK_LSHIFT: // Space resets position to origin
            m_filterTranslation.setOffset(0, 0);
            break;
        }
    }
    engine->redrawDisplay();
}

void scoreBoardState::mouseWheelScrolled(int x, int y, int which, int timestamp) 
{
    // We grab the position of the center of the screen before the zoom
    int iOldCentreX = m_filterTranslation.filterConvertRealToVirtualXPosition(engine->getWindowWidth() / 2);
    int iOldCentreY = m_filterTranslation.filterConvertRealToVirtualYPosition(engine->getWindowHeight() / 2);

    // Apply zoom
    if (y < 0) {
        m_filterScaling.compress(); // Zoom out
    }
    else if (y > 0) {
        m_filterScaling.stretch(); // Zoom in
    }

    // Now we grab the position after the zoom
    int iNewCentreX = m_filterTranslation.filterConvertRealToVirtualXPosition(engine->getWindowWidth() / 2);
    int iNewCentreY = m_filterTranslation.filterConvertRealToVirtualYPosition(engine->getWindowHeight() / 2);
    
    // Apply a translation offset to keep the view centered
    m_filterTranslation.changeOffset(iNewCentreX - iOldCentreX, iNewCentreY - iOldCentreY);
    
    // Redraw everything
    engine->redrawDisplay();
}
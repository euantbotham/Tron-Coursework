#include "scoreBoardState.h"
#include <iostream>
#include "scoreBoardFilterPoints.h"


void scoreBoardState::enter() {
    // Fill the background with black
    engine->fillBackground(0x000000);
    engine->getForegroundSurface()->setDrawPointsFilter(&m_filterScaling);
	engine->getBackgroundSurface()->setDrawPointsFilter(&m_filterTranslation);
}

void scoreBoardState::foreGroundStrings() {
    // Set the title
    engine->drawForegroundString(525, 200, "Score Board", engine->tronBlue);
    engine->drawForegroundString(525, 250, "Press 'ESC' to go back to the main menu", engine->tronBlue);
    engine->drawForegroundString(525, 270, "Press 'R' to reset zoom and position", engine->tronBlue);

    // Text input box - 30px high 
    engine->drawForegroundLine(520, 300, 620, 300, engine->tronBlue);
    engine->drawForegroundLine(520, 300, 520, 330, engine->tronBlue);
    engine->drawForegroundLine(520, 330, 620, 330, engine->tronBlue);
    engine->drawForegroundLine(620, 300, 620, 330, engine->tronBlue);

    if (inputName.length() > 0) {
        int x = 525;
        int y = 310; // Center vertically in the box (300 + 10)
        unsigned int colour = engine->tronBlue;
        // Draw directly — the foreground surface will automatically filter
        engine->drawForegroundString(x, y, inputName.c_str(), colour);
    }

    // Display current zoom level
    char zoomText[50];
    sprintf(zoomText, "Zoom: %.1fx", m_filterScaling.getZoomX());
    engine->drawForegroundString(525, 350, zoomText, engine->tronBlue);
}

void scoreBoardState::keyPressed(int iKeyCode) {
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
    else if (iKeyCode == SDLK_LEFT || iKeyCode == SDLK_RIGHT ||
        iKeyCode == SDLK_UP || iKeyCode == SDLK_DOWN ||
        iKeyCode == SDLK_LSHIFT) {
        // Handle scrolling with arrow keys and reset with 'r'
        //filter.handleKeyPress(iKeyCode);
    }

    engine->redrawDisplay();
}

void scoreBoardState::mouseWheelScrolled(int x, int y, int which, int timestamp) {
    // We grab the position of the centre of the screen before the zoom
    int iOldCentreX = engine->convertClickedToVirtualPixelXPosition(engine->getWindowWidth() / 2);
    int iOldCentreY = engine->convertClickedToVirtualPixelYPosition(engine->getWindowHeight() / 2);

    if (y < 0)
        m_filterScaling.compress();
    else if (y > 0)
        m_filterScaling.stretch();

    // Now we grab the position after the zoom
    int iNewCentreX = engine->convertClickedToVirtualPixelXPosition(engine->getWindowWidth() / 2);
    int iNewCentreY = engine->convertClickedToVirtualPixelYPosition(engine->getWindowHeight() / 2);


    // Apply a translation to offset so it appears to have zoomed on the centre by moving the old centre back to the centre of the screen
    m_filterTranslation.changeOffset(iNewCentreX - iOldCentreX, iNewCentreY - iOldCentreY);
    // Uncomment the above line to zoom in on centre rather than top left

    // Redraw the background
    engine->redrawDisplay(); // Force total redraw
}
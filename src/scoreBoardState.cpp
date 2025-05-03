#include "scoreBoardState.h"
#include <cstdio> // For sprintf

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
    engine->fillBackground(0x000000);
    // Set the filter chain for drawing
    engine->getForegroundSurface()->setDrawPointsFilter(&m_filterTranslation);
}

void scoreBoardState::foreGroundStrings() 
{
    // Set the title
    engine->drawForegroundString(525, 200, "Score Board", engine->tronBlue);
    engine->drawForegroundString(525, 250, "Press 'ESC' to go back to the main menu", engine->tronBlue);
    
    // Text input box - 30 px high 
    engine->drawForegroundLine(520, 300, 620, 300, engine->tronBlue);
    engine->drawForegroundLine(520, 300, 520, 330, engine->tronBlue);
    engine->drawForegroundLine(520, 330, 620, 330, engine->tronBlue);
    engine->drawForegroundLine(620, 300, 620, 330, engine->tronBlue);
    
    // Display the input name
    if (inputName.length() > 0) {
        engine->drawForegroundString(525, 310, inputName.c_str(), engine->tronBlue);
    }

    // Display zoom information
    char zoomBuf[128];
    sprintf(zoomBuf, "Zoom: %.2fx", m_filterScaling.getZoomX());
    engine->drawForegroundString(525, 350, zoomBuf, engine->tronBlue);
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
            m_filterTranslation.changeOffset(10, 0);
            break;
        case SDLK_RIGHT:
            m_filterTranslation.changeOffset(-10, 0);
            break;
        case SDLK_UP:
            m_filterTranslation.changeOffset(0, 10);
            break;
        case SDLK_DOWN:
            m_filterTranslation.changeOffset(0, -10);
            break;
        case SDLK_SPACE: // Space resets position to origin
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
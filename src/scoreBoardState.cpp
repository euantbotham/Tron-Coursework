#include "scoreBoardState.h"
#include <iostream>
#include "scoreBoardFilterPoints.h"

void scoreBoardState::enter() {
	// Fill the background with black
	engine->fillBackground(0x000000);
	// Set the title
	engine->drawBackgroundString(525, 200, "Score Board", engine->tronBlue);
	engine->drawBackgroundString(525, 250, "Press 'ESC' to go back to the main menu", engine->tronBlue);
	
	//Text input box
	//30 px high 
	engine->drawBackgroundLine(520, 300, 620, 300, engine->tronBlue);
	engine->drawBackgroundLine(520, 300, 520, 330, engine->tronBlue);
	engine->drawBackgroundLine(520, 330, 620, 330, engine->tronBlue);
	engine->drawBackgroundLine(620, 300, 620, 330, engine->tronBlue);
	engine->getForegroundSurface()->setDrawPointsFilter(&filter);
}

void scoreBoardState::foreGroundStrings() {
	
	if (inputName.length() > 0) {
		int x = 525;
		int y = 300;
		unsigned int colour = engine->tronBlue;

		// Draw directly — the foreground surface will automatically filter
		engine->drawForegroundString(x, y, inputName.c_str(), colour);
	}
}


void scoreBoardState::keyPressed(int iKeyCode) {
	if (iKeyCode >= SDLK_a && iKeyCode <= SDLK_z) {
		inputName += static_cast<char> (iKeyCode);
	}
	else if (iKeyCode == SDLK_SPACE) {
		inputName += ' ';
	}
	else if (iKeyCode == SDLK_BACKSPACE) {
		if (inputName.length() > 0) {
			inputName.pop_back();
		}
	}
	else if (iKeyCode == SDLK_LEFT || iKeyCode == SDLK_RIGHT || iKeyCode == SDLK_UP || iKeyCode == SDLK_DOWN) {
		// Handle scrolling with arrow keys
		filter.handleKeyPress(iKeyCode);
	}
	engine->redrawDisplay();
}

void scoreBoardState::mouseWheelScrolled(int x, int y, int which, int timestamp) {
	// Get the center of the screen before zoom
	int iOldCentreX = engine->convertClickedToVirtualPixelXPosition(engine->getWindowWidth() / 2);
	int iOldCentreY = engine->convertClickedToVirtualPixelYPosition(engine->getWindowHeight() / 2);

	// Adjust zoom level based on mouse wheel direction
	if (y < 0) {
		filter.compress(); // Zoom out
	}
	else if (y > 0) {
		filter.stretch(); // Zoom in
	}

	// Get the center of the screen after zoom
	int iNewCentreX = engine->convertClickedToVirtualPixelXPosition(engine->getWindowWidth() / 2);
	int iNewCentreY = engine->convertClickedToVirtualPixelYPosition(engine->getWindowHeight() / 2);

	// Adjust the offset to keep the zoom centered
	//filter.changeOffset(iNewCentreX - iOldCentreX, iNewCentreY - iOldCentreY);

	// Redraw the display
	engine->redrawDisplay();
}

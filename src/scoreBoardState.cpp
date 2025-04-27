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
	else if (iKeyCode == SDLK_0) { // Press '0' to zoom
		// Example: zoom in by 10%, scroll a bit
		filter.setZoom(2); // Zoom in 10%
		filter.scroll(0, 0); // Scroll 5px right, 5px down
	}
	engine->redrawDisplay();
}
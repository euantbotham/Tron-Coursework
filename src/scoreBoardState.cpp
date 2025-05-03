#include "scoreBoardState.h"
#include <iostream>
#include "scoreBoardFilterPoints.h"

void scoreBoardState::enter() {
	// Fill the background with black
	engine->fillBackground(0x000000);
	engine->getForegroundSurface()->setDrawPointsFilter(&filter);
}

void scoreBoardState::foreGroundStrings() {
	
	// Set the title
	engine->drawForegroundString(525, 200, "Score Board", engine->tronBlue);
	engine->drawForegroundString(525, 250, "Press 'ESC' to go back to the main menu", engine->tronBlue);

	//Text input box
	//30 px high 
	engine->drawForegroundLine(520, 300, 620, 300, engine->tronBlue);
	engine->drawForegroundLine(520, 300, 520, 330, engine->tronBlue);
	engine->drawForegroundLine(520, 330, 620, 330, engine->tronBlue);
	engine->drawForegroundLine(620, 300, 620, 330, engine->tronBlue);
	
	
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
	// Get the center of the screen in virtual coordinates BEFORE zoom
	int screenCenterX = engine->getWindowWidth() / 2;
	int screenCenterY = engine->getWindowHeight() / 2;

    // Convert mouse screen coordinates to virtual coordinates before zoom
    int virtualX = filter.filterConvertRealToVirtualXPosition(mouseX);
    int virtualY = filter.filterConvertRealToVirtualYPosition(mouseY);

	// Apply zoom
	if (y < 0) {
		filter.compress(); // Zoom out
	}
	else if (y > 0) {
		filter.stretch(); // Zoom in
	}

	// Get the new virtual position of the same screen center point AFTER zoom
	int newVirtualX = filter.filterConvertRealToVirtualXPosition(screenCenterX);
	int newVirtualY = filter.filterConvertRealToVirtualYPosition(screenCenterY);

	// Calculate the difference and adjust the offset
	// The direction of the adjustment matters - we need to move in the correct direction
	filter.changeOffset(newVirtualX - oldVirtualX, newVirtualY - oldVirtualY);

	// Redraw everything
	engine->redrawDisplay();
}


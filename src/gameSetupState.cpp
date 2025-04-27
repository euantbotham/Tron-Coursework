#include "gameSetupState.h"
#include"gameState.h"
#include <thread>
void gameSetupState::enter()
{
	previousSurface = engine->getBackgroundSurface(); // Store the previous surface
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

	surfaces.reserve(30); // Reserve space for 30 frames

	// Precompute wave base values for all cells for efficiency
    std::vector<std::vector<float>> waveBase(rows, std::vector<float>(columns));
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            waveBase[row][col] = row * 0.2f + col * 0.2f;
        }
    }

    // Create and initialize frames for animation
    for (int i = 0; i < 30; ++i) { // More frames for smoother animation
        DrawingSurface* surface = new DrawingSurface(engine);
        surface->createSurface(screenWidth, screenHeight);
        surface->mySDLLockSurface();

        // Fill background with very dark blue
        surface->drawRectangle(0, 0, screenWidth, screenHeight, 0x080C10);

        // Animation phase (0.0 to 1.0)
        float phase = i / 30.0f;

        // Draw cells
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < columns; ++col) {
                
                // Base color for all cells (very dark blue-gray)
                unsigned int cellColor = 0x101820;

                // Very subtle highlighting based on a moving wave pattern
                float wave = sin(waveBase[row][col] + phase * 6.28318f);
                if (wave > 0.7) {
                    // Very slightly lighter for highlighted cells
                    cellColor = 0x141C24;
                }

                // Draw the cell
                surface->drawRectangle(
                    col * tileWidth, row * tileHeight,
                    (col + 1) * tileWidth - 1, (row + 1) * tileHeight - 1,
                    cellColor);
            }
        }

        // Draw grid lines separately - consistent darkness
        unsigned int lineColor = 0x202830; // Slightly lighter than cells

        // Draw horizontal grid lines
        for (int row = 0; row <= rows; ++row) {
            int y = row * tileHeight;
            surface->drawLine(0, y, screenWidth, y, lineColor);
        }

        // Draw vertical grid lines
        for (int col = 0; col <= columns; ++col) {
            int x = col * tileWidth;
            surface->drawLine(x, 0, x, screenHeight, lineColor);
        }

        // Store the surface
        surfaces.push_back(surface);
        surface->mySDLUnlockSurface();
    }

    // Set the first surface as the background
    engine->setBackgroundSurface(surfaces[0]);

    //Setup the menu tile manager
    tm.setTopLeftPositionOnScreen(525, 250);
    tm.setMapSize(1, 3); // 1 column, 3 rows
    // Define menu options
    tm.setMapValue(0, 0, 7); // New Game
    tm.setMapValue(0, 1, 5); // Load Game
    tm.setMapValue(0, 2, 6); // Settings
}

void gameSetupState::foreGroundStrings()
{
    engine->drawForegroundString(525, 200, "Game Setup", engine->tronBlue);
    tm.drawAllTiles(engine, engine->getForegroundSurface());



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
    if (iButton == SDL_BUTTON_LEFT) {
        if (tm.isValidTilePosition(iX, iY)) {
            int tx = tm.getMapXForScreenX(iX);
            int ty = tm.getMapYForScreenY(iY);

            // Perform actions based on the selected tile
            int selectedOption = tm.getMapValue(tx, ty);
            if (selectedOption == 7) {
                if (previousSurface != nullptr) {
                    engine->setBackgroundSurface(previousSurface);
                }
                // New Game
                engine->setState(new gameState(engine), false, true);

            }
            else if (selectedOption == 5) {
				//Set previous surface to the current background
                if (previousSurface != nullptr) {
                    engine->setBackgroundSurface(previousSurface);
                }
                // Load Game
				std::cout << "button clicked" << std::endl;
                gameState* game = new gameState(engine);
				std::cout << "Loading game..." << std::endl;
                engine->lockBackgroundForDrawing();
                game->initObjects();
				std::cout << "Game objects initialized!" << std::endl;
                if (game->loadGame()){
                    engine->unlockBackgroundForDrawing();
                    engine->setState(game, false, false);
                }
                else {
                    engine->unlockBackgroundForDrawing();
                    delete game;
                    std::cerr << "Error Loading game" << std::endl;
                }
          
                std::cout << "Load Game selected!" << std::endl;
                // Add load game logic here
            }
            else if (selectedOption == 6) {
                // Settings
                std::cout << "Settings selected!" << std::endl;
                // Add settings logic here
            }
        }
    }
}

void gameSetupState::mainLoopPreUpdate()
{
    // Menu Tile update
    int mx = engine->getCurrentMouseX();
    int my = engine->getCurrentMouseY();

    // Highlight the tile if the mouse hovers over it
    if (tm.isValidTilePosition(mx, my)) {
        int tx = tm.getMapXForScreenX(mx);
        int ty = tm.getMapYForScreenY(my);
        tm.tileSelected(engine, engine->getForegroundSurface(), tx, ty);
    }
    else {
        tm.tileSelected(engine, engine->getForegroundSurface(), -1, -1);
    }



    // Only change frame when the tick count is high enough
    if (animationDelay == 0) {
        animationDelay = 2;
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
    //do Nothing
}

gameSetupState::~gameSetupState()
{
    // Clean up surfaces
    for (auto surface : surfaces) {
        delete surface;
    }
    surfaces.clear();
    
	
}
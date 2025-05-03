#include "scoreBoardState.h"
#include <fstream>
#include <sstream>
#include <string>
#include "mainMenuState.h"
scoreBoardState::scoreBoardState(Psyeb10Engine* engine, int score)
    // Initialize filters - first scaling, then translation that points to scaling
    : m_filterScaling(0, 0, engine), 
      m_filterTranslation(0, 0, &m_filterScaling)
{
    this->engine = engine;
    inputName = "";

	this->playerscore = score;
	scoreBoardEntries.resize(10); // Placeholder for 10 entries
	for (int i = 0; i < 10; ++i) {
		scoreBoardEntries[i] = "Player";
	}

	scoreBoardScores.resize(10); // Placeholder for 10 scores
	for (int i = 0; i < 10; ++i) {
		scoreBoardScores[i] = 0;
	}
	nameSaved = false; // Flag to check if the name has been saved

    // initialise outside the top 10
    playerPos = 11;
	// Load the scores from the file
	loadScore();
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
        
		// Highlight the player's entry
        unsigned int colour = i == playerPos ? 0xFFA500 : engine->tronBlue;
        // Entry number
        char entryNumber[4];
        sprintf(entryNumber, "%d.", i + 1);
        engine->drawForegroundString(startX, startY + 50 + i * entryHeight, entryNumber, colour);

        // Player name
        if (i == playerPos) {
            // Use `inputName` for the player's entry
            engine->drawForegroundString(startX + 100, startY + 50 + i * entryHeight, inputName.empty() ? "Player" : inputName.c_str(), colour);
        }
        else {
            // Use the value from `scoreBoardEntries` for other entries
            engine->drawForegroundString(startX + 100, startY + 50 + i * entryHeight, scoreBoardEntries[i].c_str(), colour);
        }
        // Player score (formatted to 4 characters with leading zeros)
        char formattedScore[5];
        sprintf(formattedScore, "%04d", scoreBoardScores[i]);
        engine->drawForegroundString(startX + 300, startY + 50 + i * entryHeight, formattedScore, colour);
    }
}


void scoreBoardState::keyPressed(int iKeyCode) 
{
	// Limit to 10 characters
    if (inputName.length() <= 10) {
        if (iKeyCode >= SDLK_a && iKeyCode <= SDLK_z) {
            inputName += static_cast<char>(iKeyCode);
        }
        else if (iKeyCode == SDLK_SPACE) {
            inputName += ' ';
        }
    }
    if (iKeyCode == SDLK_BACKSPACE) {
        if (inputName.length() > 0) {
            inputName.pop_back();
        }
    }
    else if (iKeyCode == SDLK_TAB) {
		nameSaved = true; // Set the nameSaved flag to true
		scoreBoardEntries[playerPos] = inputName.empty() ? "Player" : inputName; // Use "Player" if no name is entered
		playerPos = 11; // Reset player position to outside the top 10

		saveScore(); // Save the score to the file
	}
	else if (iKeyCode == SDLK_ESCAPE) {
		std::cout << "ESC key pressed" << std::endl;
        // only allow leaving if the name is saved
        if (nameSaved) {
            // Reset to nullptr to remove the filter
            engine->getForegroundSurface()->setDrawPointsFilter(nullptr);
            engine->setState(new mainMenuState(engine), false, true);
            return;
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
			m_filterScaling.resetZoom();
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


void scoreBoardState::loadScore()
{
    bool playerDisplayed = false;
    std::ifstream scoreFile("scores.txt");

    if (!scoreFile.is_open()) {
        std::cerr << "Error: Could not open scores.txt for reading!" << std::endl;
        return;
    }

    // Read the scores and names from the file
    std::string line;
    int count = 0;

    while (std::getline(scoreFile, line) && count < 10) {
        std::istringstream lineStream(line);
        std::string name;
        int score;

        // Attempt to read name and score from the line
        if (!(lineStream >> name >> score)) {
            std::cerr << "Warning: Malformed line in scores.txt: " << line << std::endl;
            continue; // Skip malformed lines
        }

        // If the player's score is higher and hasn't been displayed yet
        if (playerscore > score && !playerDisplayed) {
            scoreBoardEntries[count] = inputName;//inputName.empty() ? "Test" : inputName; // Use "Player" if no name is entered
            scoreBoardScores[count] = playerscore;
            playerDisplayed = true;
			playerPos = count; // Store the player's position
            count++;
        }

        // Ensure we don't exceed the bounds of the scoreboard
        if (count < 10) {
            scoreBoardEntries[count] = name;
            scoreBoardScores[count] = score;
            count++;
        }
    }

    // If the player's score wasn't added and there's still space, add it at the end
    if (!playerDisplayed && count < 10) {
        scoreBoardEntries[count] = inputName;
        scoreBoardScores[count] = playerscore;
		playerPos = count; // Store the player's position
    }
	
    // No need to change scoreboard if playerPos is 11
	if(playerPos == 11) {
        nameSaved = true;
	}
    scoreFile.close();
}

void scoreBoardState::saveScore()
{
	std::ofstream scoreFile("scores.txt", std::ios::app); // Open in append mode
	if (!scoreFile.is_open()) {
		std::cerr << "Error: Could not open scores.txt for writing!" << std::endl;
		return;
	}
	
    for (int i = 0; i < 10; ++i) {
        if (inputName == scoreBoardEntries[i]) {
            scoreFile << scoreBoardEntries[i] << " " << scoreBoardScores[i] << std::endl;
        }
    }

    scoreFile.close(); // Close the file after writing
}
#include "menuTileManager.h"
#include <string>
#include <iostream>

// Helper method to draw tile with consistent styling
void menuTileManager::drawTileBox(
    BaseEngine* pEngine,
    DrawingSurface* pSurface,
    int iStartPositionScreenX, int iStartPositionScreenY,
    bool isHighlighted) const
{
    // Color definitions
    const unsigned int backgroundColor = 0x000000;  // Black background
    const unsigned int normalBorderColor = 0x57E2F9; // Your current cyan color
    const unsigned int highlightBorderColor = 0xFFA500; // Orange for highlight

    // Select border color based on highlight state
    unsigned int borderColor = isHighlighted ? highlightBorderColor : normalBorderColor;

    // Fill background
    pSurface->drawRectangle(
        iStartPositionScreenX,
        iStartPositionScreenY + 5,
        iStartPositionScreenX + getTileWidth() - 1,
        iStartPositionScreenY + getTileHeight() - 6,
        backgroundColor);

    // Draw border lines
    // Top Line
    pSurface->drawLine(
        iStartPositionScreenX,
        iStartPositionScreenY + 5,
        iStartPositionScreenX + getTileWidth() - 1,
        iStartPositionScreenY + 5,
        borderColor);
    // Left side
    pSurface->drawLine(
        iStartPositionScreenX,
        iStartPositionScreenY + 5,
        iStartPositionScreenX,
        iStartPositionScreenY + getTileHeight() - 6,
        borderColor);
    // Right side
    pSurface->drawLine(
        iStartPositionScreenX + getTileWidth() - 1,
        iStartPositionScreenY + 5,
        iStartPositionScreenX + getTileWidth() - 1,
        iStartPositionScreenY + getTileHeight() - 6,
        borderColor);
    // Bottom line
    pSurface->drawLine(
        iStartPositionScreenX,
        iStartPositionScreenY + getTileHeight() - 6,
        iStartPositionScreenX + getTileWidth() - 1,
        iStartPositionScreenY + getTileHeight() - 6,
        borderColor);
}

void menuTileManager::virtDrawTileAt(
    BaseEngine* pEngine,
    DrawingSurface* pSurface,
    int iMapX, int iMapY,
    int iStartPositionScreenX, int iStartPositionScreenY) const
{
    bool isSelected;
    int iMapValue = getMapValue(iMapX, iMapY);
    std::string text;

    // Get menu text based on map value
    switch (iMapValue)
    {
    case 1:
        text = "Resume Game";
        break;
    case 2:
        text = "Save and Quit";
        break;
    case 3:
        text = "Save and Main Menu";
        break;
    case 4:
        text = "Quit Game";
        break;
	case 5:
		text = "Load Game";
		break;
	case 6:
		text = "Settings";
		break;
	case 7:
		text = "New Game";
		break;
	case 8:
		text = "Quit";
		break;
	case 9:
		text = "Back";
        break;
    case 10:
		text = "collisions";
        break;
    default:
        text = "Not Ready";
        break;
    }

    if (iMapX == selectedX && iMapY == selectedY) {
        isSelected = true;
    }
    else {
        isSelected = false;
    }
    // Draw the tile (not highlighted by default)
    drawTileBox(pEngine, pSurface, iStartPositionScreenX, iStartPositionScreenY, isSelected);

    // Draw the text with normal color
    pEngine->drawForegroundString(
        iStartPositionScreenX + 5, // Add a small offset for better appearance
        iStartPositionScreenY + (getTileHeight() / 2) - 6, // Center text vertically
        text.c_str(),
        isSelected ? 0xFFA500 : 0x57E2F9, // Use regular cyan color for text
        NULL);

    // Draw the save game image if the map value corresponds to "Save and Quit"
    if (iMapValue == 2) {
        SimpleImage saveGameImage;
        if (isSelected) {
            saveGameImage = ImageManager::loadImage("saveLogoHighlighted.png", true);
        }
        else {
			saveGameImage = ImageManager::loadImage("saveLogo.png", true);  
        }
        saveGameImage.renderImageWithMask(
            pSurface,
            0, 0, // Source position in the image (top-left corner)
            iStartPositionScreenX + 210, 
            iStartPositionScreenY + 7, 
            saveGameImage.getWidth(), saveGameImage.getHeight() // Width and height of the image to render
        );
	}
    else if (iMapValue == 4) {
        SimpleImage saveGameImage;
        if (isSelected) {
            saveGameImage = ImageManager::loadImage("exitLogoHighlighted.png", true);
        }
        else {
            saveGameImage = ImageManager::loadImage("exitLogo.png", true);
        }
        saveGameImage.renderImageWithMask(
            pSurface,
            0, 0, // Source position in the image (top-left corner)
            iStartPositionScreenX + 210,
            iStartPositionScreenY + 7,
            saveGameImage.getWidth(), saveGameImage.getHeight() // Width and height of the image to render
        );
    }
    else if (iMapValue == 10) {
        // Draw the text with normal color
        pEngine->drawForegroundString(
            iStartPositionScreenX + 190, // Add a small offset for better appearance
            iStartPositionScreenY + (getTileHeight() / 2) - 6, // Center text vertically
            collisions ? "ON" : "OFF",
            isSelected ? 0xFFA500 : 0x57E2F9, // Use regular cyan color for text
            NULL);
        
    }
}

void menuTileManager::tileSelected(
    BaseEngine* pEngine,
    DrawingSurface* pSurface,
    int x, int y)
{
    if (x != this->selectedX || y != this->selectedY) {
        this->selectedX = x;
        this->selectedY = y;
        pEngine->redrawDisplay();
    }
}
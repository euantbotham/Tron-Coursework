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
        iStartPositionScreenY,
        iStartPositionScreenX + getTileWidth() - 1,
        iStartPositionScreenY + getTileHeight() - 1,
        backgroundColor);

    // Draw border lines
    // Top Line
    pSurface->drawLine(
        iStartPositionScreenX,
        iStartPositionScreenY,
        iStartPositionScreenX + getTileWidth() - 1,
        iStartPositionScreenY,
        borderColor);
    // Left side
    pSurface->drawLine(
        iStartPositionScreenX,
        iStartPositionScreenY,
        iStartPositionScreenX,
        iStartPositionScreenY + getTileHeight() - 1,
        borderColor);
    // Right side
    pSurface->drawLine(
        iStartPositionScreenX + getTileWidth() - 1,
        iStartPositionScreenY,
        iStartPositionScreenX + getTileWidth() - 1,
        iStartPositionScreenY + getTileHeight() - 1,
        borderColor);
    // Bottom line
    pSurface->drawLine(
        iStartPositionScreenX,
        iStartPositionScreenY + getTileHeight() - 1,
        iStartPositionScreenX + getTileWidth() - 1,
        iStartPositionScreenY + getTileHeight() - 1,
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
#include "menuTileManager.h"



void menuTileManager::virtDrawTileAt(
    BaseEngine* pEngine,
    DrawingSurface* pSurface,
    int iMapX, int iMapY,
    int iStartPositionScreenX, int iStartPositionScreenY) const
{


    int iMapValue = getMapValue(iMapX, iMapY);

    /*
    Current codes and their meaning, 
    1 = resume game
    2 = save and quit
    3 = save and main menu
    4 = quit game
    */
    pSurface->drawRectangle(iStartPositionScreenX,
        iStartPositionScreenY,
        iStartPositionScreenX + getTileWidth() - 1,
        iStartPositionScreenY + getTileHeight() - 1, 0x000000);
    //Top Line
    pSurface->drawLine(iStartPositionScreenX, iStartPositionScreenY, iStartPositionScreenX + getTileWidth() - 1, iStartPositionScreenY, 0x57E2F9);
    //left side
    pSurface->drawLine(iStartPositionScreenX, iStartPositionScreenY, iStartPositionScreenX , iStartPositionScreenY + getTileHeight() - 1, 0x57E2F9);
    //right side
    pSurface->drawLine(iStartPositionScreenX + getTileWidth() - 1, iStartPositionScreenY, iStartPositionScreenX + getTileWidth() - 1, iStartPositionScreenY + getTileHeight() - 1, 0x57E2F9);
    //Bottom line
    pSurface->drawLine(iStartPositionScreenX, iStartPositionScreenY + getTileHeight() - 1, iStartPositionScreenX + getTileWidth() - 1, iStartPositionScreenY + getTileHeight() - 1, 0x57E2F9);
    pEngine->drawForegroundString(iStartPositionScreenX, iStartPositionScreenY, "test", 0x57E2F9, 0);
}
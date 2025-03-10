#include "header.h"
#include "Psyeb10TileManager.h"


void Psyeb10TileManager::virtDrawTileAt(
    BaseEngine* pEngine,
    DrawingSurface* pSurface,
    int iMapX, int iMapY,
    int iStartPositionScreenX, int iStartPositionScreenY) const 
{
    int iMapValue = getMapValue(iMapX, iMapY);
    unsigned int iColour = 0;
    // zero used to represent innactive tile, 1 for blue, 2 for red
    if (iMapValue != 0) {
        if (iMapValue == 1) {
            iColour = 0x0000FF;
        }else {
            iColour = 0xFF0000;
        }
        //Add a conditional based on if tile value is 1 or 2 it will display blue or orange square
        pSurface->drawRectangle(iStartPositionScreenX,
            iStartPositionScreenY, 
            iStartPositionScreenX + getTileWidth() - 1, 
            iStartPositionScreenY + getTileHeight() - 1, iColour);

    }
}
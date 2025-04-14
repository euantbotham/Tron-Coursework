#include "menuTileManager.h"



void menuTileManager::virtDrawTileAt(
    BaseEngine* pEngine,
    DrawingSurface* pSurface,
    int iMapX, int iMapY,
    int iStartPositionScreenX, int iStartPositionScreenY) const
{
    pSurface->drawRectangle(iStartPositionScreenX,
        iStartPositionScreenY,
        iStartPositionScreenX + getTileWidth() - 1,
        iStartPositionScreenY + getTileHeight() - 1, 0xFFFFFF);
}
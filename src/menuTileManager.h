#pragma once
#include "header.h"
#include "TileManager.h"
class menuTileManager :
    public TileManager
{
public:
    menuTileManager() : TileManager(0, 0, 1, 1) {}
    menuTileManager(int xsize, int ysize, int rows, int columns) : TileManager(xsize, ysize, rows, columns) 
    {

    }
    void virtDrawTileAt(
        BaseEngine* pEngine,
        DrawingSurface* pSurface,
        int iMapX, int iMapY,
        int iStartPositionScreenX, int iStartPositionScreenY) const override;
};


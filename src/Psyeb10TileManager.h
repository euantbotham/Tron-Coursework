#pragma once
#include "TileManager.h"
class Psyeb10TileManager :
    public TileManager
{
    public:
        //Height and width set to 20, and in a 30x30 layout
        Psyeb10TileManager() : TileManager(5, 5, 120, 120)
        {

        }
        virtual void virtDrawTileAt(
            BaseEngine* pEngine,
            DrawingSurface* pSurface,
            int iMapX, int iMapY,
            int iStartPositionScreenX, int iStartPositionScreenY) const override;

};


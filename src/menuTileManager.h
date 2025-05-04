#pragma once
#include "header.h"
#include "TileManager.h"
/* used to store the graphics for the menu,
1 = Resume game,
2 = Save game and quit,
3 = save and main menu,
4 = quit game
*/
class menuTileManager :
    public TileManager
{
public:
    //menuTileManager() : TileManager(0, 0, 1, 1) {}
    menuTileManager(int ysize, int xsize, int rows, int columns) : TileManager(ysize, xsize, rows, columns) 
    {
        selectedX = -1;
        selectedY = -1;
    }
    void virtDrawTileAt(
        BaseEngine* pEngine,
        DrawingSurface* pSurface,
        int iMapX, int iMapY,
        int iStartPositionScreenX, int iStartPositionScreenY) const override;

    void tileSelected(BaseEngine* pEngine,
        DrawingSurface* pSurface, int x, int y);

    void drawTileBox(
        BaseEngine* pEngine,
        DrawingSurface* pSurface,
        int iStartPositionScreenX, int iStartPositionScreenY,
        bool isHighlighted) const;

	void setCollisions(bool collision)
	{
		collisions = collision;
	}
protected:
    // x and y map for selected tile
    int selectedX, selectedY;
    bool collisions = true;
};


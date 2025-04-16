#include "header.h"
#include "Psyeb10Enemy.h"
#include "Psyeb10Engine.h"
#include "DisplayableObject.h"
#include "Psyeb10TileManager.h"
#include <iostream>
#include <queue>
#include <set>
#include <utility>  // for std::pair
#include <vector>
Psyeb10Enemy::Psyeb10Enemy(BaseEngine* pEngine) : Psyeb10Bike(500,150,pEngine, 20,20, 2)
{
	speedX = 0;
	speedY = 1;
}


void Psyeb10Enemy::virtDraw()
{
	getEngine()->drawForegroundRectangle(
		m_iCurrentScreenX, m_iCurrentScreenY,
		m_iCurrentScreenX + m_iDrawWidth - 1,
		m_iCurrentScreenY + m_iDrawHeight - 1,
		0xFFD700);
	getEngine()->drawForegroundRectangle(m_iCurrentScreenX, m_iCurrentScreenY,
		m_iCurrentScreenX + 3,
		m_iCurrentScreenY + 3,
		0x000000);
	getEngine()->drawForegroundRectangle(m_iCurrentScreenX + m_iDrawWidth - 4, m_iCurrentScreenY,
		m_iCurrentScreenX + m_iDrawWidth - 1,
		m_iCurrentScreenY + 3,
		0x000000);
	getEngine()->drawForegroundRectangle(m_iCurrentScreenX, m_iCurrentScreenY + m_iDrawHeight - 4,
		m_iCurrentScreenX + 3,
		m_iCurrentScreenY + m_iDrawHeight - 1,
		0x000000);
	getEngine()->drawForegroundRectangle(m_iCurrentScreenX + m_iDrawWidth - 4, m_iCurrentScreenY + m_iDrawHeight - 4,
		m_iCurrentScreenX + m_iDrawWidth - 1,
		m_iCurrentScreenY + m_iDrawHeight - 1,
		0x000000);
}





void Psyeb10Enemy::virtHandleDeath() 
{
	//TODO add death logic for enemy, Eventually make class so they both a sub of it 
	// Make a new function for subclasses and get this to just call them, make it virt.
	//Add logic to increase score
	engine->resetGame();
	speedX = 0;
	speedY = 1;
}


void Psyeb10Enemy::virtPostMoveLogic()
{
    static int strategicCoolDown = 0; // cooldown counter so doesn't update every tile.

    // Get player position
    DisplayableObject* pPlayerObject = engine->getDisplayableObject(0); // Assuming player is the first object
    int playerX = pPlayerObject->getXCentre();
    int playerY = pPlayerObject->getYCentre();

    // Look ahead in current direction
    bool canContinue = isValidMove(getXCentre() + 5 * speedX, getYCentre() + 5 * speedY);

    // Look ahead further
    bool twoStepsValid = isValidMove(getXCentre() + 10 * speedX, getYCentre() + 10 * speedY);
    bool threeStepsValid = isValidMove(getXCentre() + 15 * speedX, getYCentre() + 15 * speedY);

    

    // If can continue in current direction for 3 steps, do some strategic thinking
    if (canContinue && twoStepsValid && threeStepsValid) {
        if (strategicCoolDown <= 0) {
            // Check if we should change direction based on strategy
            decideStrategicDirection(playerX, playerY);
            strategicCoolDown = 3; // Reset cooldown
		}
		else {
			strategicCoolDown--;
            }
    }
    else {
        // Need to turn - find best direction using floodFill
        int bestDirection = findBestDirection();
		std::cout << "changind direction due to invalid moves: " << std::endl;
        // Apply the direction change
        changeDirection(bestDirection);
    }
}

// Strategic decision making - when to change direction even if current path is valid
void Psyeb10Enemy::decideStrategicDirection(int playerX, int playerY)
{
    // Random chance to be unpredictable (0.5%)
    if (rand() % 1000 < 5) {
		std::cout << "Random direction change" << std::endl;
        int newDirection = rand() % 4;
        // Only change if the new direction is valid
        int newX = getXCentre() + (newDirection == 1 ? 5 : (newDirection == 3 ? -5 : 0));
        int newY = getYCentre() + (newDirection == 2 ? 5 : (newDirection == 0 ? -5 : 0));
        if (isValidMove(newX, newY)) {
            changeDirection(newDirection);
            return;
        }
    }

    // Distance to player
    int distX = playerX - getXCentre();
    int distY = playerY - getYCentre();

    // Current direction
    int currentDirection = -1;
    if (speedX == 0 && speedY == -1) currentDirection = 0; // Up
    if (speedX == 1 && speedY == 0) currentDirection = 1;  // Right
    if (speedX == 0 && speedY == 1) currentDirection = 2;  // Down
    if (speedX == -1 && speedY == 0) currentDirection = 3; // Left

  
    // Check if current path leads to a small area compared to alternatives
    int currentMapX = engine->getTileManager()->getMapXForScreenX(getXCentre() + 5 * speedX);
    int currentMapY = engine->getTileManager()->getMapYForScreenY(getYCentre() + 5 * speedY);
    int currentSpaceSize = floodFill(currentMapX, currentMapY,speedX, speedY, 10,  2000); // Increased max tiles due to smaller tile size

    // Check if there's a significantly better direction
    std::vector<std::pair<int, int>> directions = {
        {0, -1}, {1, 0}, {0, 1}, {-1, 0}
    };

    int bestMove = currentDirection;
    int bestSpaceSize = currentSpaceSize;
    for (int i = 0; i < 4; i++) {
        // Skip current direction
        if (i == currentDirection) continue;

        int testX = getXCentre() + 5 * directions[i].first;
        int testY = getYCentre() + 5 * directions[i].second;

        if (isValidMove(testX, testY)) {
            int testMapX = engine->getTileManager()->getMapXForScreenX(testX);
            int testMapY = engine->getTileManager()->getMapYForScreenY(testY);
            int testSpaceSize = floodFill(testMapX, testMapY,directions[i].first, directions[i].second, 10,  2000);

            // Saves the best direction
            if (testSpaceSize > bestSpaceSize) {
				bestMove = i;   
                bestSpaceSize = testSpaceSize;
            }
        }
    }

    // If this direction has significantly more space (30% more),
    if (bestSpaceSize > currentSpaceSize * 1.3) {
        // Change direction to the best move
        changeDirection(bestMove);
    }



    // Try to intercept or cut off player (10% chance if player is not too far)
    if (rand() % 100 < 10 && abs(distX) < 200 && abs(distY) < 200) {
        int interceptDirection = calculateInterceptDirection(playerX, playerY);
        if (interceptDirection != -1 && interceptDirection != currentDirection) {
            int newX = getXCentre() + (interceptDirection == 1 ? 5 : (interceptDirection == 3 ? -5 : 0));
            int newY = getYCentre() + (interceptDirection == 2 ? 5 : (interceptDirection == 0 ? -5 : 0));
            if (isValidMove(newX, newY)) {
				std::cout << "Changing direction to intercept player" << std::endl;
                changeDirection(interceptDirection);
                return;
            }
        }
    }

    /*
    // Sometimes try to move toward player for pursuit (20% chance if player is far)
    if (rand() % 100 < 20 && (abs(distX) > 100 || abs(distY) > 100)) {
        // Decide whether to prioritize X or Y movement toward player
        if (abs(distX) > abs(distY)) {
            // Move horizontally toward player
            if (distX > 0 && isValidMove(getXCentre() + 5, getYCentre())) {
                speedX = 1;
                speedY = 0;
            }
            else if (distX < 0 && isValidMove(getXCentre() - 5, getYCentre())) {
                speedX = -1;
                speedY = 0;
            }
        }
        else {
            // Move vertically toward player
            if (distY > 0 && isValidMove(getXCentre(), getYCentre() + 5)) {
                speedX = 0;
                speedY = 1;
            }
            else if (distY < 0 && isValidMove(getXCentre(), getYCentre() - 5)) {
                speedX = 0;
                speedY = -1;
            }
        }
    
    }
*/
}

// Calculate direction to intercept player
int Psyeb10Enemy::calculateInterceptDirection(int playerX, int playerY)
{
    // Determine player movement direction based on trail
    // This is a simplified version - you'd need to track player's actual direction

    // For now, just try to cut player off by moving perpendicular to line between us
    int dx = playerX - getXCentre();
    int dy = playerY - getYCentre();

    // We try to choose a perpendicular direction
    if (abs(dx) > abs(dy)) {
        // Player is more horizontal from us, so try vertical movement
        if (dy > 0 && isValidMove(getXCentre(), getYCentre() + 5)) {
            return 2; // Down
        }
        else if (isValidMove(getXCentre(), getYCentre() - 5)) {
            return 0; // Up
        }
    }
    else {
        // Player is more vertical from us, so try horizontal movement
        if (dx > 0 && isValidMove(getXCentre() + 5, getYCentre())) {
            return 1; // Right
        }
        else if (isValidMove(getXCentre() - 5, getYCentre())) {
            return 3; // Left
        }
    }

    return -1; // No good intercept direction
}

// Find the best direction to turn using floodFill
int Psyeb10Enemy::findBestDirection()
{
    // Each direction: 0 = up, 1 = right, 2 = down, 3 = left
    std::vector<std::pair<int, int>> directions = {
        {0, -1}, {1, 0}, {0, 1}, {-1, 0}
    };

    int bestDirection = -1;
    int maxSpace = -1;

    // For each direction, use floodFill to determine available space
    for (int i = 0; i < 4; i++) {
        int nextX = getXCentre() + 5 * directions[i].first;
        int nextY = getYCentre() + 5 * directions[i].second;

        if (isValidMove(nextX, nextY)) {
            // Convert to map coordinates
            int mapX = engine->getTileManager()->getMapXForScreenX(nextX);
            int mapY = engine->getTileManager()->getMapYForScreenY(nextY);

            // Use floodFill to count available space
            int availableSpace = floodFill(mapX, mapY,directions[i].first, directions[i].second, 3, 500);

            // Apply additional scoring factors:

            // 1. Favor directions leading away from walls
            availableSpace = adjustScoreForWallProximity(availableSpace, nextX, nextY, directions[i].first, directions[i].second);

            // 2. Consider directions that might lead toward player (with low weight)
            DisplayableObject* pPlayerObject = engine->getDisplayableObject(0);
            int playerX = pPlayerObject->getXCentre();
            int playerY = pPlayerObject->getYCentre();
            availableSpace = adjustScoreForPlayerProximity(availableSpace, nextX, nextY, playerX, playerY);

            if (availableSpace > maxSpace) {
                maxSpace = availableSpace;
                bestDirection = i;
            }
        }
    }

    // If no valid moves, return current direction (will crash)
    if (bestDirection == -1) {
        if (speedX == 0 && speedY == -1) return 0;
        if (speedX == 1 && speedY == 0) return 1;
        if (speedX == 0 && speedY == 1) return 2;
        if (speedX == -1 && speedY == 0) return 3;
        return 2; // Default to down
    }

    return bestDirection;
}

// Adjust space score based on wall proximity
//TODO this does not acc check the wall
int Psyeb10Enemy::adjustScoreForWallProximity(int baseScore, int x, int y, int dirX, int dirY)
{
    int wallPenalty = 0;

    // Check for walls on sides of this path
    int perpDirX = -dirY; // Perpendicular direction
    int perpDirY = dirX;

    // Left wall check
    if (!isValidMove(x + 5 * perpDirX, y + 5 * perpDirY)) {
        wallPenalty += 5;
    }

    // Right wall check
    if (!isValidMove(x - 5 * perpDirX, y - 5 * perpDirY)) {
        wallPenalty += 5;
    }

    // Prefer directions with space on both sides
    return baseScore - wallPenalty;
}

// Adjust score based on player position (slight preference to directions toward player)
int Psyeb10Enemy::adjustScoreForPlayerProximity(int baseScore, int x, int y, int playerX, int playerY)
{
    // Calculate vector to player
    int toPlayerX = playerX - x;
    int toPlayerY = playerY - y;

    // Calculate distance to player
    double distToPlayer = sqrt(toPlayerX * toPlayerX + toPlayerY * toPlayerY);

    // Small bonus for directions that get closer to player
    if (distToPlayer < 300) {
        return baseScore + 3;
    }

    return baseScore;
}

int Psyeb10Enemy::floodFill(int startMapX, int startMapY, int dirX, int dirY, int maxScope, int maxTiles) {
	//int maxScope = 7; //max scope for tunnel vision
    
    std::queue<std::pair<int, int>> q;
    std::set<std::pair<int, int>> visited;

    int firstX = startMapX + dirX;
    int firstY = startMapY + dirY;

    // Make sure the first tile in the direction is valid
    if (firstX < 0 || firstX >= 120 || firstY < 0 || firstY >= 120) return 0;
    if (engine->getTileManager()->getMapValue(firstX, firstY) != 0) return 0;

    q.push({ firstX, firstY });
    visited.insert({ firstX, firstY });

    int count = 0;

    std::vector<std::pair<int, int>> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    while (!q.empty() && count < maxTiles) {
        std::pair<int, int> current = q.front();
        q.pop();
        count++;

        for (auto& d : directions) {
            int nx = current.first + d.first;
            int ny = current.second + d.second;

            std::pair<int, int> next = { nx, ny };

            // Skip if out of bounds
            if (nx < 0 || nx >= 120 || ny < 0 || ny >= 120) continue;
            if (visited.find(next) != visited.end()) continue;
            visited.insert(next);
            if (engine->getTileManager()->getMapValue(nx, ny) != 0) continue;

            // Constrain movement: do not go behind the starting tile
            // For dirX (horizontal movement)
            if (dirX > 0 && nx < startMapX) continue;
            if (dirX < 0 && nx > startMapX) continue;

            // For dirY (vertical movement)
            if (dirY > 0 && ny < startMapY) continue;
            if (dirY < 0 && ny > startMapY) continue;

            // checks scope to ensure it chooses best path
            if (dirX != 0 && std::abs(ny - startMapY) > maxScope) continue;
            if (dirY != 0 && std::abs(nx - startMapX) > maxScope) continue;


            q.push(next);
        }
    }

    return count;
}


// Helper to change direction based on a direction code
void Psyeb10Enemy::changeDirection(int direction)
{
    switch (direction) {
    case 0: // Up
        speedX = 0;
        speedY = -1;
        break;
    case 1: // Right
        speedX = 1;
        speedY = 0;
        break;
    case 2: // Down
        speedX = 0;
        speedY = 1;
        break;
    case 3: // Left
        speedX = -1;
        speedY = 0;
        break;
    }
}

// Update the isValidMove function to work with 5x5 tiles
bool Psyeb10Enemy::isValidMove(int x, int y) {
    // Convert screen coordinates to map coordinates
    int mapX = engine->getTileManager()->getMapXForScreenX(x);
    int mapY = engine->getTileManager()->getMapYForScreenY(y);

    // Check if coordinates are within map boundaries
    if (mapX >= 0 && mapX < 120 && mapY >= 0 && mapY < 120) {
        // Check if the tile is free (value = 0)
        if (engine->getTileManager()->getMapValue(mapX, mapY) == 0) {
            return true;
        }
    }
    return false;
}

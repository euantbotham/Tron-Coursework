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

void Psyeb10Enemy::decideStrategicDirection(int playerX, int playerY)
{
    // Current direction
    int currentDirection = -1;
    if (speedX == 0 && speedY == -1) currentDirection = 0; // Up
    if (speedX == 1 && speedY == 0) currentDirection = 1;  // Right
    if (speedX == 0 && speedY == 1) currentDirection = 2;  // Down
    if (speedX == -1 && speedY == 0) currentDirection = 3; // Left

    // Check if current path leads to a small area compared to alternatives
    int currentMapX = engine->getTileManager()->getMapXForScreenX(getXCentre() + 5 * speedX);
    int currentMapY = engine->getTileManager()->getMapYForScreenY(getYCentre() + 5 * speedY);
    int currentSpaceSize = floodFill(currentMapX, currentMapY, speedX, speedY, 15, 2000);

    // Check if there's a significantly better direction
    std::vector<std::pair<int, int>> directions = {
        {0, -1}, {1, 0}, {0, 1}, {-1, 0}
    };

    int bestMove = currentDirection;
    int bestSpaceSize = currentSpaceSize;
    bool foundBetterDirection = false;

    // Store all direction scores for logging
    std::vector<int> directionScores(4, 0);
    directionScores[currentDirection] = currentSpaceSize;

    for (int i = 0; i < 4; i++) {
        // Skip current direction and reverse direction (already evaluated)
        if (i == currentDirection || i == (currentDirection + 2) % 4) continue;

        int testX = getXCentre() + 5 * directions[i].first;
        int testY = getYCentre() + 5 * directions[i].second;

        if (isValidMove(testX, testY)) {
            int testMapX = engine->getTileManager()->getMapXForScreenX(testX);
            int testMapY = engine->getTileManager()->getMapYForScreenY(testY);
            int testSpaceSize = floodFill(testMapX, testMapY, directions[i].first, directions[i].second, 15, 2000);

            directionScores[i] = testSpaceSize;

            // Save the best direction
            if (testSpaceSize > bestSpaceSize * 1.3) { // Only 30% better is enough to change
                bestMove = i;
                bestSpaceSize = testSpaceSize;
                foundBetterDirection = true;
            }
        }
    }

    // If we found a significantly better direction, change to it
    if (foundBetterDirection) {
        std::cout << "Strategic direction change: Dir " << currentDirection
            << " (score " << currentSpaceSize << ") -> Dir " << bestMove
            << " (score " << bestSpaceSize << ")" << std::endl;

        changeDirection(bestMove);
        return; // Exit early - prioritize open space over player chasing
    }

    // Only consider player chasing if current path has plenty of space
    // This prevents suicide moves
    if (currentSpaceSize > 200) { // Must have substantial space ahead to consider chasing
        // Distance to player
        int distX = playerX - getXCentre();
        int distY = playerY - getYCentre();
        double distToPlayer = sqrt(distX * distX + distY * distY);

        // Player must be within chase range but not too close
        if (distToPlayer < 180 && distToPlayer > 50) {
            // Determine which direction would move closer to player
            int horizontalDir = (distX > 0) ? 1 : 3; // Right or Left
            int verticalDir = (distY > 0) ? 2 : 0;   // Down or Up

            // Choose the best direction based on available space
            int chaseDirs[2] = { -1, -1 };

            // Check which axis (horizontal or vertical) has the player further away
            if (abs(distX) > abs(distY)) {
                chaseDirs[0] = horizontalDir; // Primary chase direction
                chaseDirs[1] = verticalDir;   // Secondary chase direction
            }
            else {
                chaseDirs[0] = verticalDir;   // Primary chase direction
                chaseDirs[1] = horizontalDir; // Secondary chase direction
            }

            // Try primary chase direction first, then secondary
            for (int i = 0; i < 2; i++) {
                int chaseDir = chaseDirs[i];

                // Skip if this is current direction or reverse of current
                if (chaseDir == currentDirection || chaseDir == (currentDirection + 2) % 4) {
                    continue;
                }

                // Verify this direction has been evaluated and has good space
                if (directionScores[chaseDir] > 0) {
                    // CRITICAL: Only chase if this direction has at least 75% of the space 
                    // of the current direction to avoid boxing in
                    if (directionScores[chaseDir] >= currentSpaceSize * 0.85) {
                        std::cout << "Safe player chase: Dir " << currentDirection
                            << " -> Dir " << chaseDir
                            << " (space: " << directionScores[chaseDir]
                            << " vs current: " << currentSpaceSize << ")" << std::endl;

                            changeDirection(chaseDir);
                            return;
                    }
                }
            }
        }
    }

    // Random direction change occasionally (5% chance) if we have good space ahead
    // This adds unpredictability and helps avoid getting stuck in patterns
    if (currentSpaceSize > 300 && (rand() % 100 < 5)) {
        // Pick a random direction that's not current or reverse
        std::vector<int> possibleDirs;
        for (int i = 0; i < 4; i++) {
            if (i != currentDirection && i != (currentDirection + 2) % 4 &&
                directionScores[i] >= currentSpaceSize * 0.6) { // Must have decent space
                possibleDirs.push_back(i);
            }
        }

        if (!possibleDirs.empty()) {
            int randomDir = possibleDirs[rand() % possibleDirs.size()];
            std::cout << "Random direction change for unpredictability: " << randomDir << std::endl;
            changeDirection(randomDir);
        }
    }
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

int Psyeb10Enemy::findBestDirection()
{
    // Each direction: 0 = up, 1 = right, 2 = down, 3 = left
    std::vector<std::pair<int, int>> directions = {
        {0, -1}, {1, 0}, {0, 1}, {-1, 0}
    };

    int bestDirection = -1;
    int maxSpace = -1;

    // Get current direction
    int currentDirection = -1;
    if (speedX == 0 && speedY == -1) currentDirection = 0; // Up
    if (speedX == 1 && speedY == 0) currentDirection = 1;  // Right
    if (speedX == 0 && speedY == 1) currentDirection = 2;  // Down
    if (speedX == -1 && speedY == 0) currentDirection = 3; // Left

    // For each direction, use floodFill to determine available space
    for (int i = 0; i < 4; i++) {
        int nextX = getXCentre() + 5 * directions[i].first;
        int nextY = getYCentre() + 5 * directions[i].second;

        if (isValidMove(nextX, nextY)) {
            // Convert to map coordinates
            int mapX = engine->getTileManager()->getMapXForScreenX(nextX);
            int mapY = engine->getTileManager()->getMapYForScreenY(nextY);

            // Increase maxScope parameter to allow wider exploration
            int availableSpace = floodFill(mapX, mapY, directions[i].first, directions[i].second, 10, 1500);

            // Bonus for continuing in same direction (avoids erratic movement)
            if (i == currentDirection) {
                availableSpace = availableSpace * 1.1; // 10% bonus for current direction
            }

            // Penalty for complete direction reversal (avoids ping-pong movement)
            if ((i + 2) % 4 == currentDirection) {
                availableSpace = availableSpace * 0.8; // 20% penalty for reversing
            }

            // Consider directions that might lead toward player (with low weight)
            DisplayableObject* pPlayerObject = engine->getDisplayableObject(0);
            int playerX = pPlayerObject->getXCentre();
            int playerY = pPlayerObject->getYCentre();
            availableSpace = adjustScoreForPlayerProximity(availableSpace, nextX, nextY, playerX, playerY);

            // Track the best option
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

    // Log the chosen direction and available space for debugging
    std::cout << "Chose direction " << bestDirection << " with space score " << maxSpace << std::endl;

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
    std::queue<std::pair<int, int>> q;
    std::set<std::pair<int, int>> visited;

    // Check the first tile in the intended direction
    int firstX = startMapX + dirX;
    int firstY = startMapY + dirY;

    // Make sure the first tile in the direction is valid
    if (firstX < 0 || firstX >= 120 || firstY < 0 || firstY >= 120) return 0;
    if (engine->getTileManager()->getMapValue(firstX, firstY) != 0) return 0;

    q.push({ firstX, firstY });
    visited.insert({ firstX, firstY });

    int count = 0;
    int maxWidth = 0;  // Track the maximum width of the open space
    int maxHeight = 0; // Track the maximum height of the open space
    int minX = firstX, maxX = firstX;
    int minY = firstY, maxY = firstY;

    std::vector<std::pair<int, int>> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    while (!q.empty() && count < maxTiles) {
        std::pair<int, int> current = q.front();
        q.pop();
        count++;

        // Update the boundaries of discovered open space
        minX = std::min(minX, current.first);
        maxX = std::max(maxX, current.first);
        minY = std::min(minY, current.second);
        maxY = std::max(maxY, current.second);

        for (auto& d : directions) {
            int nx = current.first + d.first;
            int ny = current.second + d.second;

            // Skip if out of bounds or already visited
            if (nx < 0 || nx >= 120 || ny < 0 || ny >= 120) continue;
            if (visited.find({ nx, ny }) != visited.end()) continue;

            // Skip if tile is not empty
            if (engine->getTileManager()->getMapValue(nx, ny) != 0) continue;

            // Mark as visited
            visited.insert({ nx, ny });

            // Use a more relaxed constraint to allow exploration of more open space
            // Only restrict going backwards in primary direction of movement
            if (dirX > 0 && nx < startMapX - maxScope) continue;
            if (dirX < 0 && nx > startMapX + maxScope) continue;
            if (dirY > 0 && ny < startMapY - maxScope) continue;
            if (dirY < 0 && ny > startMapY + maxScope) continue;

            // Allow wider exploration perpendicular to movement direction
            // Increased scope multiplier to explore more widely
            int perpendicularScope = maxScope * 4;
            if (dirX != 0 && std::abs(ny - startMapY) > perpendicularScope) continue;
            if (dirY != 0 && std::abs(nx - startMapX) > perpendicularScope) continue;

            q.push({ nx, ny });
        }
    }

    // Calculate dimensions of the open space
    int width = maxX - minX + 1;
    int height = maxY - minY + 1;
    int openArea = width * height;

    // Calculate a weighted score that favors both:
    // 1. Total tiles discovered (count)
    // 2. The dimensions of the open area (to prefer wide open spaces)
    int weightedScore = count;

    // Bonus for large open areas (avoid narrow corridors)
    if (width > 5 && height > 5) {
        weightedScore += (width * height) / 20; // Add bonus based on area size
    }

    // Penalty for very narrow paths
    if (width <= 2 || height <= 2) {
        weightedScore = weightedScore / 2; // Reduce score for narrow paths
    }

    // If we found many tiles but in a very constrained space, this might be a trap
    float density = float(count) / (width * height);
    if (density > 0.8 && openArea < 100) {
        // This might be a small enclosed area - penalize
        weightedScore -= 20;
    }

    return std::max(1, weightedScore); // Ensure we return at least 1 if path is valid
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
        if (engine->getTileManager()->getMapValue(mapX, mapY) <= 0) {
            return true;
        }
    }
    return false;
}

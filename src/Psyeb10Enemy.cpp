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
	changeDirection(2); // Start moving down

    for (int i = 1; i <= 6; ++i) {
        std::string imageName = "enemy" + std::to_string(i) + ".png";
        SimpleImage image = ImageManager::loadImage(imageName, false);
        image.setTransparencyColour(0); // Set transparency color if needed
        animationImages.push_back(image); // Add the image to the vector

    }
}



void Psyeb10Enemy::virtHandleDeath() 
{
	//TODO add death logic for enemy, Eventually make class so they both a sub of it 
	// Make a new function for subclasses and get this to just call them, make it virt.
	//Add logic to increase score
	engine->resetGame();
	//Set direction to down
    changeDirection(2);
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

    // Check current path space
    int currentMapX = engine->getTileManager()->getMapXForScreenX(getXCentre() + 5 * speedX);
    int currentMapY = engine->getTileManager()->getMapYForScreenY(getYCentre() + 5 * speedY);
    int currentSpaceSize = floodFill(currentMapX, currentMapY, speedX, speedY, 15, 2000);

    // Evaluate all potential directions
    std::vector<std::pair<int, int>> directions = {
        {0, -1}, {1, 0}, {0, 1}, {-1, 0}
    };

    // Store all direction scores and validity
    std::vector<int> directionScores(4, 0);
    std::vector<bool> validDirection(4, false);
    directionScores[currentDirection] = currentSpaceSize;
    validDirection[currentDirection] = true;

    // Player distance metrics
    int distX = playerX - getXCentre();
    int distY = playerY - getYCentre();
    double distToPlayer = sqrt(distX * distX + distY * distY);
    bool playerInRange = (distToPlayer < 160 && distToPlayer > 70); // Adjusted chase range

    // Determine chase direction preferences
    int horizontalChaseDir = (distX > 0) ? 1 : 3; // Right or Left
    int verticalChaseDir = (distY > 0) ? 2 : 0;   // Down or Up

    // Calculate desirability score for all directions
    for (int i = 0; i < 4; i++) {
        // Skip reverse direction (prevents immediate backward movement)
        if (i == (currentDirection + 2) % 4) continue;

        // Check if this move is valid
        int testX = getXCentre() + 5 * directions[i].first;
        int testY = getYCentre() + 5 * directions[i].second;

        if (isValidMove(testX, testY)) {
            validDirection[i] = true;
            int testMapX = engine->getTileManager()->getMapXForScreenX(testX);
            int testMapY = engine->getTileManager()->getMapYForScreenY(testY);
            int testSpaceSize = floodFill(testMapX, testMapY, directions[i].first, directions[i].second, 15, 2000);
            directionScores[i] = testSpaceSize;
        }
    }

    // Create composite scores that balance space and chase factors
    std::vector<double> compositeScores(4, 0.0);
    for (int i = 0; i < 4; i++) {
        if (!validDirection[i]) continue;

        // Base score is the space score
        compositeScores[i] = directionScores[i];

        // Add chase bonus if the player is in range
        if (playerInRange && directionScores[i] >= currentSpaceSize * 0.95) {
            if (i == horizontalChaseDir || i == verticalChaseDir) {
                compositeScores[i] *= 1.1; // 10% bonus for primary chase direction
            }
        }

        // Prefer staying in current direction slightly (momentum)
        if (i == currentDirection) {
            compositeScores[i] *= 1.05; // 5% same direction bonus
        }
    }

    // Find best direction based on composite score
    int bestDirection = currentDirection;
    double bestScore = compositeScores[currentDirection];

    for (int i = 0; i < 4; i++) {
        if (validDirection[i] && compositeScores[i] > bestScore) {
            bestDirection = i;
            bestScore = compositeScores[i];
        }
    }

    // Only change direction if we found a better one
    if (bestDirection != currentDirection) {
        std::cout << "Direction change: Dir " << currentDirection
            << " -> Dir " << bestDirection
            << " (score: " << compositeScores[bestDirection] << ")" << std::endl;

        changeDirection(bestDirection);
    }
}


int Psyeb10Enemy::findBestDirection()
{
    // Each direction: 0 = up, 1 = right, 2 = down, 3 = left
    std::vector<std::pair<int, int>> directions = {
        {0, -1}, {1, 0}, {0, 1}, {-1, 0}
    };

    int bestDirection = -1;
    double bestScore = -1.0;

    // Get current direction
    int currentDirection = -1;
    if (speedX == 0 && speedY == -1) currentDirection = 0; // Up
    if (speedX == 1 && speedY == 0) currentDirection = 1;  // Right
    if (speedX == 0 && speedY == 1) currentDirection = 2;  // Down
    if (speedX == -1 && speedY == 0) currentDirection = 3; // Left

    // Player position
    DisplayableObject* pPlayerObject = engine->getDisplayableObject(0);
    int playerX = pPlayerObject->getXCentre();
    int playerY = pPlayerObject->getYCentre();

    // Player distance metrics
    int distX = playerX - getXCentre();
    int distY = playerY - getYCentre();
    double distToPlayer = sqrt(distX * distX + distY * distY);

    // Evaluate all potential directions
    for (int i = 0; i < 4; i++) {
        // Strictly exclude reverse direction
        if (i == (currentDirection + 2) % 4) {
            std::cout << "Skipping reverse direction: " << i << std::endl;
            continue;
        }

        // Check if this move is valid
        int nextX = getXCentre() + 5 * directions[i].first;
        int nextY = getYCentre() + 5 * directions[i].second;

        if (isValidMove(nextX, nextY)) {
            // Convert to map coordinates
            int mapX = engine->getTileManager()->getMapXForScreenX(nextX);
            int mapY = engine->getTileManager()->getMapYForScreenY(nextY);

            // Dynamically adjust floodFill scope based on current space size
            int floodFillScope = (distToPlayer > 150) ? 20 : 10; // Larger scope if player is far
            int availableSpace = floodFill(mapX, mapY, directions[i].first, directions[i].second, floodFillScope, 1500);

            // If all directions are confined, increase floodFill scope
            if (availableSpace < 50) {
                availableSpace = floodFill(mapX, mapY, directions[i].first, directions[i].second, 30, 2000);
            }

            // Calculate composite score
            double score = availableSpace;

            // Add bonus for continuing in the same direction (momentum)
            if (i == currentDirection) {
                score *= 1.1; // 10% bonus for staying in the same direction
            }


            // Add randomness to escape patterns (10% chance)
            if (rand() % 100 < 10) {
                score *= 1.2; // 20% random bonus
            }

            // Track the best direction
            if (score > bestScore) {
                bestScore = score;
                bestDirection = i;
            }
        }
    }

    // If no valid moves, prioritize the direction with the most open space
    if (bestDirection == -1) {
        std::cout << "No valid moves found. Prioritizing open space." << std::endl;
        for (int i = 0; i < 4; i++) {
            if (i == (currentDirection + 2) % 4) continue; // Skip reverse direction

            int nextX = getXCentre() + 5 * directions[i].first;
            int nextY = getYCentre() + 5 * directions[i].second;

            if (isValidMove(nextX, nextY)) {
                int mapX = engine->getTileManager()->getMapXForScreenX(nextX);
                int mapY = engine->getTileManager()->getMapYForScreenY(nextY);
                int availableSpace = floodFill(mapX, mapY, directions[i].first, directions[i].second, 30, 2000);

                if (availableSpace > bestScore) {
                    bestScore = availableSpace;
                    bestDirection = i;
                }
            }
        }
    }

    // If still no valid moves, log an error and stay in the current direction
    if (bestDirection == -1) {
        std::cout << "ERROR: No valid moves available. Staying in current direction: " << currentDirection << std::endl;
        return currentDirection;
    }

    // Log the chosen direction and score for debugging
    std::cout << "Best direction: " << bestDirection << " with score: " << bestScore << std::endl;

    return bestDirection;
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

#include "gameState.h"
#include "MainCharacter.h"
#include <sstream>
#include <string>
#include <fstream>
#include "Psyeb10Enemy.h"
#include "pauseState.h"
#include "scoreBoardState.h"
#include "UtilCollisionDetection.h"

void gameState::enter()
{
	// Setu background grid drawing
	drawBackground();

	//Setup tile manager
	for (int i = 0; i < 120; i++)
		for (int j = 0; j < 120; j++)
			tm.setMapValue(i, j, 0);
	tm.setTopLeftPositionOnScreen(350, 100);
	tm.drawAllTiles(engine, engine->getBackgroundSurface());
	this->isDisplayed = true;
}


void gameState::foreGroundStrings()
{
	std::stringstream p1Lives, p1Score;
	p1Lives << "lives : " << mainChar->getLives();
	std::string livesMessage = p1Lives.str();
	engine->drawForegroundString(875, 20, livesMessage.c_str(), 0x57E2F9, 0);


	p1Score << "SCORE : " << gameScore;
	livesMessage = p1Score.str();

	engine->drawForegroundString(300, 20, livesMessage.c_str(), 0x57E2F9, 0);
}


void gameState::initObjects()
{
	engine->notifyObjectsAboutKeys(true);
	engine->drawableObjectsChanged();
	engine->destroyOldObjects(true);
	engine->createObjectArray(1);
	this->mainChar = new MainCharacter(engine);
	engine->storeObjectInArray(0, mainChar);
	this->enemyVec.push_back(new Psyeb10Enemy(engine, 500, 150, 2));
	engine->appendObjectToArray(enemyVec[0]);
	engine->setAllObjectsVisible(true);
	currentEnemies = 1; // Set the number of enemies to 1
}

void gameState::reset()
{
	if (mainChar->getLives() <= 0) {
		isDisplayed = false;
		engine->setState(new scoreBoardState(engine, gameScore), false, true);
		return;
	}
	
	engine->lockBackgroundForDrawing();
	enter();
	engine->unlockBackgroundForDrawing();
	engine->redrawDisplay();

	//Reset bikes to start positions and directions
	mainChar->resetPos();
	mainChar->changeDirection(0);

	// Reset Bike startPositions and directions
	for (auto& bike : enemyVec) {
		bike->resetPos();
		bike->changeDirection(2);
	}
}

void gameState::mouseDown(int iButton, int iX, int iY) {
	if (iButton == SDL_BUTTON_RIGHT)
	{
		engine->pause();
		this->mainChar->setPaused(true);
		//Pause all active enemies
		for (auto& enemy : enemyVec) {
			enemy->setPaused(true);
		}
		this->isDisplayed = false;
		engine->setState(new pauseState(engine), true , true);
		
	}
	else if (iButton == SDL_BUTTON_LEFT)
	{
		recieveUpdate(enemyVec.back()->getBikeValue());
	}
}

void gameState::mainLoopPreUpdate() {
	int time = engine->getModifiedTime() / 1000;
	if (time > lastTick)
	{
		lastTick = time;
		gameScore += 10;
	}

	for (int i = 0; i < currentEnemies +1; i++) {
		for (int j = i + 1; j < currentEnemies + 1; j++) {
			if (i != j) {
				Psyeb10Bike* bike1 = dynamic_cast<Psyeb10Bike*>(engine->getDisplayableObject(i));
				Psyeb10Bike* bike2 = dynamic_cast<Psyeb10Bike*>(engine->getDisplayableObject(j));
				int bX1, bY1, bX2, bY2;
				bX1 = bike1->getXCentre() - bike1->getDrawWidth() / 2;
				bY1 = bike1->getYCentre() - bike1->getDrawHeight() / 2;
				bX2 = bike2->getXCentre() - bike2->getDrawWidth() / 2;
				bY2 = bike2->getYCentre() - bike2->getDrawHeight() / 2;
				
				if (checkBikeCollision(bike1->getImage(), bX1, bY1, bike1->getDirection(),
					bike2->getImage(), bX2, bY2, bike2->getDirection())) {
					std::cout << "Collision detected!" << std::endl;
					reset();
					// Handle collision (e.g., reduce lives, reset positions, etc.)
				}
			}
		}
		
	}

	//Get directions for rotations
	/*
	int b1Dir = mainChar->getDirection();
	int b2Dir = enemyVec[0]->getDirection();
	

	if (checkBikeCollision(mainChar->getImage(), mainChar->getXCentre() - mainChar->getDrawWidth()/2, mainChar->getYCentre() - mainChar->getDrawHeight()/2, b1Dir,
		enemyVec[0]->getImage(), enemyVec[0]->getXCentre() - enemyVec[0]->getDrawWidth()/2, enemyVec[0]->getYCentre() - enemyVec[0]->getDrawHeight()/2 ,b2Dir)) {
		std::cout << "Collision detected!" << std::endl;
		reset();
		// Handle collision (e.g., reduce lives, reset positions, etc.)
	}
	*/
}


void gameState::reEntry()
{
	drawBackground();
	engine->createObjectArray(1);
	engine->storeObjectInArray(0, mainChar);
	mainChar->setPaused(false);
	for (auto& enemy : enemyVec) {
		engine->appendObjectToArray(enemy);
		enemy->setPaused(false);
	}
	tm.drawAllTiles(engine, engine->getBackgroundSurface());
	this->isDisplayed = true;
}


void gameState::drawBackground()
{
	//set all to black initally
	engine->fillBackground(0x000000);
	//TODO add lines to make a grid
	// grey walls
	engine->drawBackgroundRectangle(325, 75, 975, 725, 0x2C3E50);

	//Blue background
	engine->drawBackgroundRectangle(350, 100, 950, 700, 0x000822);

	//Grid vert lines for background
	for (int i = 0; i < 13; i++)
	{
		engine->drawBackgroundThickLine(gridstartX + 50 * i, gridstartY + 1, gridstartX + 50 * i, gridEndY - 1, 0x000000, 2);
	}

	//Grid horizontal lines for background
	for (int i = 0; i < 13; i++)
	{
		engine->drawBackgroundThickLine(gridstartX + 1, gridstartY + 50 * i, gridEndX - 1, gridstartY + 50 * i, 0x000000, 2);
	}

	//background text
	engine->drawBackgroundString(710, 25, "C++", 0x57E2F9, 0);


	// Background Image used as game logo
	SimpleImage image = ImageManager::loadImage("tronLogo.png", true);
	image.renderImageWithMask(engine->getBackgroundSurface(), 0, 0, 575, 20, image.getWidth(), image.getHeight());
}


gameState::~gameState()
{
	// delete mainchar and enemy if not currently helf in displayable object container
	if (!(this->isDisplayed)) {
		delete mainChar;
		for (auto& enemy : enemyVec) {
			delete enemy;
		}
	}
}

MainCharacter* gameState::getmainChar()const {
	return mainChar;
}

int gameState::getGameScore()const {
	return gameScore;
}


void gameState::recieveUpdate(int code) {
	// Destroy Bike that has been deleted and remove from vectors
	Psyeb10Enemy* toDelete = nullptr;
	if (currentEnemies > 0) {
		for (Psyeb10Enemy* enemy : enemyVec) {
			if (enemy->getBikeValue() == code) {
				// Only seemed to work when passed a DisplayableObject pointer
				engine->drawableObjectsChanged();
				engine->removeDisplayableObject(enemy);
				enemy->setVisible(false);
				enemy->setPaused(true);


				//enemyVec.erase(std::remove(enemyVec.begin(), enemyVec.end(), enemy), enemyVec.end());
				//delete enemy;
				toDelete = enemy;

				currentEnemies--;
				cleanTileManager(code);

				engine->lockBackgroundForDrawing();
				drawBackground();
				tm.drawAllTiles(engine, engine->getBackgroundSurface());
				engine->unlockBackgroundForDrawing();
				engine->redrawDisplay();

				std::cout << "Enemy " << code << " destroyed!" << std::endl;
				break;
			}

		}
		if (toDelete != nullptr) {
			// Erase the exact pointer from the vector
			enemyVec.erase(std::remove(enemyVec.begin(), enemyVec.end(), toDelete), enemyVec.end());
			delete toDelete;  // Free memory after erase
		}
	}
	if (currentEnemies == 0) {
		if (currentLevel < 2) {
			Psyeb10Enemy* enemy = new Psyeb10Enemy(engine, 500, 150, 2);
			reset();
			enemyVec.push_back(enemy);
			engine->appendObjectToArray(enemy);
			currentEnemies = 1;
		}
		else if (currentLevel < 5) {
			reset();
			Psyeb10Enemy* enemy = new Psyeb10Enemy(engine, 500, 150, 2);
			enemyVec.push_back(enemy);
			engine->appendObjectToArray(enemy);

			enemy = new Psyeb10Enemy(engine, 650, 150, 3);
			this->enemyVec.push_back(enemy);
			engine->appendObjectToArray(enemy);
			currentEnemies = 2;
		}
		
		else {
			isDisplayed = false;
			engine->setState(new scoreBoardState(engine, gameScore),false, true);
			return;
		}
		currentLevel++;
	}
}

bool gameState::loadGame() {
	engine->notifyObjectsAboutKeys(true);
	engine->drawableObjectsChanged();
	engine->destroyOldObjects(true);
	// Open the tile data file
	std::ifstream tileFile("tile_data.csv");
	if (!tileFile.is_open()) {
		std::cerr << "Error: Could not open tile_data.csv for reading!" << std::endl;
		return false;
	}

	// Read the tile data and populate the TileManager
	int width = tm.getMapWidth();
	int height = tm.getMapHeight();
	int y = 0;

	std::string line;
	while (std::getline(tileFile, line) && y < height) {
		std::stringstream ss(line);
		std::string value;
		int x = 0;

		while (std::getline(ss, value, ',') && x < width) {
			int tileValue = std::stoi(value);
			tm.setMapValue(x, y, tileValue);
			x++;
		}
		y++;
	}
	tileFile.close();



	// Open the game stats file
	std::ifstream statsFile("game_stats.txt");
	if (!statsFile.is_open()) {
		std::cerr << "Error: Could not open game_stats.txt for reading!" << std::endl;
		return false;
	}

	// Read and restore game stats
	std::string lineKey;
	while (std::getline(statsFile, line)) {
		if (line.find("# Main Character") != std::string::npos) {
			// Read main character stats
			int x, y, speedX, speedY, lives, lastX, lastY;
			statsFile >> lineKey >> x >> y;
			statsFile >> lineKey >> speedX >> speedY;
			statsFile >> lineKey >> lives;
			statsFile >> lineKey >> lastX >> lastY;
			// Dynamic allocation due to number of objects
			this->mainChar = new MainCharacter(engine);
			mainChar->setPosition(x - mainChar->getDrawWidth()/2, y - mainChar->getDrawHeight()/2);
			mainChar->setSpeed(speedX, speedY);
			mainChar->setLives(lives);
			mainChar->setLastTiles(lastX, lastY);
			mainChar->setPaused(false);
			engine->appendObjectToArray(mainChar);
		}
		else if (line.find("# Enemy") != std::string::npos) {
			// Read enemy stats
			int x, y, speedX, speedY, lastX, lastY, bikeVal;
			statsFile >> lineKey >> x >> y;
			statsFile >> lineKey >> speedX >> speedY;
			statsFile >> lineKey >> lastX >> lastY;
			statsFile >> lineKey >> bikeVal;

			Psyeb10Enemy* enemy = new Psyeb10Enemy(engine, x, y, bikeVal);
			// Have to reverse engineer this so they are drawn in the right place
			enemy->setPosition(x - enemy->getDrawWidth() / 2, y - enemy->getDrawHeight() / 2);
			enemy->setSpeed(speedX, speedY);
			enemy->setLastTiles(lastX, lastY);
			enemyVec.push_back(enemy);
			engine->appendObjectToArray(enemy);
			enemy->setPaused(false);
			currentEnemies++;
		}
		else if (line.find("# Game State") != std::string::npos) {
			// Read game state stats
			int score, level;
			statsFile >> lineKey >> score;
			statsFile >> lineKey >> level;
			gameScore = score;
			currentLevel = level;
		}
	}
	statsFile.close();

	
	// Redraw the tiles to the background
	
	drawBackground();
	tm.setTopLeftPositionOnScreen(350, 100);
	tm.drawAllTiles(engine, engine->getBackgroundSurface());
	this->isDisplayed = true;

	std::cout << "Game loaded successfully!" << std::endl;
	
	return true;
}



void gameState::saveGame()
{
	

	std::ofstream outFile("tile_data.csv");
	if (!outFile.is_open()) {
		std::cerr << "Error: Could not open file for writing!" << std::endl;
		return;
	}

	const int width = tm.getMapWidth();
	const int height = tm.getMapHeight();

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			outFile << tm.getMapValue(x, y);
			if (x < width - 1) {
				outFile << ",";
			}
		}
		outFile << "\n";
	}
	outFile.close();
	std::cout << "Game saved to tile_data.csv!" << std::endl;

	// Save game stats to game_stats.txt
	std::ofstream statsFile("game_stats.txt");
	if (!statsFile.is_open()) {
		std::cerr << "Error: Could not open game_stats.txt for writing!" << std::endl;
		return;
	}

	

	// Save main character stats
	if (mainChar) {
		statsFile << "# Main Character\n";
		statsFile << "Position: " << mainChar->getXCentre() << " " << mainChar->getYCentre() << "\n";
		statsFile << "Speed: " << mainChar->getSpeedX() << " " << mainChar->getSpeedY() << "\n";
		statsFile << "Lives: " << mainChar->getLives() << "\n";
		// Can't have spaces due to how the file is read
		statsFile << "LastTiles: " << mainChar->getLastTileX() << " " << mainChar->getLastTileY() << "\n";
	}

	// Save all enemy stats
	for (auto& enemy : enemyVec) {
		statsFile << "# Enemy\n";
		statsFile << "Position: " << enemy->getXCentre() << " " << enemy->getYCentre() << "\n";
		statsFile << "Speed: " << enemy->getSpeedX() << " " << enemy->getSpeedY() << "\n";
		// Can't have spaces due to how the file is read
		statsFile << "LastTiles: " << enemy->getLastTileX() << " " << enemy->getLastTileY() << "\n";
		statsFile << "bikeVal: " << enemy->getBikeValue() << "\n";
	}


	// Save game state stats
	statsFile << "# Game State\n";
	statsFile << "Score: " << this->getGameScore() << "\n";
	statsFile << "Level: " << currentLevel << "\n";

	statsFile.close();
	std::cout << "Game stats saved to game_stats.txt!" << std::endl;
}

//Allows certian tiles to be removed from the tile manager
void gameState::cleanTileManager(int code) {
	for (int i = 0; i < tm.getMapWidth(); i++)
		for (int j = 0; j < tm.getMapHeight(); j++)
			// If the tile is the one we want to remove, set it to 0
			if (tm.getMapValue(i, j) == code)
				tm.setMapValue(i, j, 0);
}



bool gameState::checkPixelPerfectCollision(SimpleImage& bike1, int x1, int y1, int r1,
	SimpleImage& bike2, int x2, int y2, int r2) {
	// Get dimensions of the images
	int w1 = bike1.getWidth();
	int h1 = bike1.getHeight();
	int w2 = bike2.getWidth();
	int h2 = bike2.getHeight();

	// Calculate the intersection rectangle
	int xStart = std::max(x1, x2);
	int yStart = std::max(y1, y2);
	int xEnd = std::min(x1 + w1, x2 + w2);
	int yEnd = std::min(y1 + h1, y2 + h2);

	// Loop through the intersection region
	for (int y = yStart; y < yEnd; ++y) {
		for (int x = xStart; x < xEnd; ++x) {
			// Get pixel positions relative to each bike
			int bike1PixelX = x - x1;
			int bike1PixelY = y - y1;
			int bike2PixelX = x - x2;
			int bike2PixelY = y - y2;

			applyRotation(bike1PixelX, bike1PixelY, w1, h1, r1);
			applyRotation(bike2PixelX, bike2PixelY, w2, h2, r2);


			// Check if both pixels are opaque
			if (bike1.getPixelColour(bike1PixelX, bike1PixelY) != 0x00000000 && // Not transparent
				bike2.getPixelColour(bike2PixelX, bike2PixelY) != 0x00000000) { // Not transparent
				return true; // Collision detected
			}
		}
	}

	return false; // No collision
}


bool gameState::checkBikeCollision(SimpleImage& bike1, int x1, int y1, int r1,
	SimpleImage& bike2, int x2, int y2, int r2) {
	//std::cout << "Checking collision between bikes" << std::endl;
	// Bounding box dimensions
	int w1 = bike1.getWidth();
	int h1 = bike1.getHeight();
	int w2 = bike2.getWidth();
	int h2 = bike2.getHeight();

	// Step 1: Bounding box collision check
	if (!CollisionDetection::checkRectangles(x1, x1 + w1,y1, y1 + h1, x2, x2 + w2,y2, y2 + h2)) {
		return false; // No collision
	}
	// Step 2: Pixel-perfect collision check
	return checkPixelPerfectCollision(bike1, x1, y1,r1, bike2, x2, y2, r2);
}


void gameState::applyRotation(int& x, int& y, int width, int height, int rotation) {
	int tempX = x, tempY = y;
	switch (rotation) {
	case 1: // right
		x = height - 1 - tempY;
		y = tempX;
		break;
	case 2: // down
		x = width - 1 - tempX;
		y = height - 1 - tempY;
		break;
	case 3: // left
		x = tempY;
		y = width - 1 - tempX;
		break;
	default: // No rotation
		break;
	}
}
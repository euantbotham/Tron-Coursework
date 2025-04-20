#include "gameState.h"
#include "MainCharacter.h"
#include <sstream>
#include <string>
#include <fstream>
#include "Psyeb10Enemy.h"
#include "pauseState.h"


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
	this->enemy = new Psyeb10Enemy(engine);
	engine->appendObjectToArray(enemy);
	engine->setAllObjectsVisible(true);
}

void gameState::reset()
{
	engine->lockBackgroundForDrawing();
	enter();
	engine->unlockBackgroundForDrawing();
	engine->redrawDisplay();

	//Reset bikes to start positions
	DisplayableObject* bike1 = engine->getDisplayableObject(0);
	bike1->setPosition(650, 400);

	DisplayableObject* bike2 = engine->getDisplayableObject(1);
	bike2->setPosition(500, 150);
	//TODO add death reset for AI
}

void gameState::mouseDown(int iButton, int iX, int iY) {
	if (iButton == SDL_BUTTON_RIGHT)
	{
		engine->pause();
		this->mainChar->setPaused(true);
		this->enemy->setPaused(true);
		this->isDisplayed = false;
		engine->setState(new pauseState(engine), true , true);
	
	}
}

void gameState::mainLoopPreUpdate() {
	int time = engine->getModifiedTime() / 1000;
	if (time > lastTick)
	{
		lastTick = time;
		gameScore += 10;
	}
}


void gameState::reEntry()
{
	drawBackground();
	engine->createObjectArray(1);
	engine->storeObjectInArray(0, mainChar);
	engine->appendObjectToArray(enemy);
	mainChar->setPaused(false);
	enemy->setPaused(false);
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
	SimpleImage image = ImageManager::loadImage("MainCharacter.png", true);
	image.renderImageWithMask(engine->getBackgroundSurface(), 0, 0, 575, 20, image.getWidth(), image.getHeight());
}


gameState::~gameState()
{
	// delete mainchar and enemy if not currently helf in displayable object container
	if (!(this->isDisplayed)) {
		delete mainChar;
		delete enemy;
	}
}

MainCharacter* gameState::getmainChar()const {
	return mainChar;
}

Psyeb10Enemy* gameState::getEnemy() const{
	return enemy;
}

int gameState::getGameScore()const {
	return gameScore;
}


bool gameState::loadGame() {
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
			int x, y, speedX, speedY, lives;
			statsFile >> lineKey >> x >> y;
			statsFile >> lineKey >> speedX >> speedY;
			statsFile >> lineKey >> lives;

			mainChar->setPosition(x, y);
			mainChar->setSpeed(speedX, speedY);
			mainChar->setLives(lives);
		}
		else if (line.find("# Enemy") != std::string::npos) {
			// Read enemy stats
			int x, y, speedX, speedY;
			statsFile >> lineKey >> x >> y;
			statsFile >> lineKey >> speedX >> speedY;

			enemy->setPosition(x, y);
			enemy->setSpeed(speedX, speedY);
		}
		else if (line.find("# Game State") != std::string::npos) {
			// Read game state stats
			int score;
			statsFile >> lineKey >> score;
			gameScore = score;
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
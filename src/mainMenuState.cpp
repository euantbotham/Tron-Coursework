#include "mainMenuState.h"
#include "gameState.h"
#include "gameSetupState.h"

void mainMenuState::enter()
{
    // Fill the background with black
    engine->fillBackground(0x000000);

    // Define the grid dimensions
    const int gridStartX = 0;
    const int gridStartY = 0;
    const int gridEndX = 1300;
    const int gridEndY = 800;
    const int cellSize = 50; // Size of each grid cell

    // Draw the blue background
    engine->drawBackgroundRectangle(gridStartX, gridStartY, gridEndX, gridEndY, 0x000822);

    // Draw vertical grid lines
    for (int x = gridStartX; x <= gridEndX; x += cellSize) {
        engine->drawBackgroundThickLine(x, gridStartY, x, gridEndY, 0x000000, 2);
    }

    // Draw horizontal grid lines
    for (int y = gridStartY; y <= gridEndY; y += cellSize) {
        engine->drawBackgroundThickLine(gridStartX, y, gridEndX, y, 0x000000, 2);
    }

    loadAndPlayMusic("TronMusic.wav");
}

void mainMenuState::mainLoopPreUpdate()
{
        screenOffsetX = (screenOffsetX + 1) % engine->getWindowWidth();
        engine->redrawDisplay();
}



void mainMenuState::foreGroundStrings()
{
    // Load the image
    SimpleImage image = ImageManager::loadImage("Tron.png", true);

    // Get the screen dimensions
    int screenWidth = engine->getWindowWidth();
    int screenHeight = engine->getWindowHeight();

    // Get the image dimensions
    int imageWidth = image.getWidth();
    int imageHeight = image.getHeight();

    // Calculate the top-left position to center the image
    int xPosition = (screenWidth / 2) - (imageWidth / 2);
    int yPosition = (screenHeight / 2) - (imageHeight / 2);

    // Render the image at the calculated position
    image.renderImageWithMask(engine->getForegroundSurface(), 0, 0, xPosition, yPosition, imageWidth, imageHeight);

	engine->drawForegroundString(xPosition + imageWidth/3, yPosition + imageHeight + 50, "Press any key to start", 0x57E2F9, 0);
}

void mainMenuState::initObjects()
{
}

void mainMenuState::reset()
{
}

void mainMenuState::mouseDown(int iButton, int iX, int iY)
{
}
void mainMenuState::reEntry()
{
}


void mainMenuState::copyAllBackgroundBuffer()
{
	engine->getForegroundSurface()->copyRectangleFrom(engine->getBackgroundSurface(), 0, 0, engine->getWindowWidth(), engine->getWindowHeight(), screenOffsetX, 0);
	engine->getForegroundSurface()->copyRectangleFrom(engine->getBackgroundSurface(), engine->getWindowWidth() -screenOffsetX, 0, engine->getWindowWidth(), engine->getWindowHeight(),screenOffsetX - engine->getWindowWidth(), 0);
}


//Set the state to the game state when a key is pressed
void mainMenuState::keyPressed(int iKeyCode)
{
	engine->setState(new gameSetupState(engine), false, true);
}


void mainMenuState::loadAndPlayMusic(const char* filename)
{
    // Initialize audio subsystem if not already done
    if (SDL_GetCurrentAudioDriver() == NULL) {
        if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
            std::cerr << "SDL audio initialization failed: " << SDL_GetError() << std::endl;
            audioLoaded = false;
            return;
        }
    }

    // Load the WAV file
    if (SDL_LoadWAV(filename, &wavSpec, &wavBuffer, &wavLength) == NULL) {
        std::cerr << "Failed to load WAV file: " << SDL_GetError() << std::endl;
        audioLoaded = false;
        return;
    }

    // Open audio device
    audioDevice = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    if (audioDevice == 0) {
        std::cerr << "Failed to open audio device: " << SDL_GetError() << std::endl;
        SDL_FreeWAV(wavBuffer);
        audioLoaded = false;
        return;
    }

    // Play the audio
    if (SDL_QueueAudio(audioDevice, wavBuffer, wavLength) < 0) {
        std::cerr << "Failed to queue audio: " << SDL_GetError() << std::endl;
        SDL_CloseAudioDevice(audioDevice);
        SDL_FreeWAV(wavBuffer);
        audioLoaded = false;
        return;
    }

    // Start playing
    SDL_PauseAudioDevice(audioDevice, 0);
    audioLoaded = true;
}


void mainMenuState::cleanupAudio()
{
    if (audioLoaded) {
        SDL_PauseAudioDevice(audioDevice, 1);
        SDL_CloseAudioDevice(audioDevice);
        SDL_FreeWAV(wavBuffer);
        audioLoaded = false;
    }
}

mainMenuState::~mainMenuState()
{
    cleanupAudio();
}
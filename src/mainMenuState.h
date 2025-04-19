#pragma once
#include "header.h"
#include "Psyeb10States.h"
#include <SDL_audio.h>
class mainMenuState : public Psyeb10States
{
public:
	mainMenuState(Psyeb10Engine* engineParam)  {
		this->engine = engineParam;
		screenOffsetX = 0;

		// Initialize audio members
		wavBuffer = nullptr;
		wavLength = 0;
		audioDevice = 0;  // 0 is an invalid device ID in SDL
		audioLoaded = false;

		wavSpec = {};
	}
	~mainMenuState();
	void enter() override;
	void foreGroundStrings() override;
	void initObjects() override;
	void reset() override;
	void mouseDown(int iButton, int iX, int iY) override;
	void mainLoopPreUpdate() override;
	void reEntry() override;
	void copyAllBackgroundBuffer() override;
	void keyPressed(int iKeyCode) override;
	void loadAndPlayMusic(const char* filename);
	void cleanupAudio();

private:
	int screenOffsetX;
	SDL_AudioSpec wavSpec;
	Uint32 wavLength;
	Uint8* wavBuffer;
	SDL_AudioDeviceID audioDevice;
	bool audioLoaded;
};


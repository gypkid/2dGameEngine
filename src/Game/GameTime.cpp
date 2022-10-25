#include "GameTime.h"
#include <iostream>
#include <SDL2/SDL.h>

GameTime::GameTime() {}

GameTime::~GameTime() {}

float GameTime::DeltaTime(){
	int timeToWait = MILISECS_PER_FRAME - (SDL_GetTicks64() - milisecsPreviousFrame);
	if (timeToWait > 0 && timeToWait <= MILISECS_PER_FRAME) {
		SDL_Delay(timeToWait);
	}
	delta = (SDL_GetTicks64() - milisecsPreviousFrame) / 1000.0f;
	milisecsPreviousFrame = SDL_GetTicks64();
	return delta;
} 

#include "SDL.h"
#include "Timer.h"

Timer::Timer() {
	prevTicks = 0;
	currTicks = 0;
}

Timer::~Timer() {}

void Timer::UpdateFrameTicks() {
	prevTicks = currTicks;
	currTicks = SDL_GetTicks();
}

void Timer::Start() {
	prevTicks = SDL_GetTicks();
	currTicks = SDL_GetTicks();
}

float Timer::GetDeltaTime() const {
	return (float(currTicks - prevTicks));
}

unsigned int Timer::GetSleepTime(const unsigned int fps) const {
	unsigned int milliSecsPerFrame = 1000 / fps;
	if (milliSecsPerFrame == 0) {
		return 0;
	}

	unsigned int sleepTime = milliSecsPerFrame - (SDL_GetTicks() - currTicks);
	if (sleepTime > milliSecsPerFrame) {
		return milliSecsPerFrame;
	}

	return sleepTime;
}

#ifndef TIME_H
#define TIME_H

const int FPS = 500;
const int MILISECS_PER_FRAME = 1000 / FPS;

class GameTime {

private:
	int milisecsPreviousFrame;
	float delta;
public:
	GameTime();
	~GameTime();
	float DeltaTime();
};

#endif
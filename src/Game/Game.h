#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "../ECS/ECS.h"

class Game {
private:
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;

  std::unique_ptr<Registry> registry;

public:
	int windowWitdh;
	int windowHeight;

	Game();
	~Game();
	void Initialize();
	void Run();
	void Setup();
	void ProcessInput();
	void Update(float delta);
	void Render();
	void Destroy();
};

#endif

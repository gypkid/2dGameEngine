#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "../ECS/ECS.h"
#include "../AssetLib/AssetLib.h"
#include "../EventBus/EventBus.h"

class Game {
private:
	bool isRunning;
	bool isDebug;
	SDL_Window* window;
	SDL_Renderer* renderer;

  	std::unique_ptr<Registry> registry;
	std::unique_ptr<AssetLib> assetLib;
	std::unique_ptr<EventBus> eventBus;
public:
	int windowWitdh;
	int windowHeight;

	Game();
	~Game();
	void Initialize();
	void Run();
	void LoadLevel(int level);
  void Setup();
	void ProcessInput();
	void Update(float delta);
	void Render();
	void Destroy();
};

#endif

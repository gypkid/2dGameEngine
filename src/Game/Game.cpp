#include "Game.h"
#include "GameTime.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <memory>
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Systems/MovementSystem.h"

glm::vec2 playerPosition;
glm::vec2 velocity;
GameTime gameTime;

Game::Game() {
	isRunning = false;
  registry = std::make_unique<Registry>();
	Logger::Log("Game is Running");
}

Game::~Game() {
	Logger::Log("Game destructed");
}

void Game::Setup() {

  registry->AddSystem<MovementSystem>();

  Entity tank = registry->CreateEntity();

  tank.AddComponent<TransformComponent>(glm::vec2(10.0, 20.0), glm::vec2(1.0, 1.0), 0.0f );
  tank.AddComponent<RigidBodyComponent>(glm::vec2(20.0));

}

void Game::Initialize() {

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		Logger::Err("Error initializing SDL.");
		return;
	}

	/* Fake Fullscreen
	* 
	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	windowWitdh = displayMode.w;
	windowHeight = displayMode.h;
	*/

	windowWitdh = 800;
	windowHeight = 600;

	window = SDL_CreateWindow(
		NULL, 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWitdh,
		windowWitdh,
		SDL_WINDOW_BORDERLESS
		);

	if (!window) {
		Logger::Err("Error creating SDL window.");
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		Logger::Err("Error creating SDL renderer.");
		return;
	}

	/* Turn the Fullscreen Mode On.
	*
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	*/

	isRunning = true;
}

void Game::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						isRunning = false;
						break;
					default:
						break;
				}
				break;
		}
	}
}

void Game::Update(float delta) {
	registry->GetSystem<MovementSystem>().Update(delta);
  registry->Update();
}

void Game::Render() {
	
	SDL_SetRenderDrawColor(renderer, 64, 64, 64, 0);
	SDL_RenderClear(renderer);

	SDL_RenderPresent(renderer);
}

void Game::Run() {
	Setup();
	while (isRunning) {
		ProcessInput();
		Update(gameTime.DeltaTime());
		Render();
	}
}

void Game::Destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

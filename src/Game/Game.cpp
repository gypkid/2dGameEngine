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
#include "../Components/SpriteComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../AssetLib/AssetLib.h"
#include <fstream>
#include <sstream>

GameTime gameTime;

Game::Game() {
	isRunning = false;
  	registry = std::make_unique<Registry>();
	assetLib = std::make_unique<AssetLib>();
	Logger::Log("Game is Running");
}

Game::~Game() {
	Logger::Log("Game destructed");
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

void Game::LoadLevel(int level){
  registry->AddSystem<MovementSystem>();
  registry->AddSystem<RenderSystem>();

  assetLib->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
  assetLib->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
  assetLib->AddTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");
  
  int tileSize = 32;
  double tileScale = 2.0;
  int mapNumCols = 25;
  int mapNumRows = 20;



	std::ifstream tilemap("./assets/tilemaps/jungle.map");

	if(tilemap.is_open()) {
		Logger::Log("Loading Tilemap...");

		int columns = 10;           
		int tilesize = 32;          
		double tileScale = 1.0;     

		int x = 0;
		int y = 0;

		std::string line;

		while(std::getline(tilemap, line)) {
			std::stringstream lineStream(line);
			std::string cell;

			while(std::getline(lineStream, cell, ',')) {
				int cellIndex = std::stoi(cell);

				Entity mapCell = registry->CreateEntity();
				mapCell.AddComponent<TransformComponent>(glm::vec2(x, y), glm::vec2(tileScale, tileScale), 0.0);

				int srcRectX = (cellIndex%columns)*tilesize;
				int srcRectY = (cellIndex/columns)*tilesize;
			
				mapCell.AddComponent<SpriteComponent>("tilemap-image", tilesize, tilesize, 0,  srcRectX, srcRectY);
				x += tilesize * tileScale;
			}
				
			x = 0;
			y += tilesize * tileScale;
		}
			
		tilemap.close();
	} else {
			Logger::Err("Error opening tilemap file");
	}

  Entity tank = registry->CreateEntity();
  tank.AddComponent<TransformComponent>(glm::vec2(10.0, 20.0), glm::vec2(2.0, 2.0), 0.0f );
  tank.AddComponent<RigidBodyComponent>(glm::vec2(20.0));
  tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 1);
  
  Entity truck= registry->CreateEntity();
  truck.AddComponent<TransformComponent>(glm::vec2(200.0, 250.0), glm::vec2(2.0, 2.0), 0.0f );
  truck.AddComponent<RigidBodyComponent>(glm::vec2(40.0, -20.0));
  truck.AddComponent<SpriteComponent>("truck-image", 32, 32, 1);
}

void Game::Setup() {
  LoadLevel(0);
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
  
  registry->GetSystem<RenderSystem>().Update(renderer, assetLib);
  
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

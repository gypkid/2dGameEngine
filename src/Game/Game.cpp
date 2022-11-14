#include "Game.h"
#include "GameTime.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <memory>
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/RenderColliderSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyboardControlSystem.h"
#include "../AssetLib/AssetLib.h"
#include "../EventBus/EventBus.h"

Game::Game() {
	isRunning = false;
  	isDebug = false;
	registry = std::make_unique<Registry>();
	assetLib = std::make_unique<AssetLib>();
	eventBus = std::make_unique<EventBus>();
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
  registry->AddSystem<AnimationSystem>();
  registry->AddSystem<CollisionSystem>();
  registry->AddSystem<RenderColliderSystem>();
  registry->AddSystem<DamageSystem>();	
  registry->AddSystem<KeyboardControlSystem>();

  assetLib->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
  assetLib->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
  assetLib->AddTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");
  assetLib->AddTexture(renderer, "chopper-image", "./assets/images/chopper-spritesheet.png");

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
	
  Entity chopper = registry->CreateEntity();
  chopper.AddComponent<TransformComponent>(glm::vec2(100.0, 100.0), glm::vec2(1.0, 1.0), 0.0f );
  chopper.AddComponent<RigidBodyComponent>(glm::vec2(0.0));
  chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 1);
  chopper.AddComponent<AnimationComponent>(2, 10);
  chopper.AddComponent<KeyboardControlledComponent>(
	glm::vec2(0, -40),
	glm::vec2(40, 0),
	glm::vec2(-40, 0),
	glm::vec2(0, 40)
  );
  
  Entity tank = registry->CreateEntity();
  tank.AddComponent<TransformComponent>(glm::vec2(10.0, 20.0), glm::vec2(1.0, 1.0), 0.0f );
  tank.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 40.0));
  tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 1);
  tank.AddComponent<BoxColliderComponent>(32, 32);

  Entity truck= registry->CreateEntity();
  truck.AddComponent<TransformComponent>(glm::vec2(10.0, 250.0), glm::vec2(1.0, 1.0), 0.0f );
  truck.AddComponent<RigidBodyComponent>(glm::vec2(0.0, -40.0));
  truck.AddComponent<SpriteComponent>("truck-image", 32, 32, 1);
  truck.AddComponent<BoxColliderComponent>(32, 32);

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
					case SDLK_d:
						isDebug = !isDebug;
						break;
					default:
						eventBus->EmitEvent<KeyPressedEvent>(event.key.keysym.sym, event);
						break;
				}
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
					default:
						eventBus->EmitEvent<KeyPressedEvent>(event.key.keysym.sym, event);
						break;
				}
			break;
		}
	}
}

void Game::Update(float delta) {
	eventBus->Reset();
	
	registry->GetSystem<KeyboardControlSystem>().SubscribeToEvents(eventBus);
	registry->GetSystem<DamageSystem>().SubscribeToEvents(eventBus);

	registry->GetSystem<MovementSystem>().Update(delta);
	registry->GetSystem<AnimationSystem>().Update();
  	registry->GetSystem<CollisionSystem>().Update(eventBus);
	
	registry->Update();
}

void Game::Render() {
	
	SDL_SetRenderDrawColor(renderer, 64, 64, 64, 0);
	SDL_RenderClear(renderer);
  
  	registry->GetSystem<RenderSystem>().Update(renderer, assetLib);
	
	if(isDebug){
		registry->GetSystem<RenderColliderSystem>().Update(renderer);
	}
	
	SDL_RenderPresent(renderer);
}

void Game::Run() {
	Setup();
	GameTime gameTime;
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

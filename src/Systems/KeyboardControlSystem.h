#ifndef KEYBOARDCONTROLSYSTEM_H
#define KEYBOARDCONTROLSYSTEM_H

#include "../Game/Game.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/KeyboardControlledComponent.h"
#include <glm/glm.hpp>
#include <iostream>

class KeyboardControlSystem: public System {
    public:
        KeyboardControlSystem(){
            RequireComponent<KeyboardControlledComponent>();
            RequireComponent<SpriteComponent>();
            RequireComponent<RigidBodyComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
        }

        void OnKeyPressed(KeyPressedEvent& event) {
            for (auto entity: GetSystemEntities()){
                const auto keyboardcontrol = entity.GetComponent<KeyboardControlledComponent>();
                auto& sprite = entity.GetComponent<SpriteComponent>();
                auto& rigidbody = entity.GetComponent<RigidBodyComponent>();
                
                if(event.keyevent.type == SDL_KEYDOWN){
                    switch (event.symbol)
                    {
                        case SDLK_ESCAPE:
                            Game::isRunning = false;
						    //Game::Running(false);
						    break;
					    case SDLK_d:
                            Game::isDebug = !Game::isDebug;
						    //Game::Debug();
						    break;
                        case SDLK_UP:
                            rigidbody.velocity = keyboardcontrol.upVelocity;
                            sprite.srcRect.y = sprite.height * 0;
                            break;
                        case SDLK_LEFT:
                            rigidbody.velocity = keyboardcontrol.leftVelocity;
                            sprite.srcRect.y = sprite.height * 3;
                            break;
                        case SDLK_RIGHT:
                            rigidbody.velocity = keyboardcontrol.rightVelocity;
                            sprite.srcRect.y = sprite.height * 1;
                            break;
                        case SDLK_DOWN:
                            rigidbody.velocity = keyboardcontrol.downVelocity;
                            sprite.srcRect.y = sprite.height * 2;
                            break;
                    }
                }

                if(event.keyevent.type == SDL_KEYUP) {
                    switch (event.symbol)
                    {
                        case SDLK_UP:
                            rigidbody.velocity = glm::vec2(0, 0);
                            sprite.srcRect.y = sprite.height * 0;
                            break;
                        case SDLK_LEFT:
                            rigidbody.velocity = glm::vec2(0, 0);
                            sprite.srcRect.y = sprite.height * 3;
                            break;
                        case SDLK_RIGHT:
                            rigidbody.velocity = glm::vec2(0, 0);
                            sprite.srcRect.y = sprite.height * 1;
                            break;
                        case SDLK_DOWN:
                            rigidbody.velocity = glm::vec2(0, 0);
                            sprite.srcRect.y = sprite.height * 2;
                            break;
                    }
                }
            }            
        }

        void Update() {
            
        }
};

#endif
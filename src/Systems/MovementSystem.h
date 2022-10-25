#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

class MovementSystem : public System {
	public:
		MovementSystem() {
			RequireComponent<TransformComponent>();
      RequireComponent<RigidBodyComponent>();
      Logger::Log("MovementSystem initialized");
		}

		void Update(float delta) {
  	  for (auto entity : GetSystemEntities()) {
        auto& transform = entity.GetComponent<TransformComponent>();
        const auto rigidbody = entity.GetComponent<RigidBodyComponent>();
        
        Logger::Log("Entity in update loop");

        transform.position.x += rigidbody.velocity.x * delta;
        transform.position.y += rigidbody.velocity.y * delta;

        Logger::Log("Entity id: " + 
            std::to_string(entity.GetId()) + 
              " position is now (x:" + 
              std::to_string(transform.position.x) + 
              ", y:" + std:: to_string(transform.position.y) + 
              ")");
      }
		}
};

#endif

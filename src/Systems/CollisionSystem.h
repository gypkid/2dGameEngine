#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Logger/Logger.h"

class CollisionSystem: public System {

    public:
        CollisionSystem(){
            RequireComponent<BoxColliderComponent>();
            RequireComponent<TransformComponent>();
        }

        void Update(){
            auto entities = GetSystemEntities(); 

            for(auto i = entities.begin(); i != entities.end(); i++){
                Entity a = *i;
                auto aTransform = a.GetComponent<TransformComponent>();
                auto aCollider = a.GetComponent<BoxColliderComponent>();

                for(auto j = i; j != entities.end(); j++){
                    Entity b = *j;

                    if(a == b) { continue; }
                
                    auto bTransform = b.GetComponent<TransformComponent>();
                    auto bCollider = b.GetComponent<BoxColliderComponent>();

                    bool collisionHappend = CheckAABBCollision(
                        aTransform.position.x + aCollider.offset.x,
                        aTransform.position.y + aCollider.offset.y,
                        aCollider.width,
                        aCollider.height,
                        bTransform.position.x + bCollider.offset.x,
                        bTransform.position.y + bCollider.offset.y,
                        bCollider.width,
                        bCollider.height
                    );

                    if (collisionHappend){
                        Logger::Log("Entitie: " + std::to_string(a.GetId()) + " collied with entitie " + std::to_string(b.GetId()));
                    }
                }
            }     
        }

        bool CheckAABBCollision(double aY, double aX, double aW, double aH, double bY, double bX, double bW, double bH){
            return (
                aX < bX +bW &&
                aX + aW > bX &&
                aY < bY + bH &&
                aY + aH > bY
            );
        }
};

#endif
#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include <SDL2/SDL.h>
#include <algorithm>

class RenderSystem: public System {
  public:
    RenderSystem(){
      RequireComponent<TransformComponent>();
      RequireComponent<SpriteComponent>();
    }

    void Update(SDL_Renderer* renderer, std::unique_ptr<AssetLib>& assetLib){

      struct RandomableEntity
      {
        TransformComponent transformComponent;
        SpriteComponent spriteComponent;
      };

      std::vector<RandomableEntity> randomableEntities;
      for (auto entity: GetSystemEntities()){
        RandomableEntity randomableEntity;
        randomableEntity.spriteComponent = entity.GetComponent<SpriteComponent>();
        randomableEntity.transformComponent = entity.GetComponent<TransformComponent>();
        randomableEntities.emplace_back(randomableEntity);
      }

      std::sort(randomableEntities.begin(),randomableEntities.end(), [](const RandomableEntity& a, const RandomableEntity& b) {
        return a.spriteComponent.zIndex < b.spriteComponent.zIndex;
      });
      
      for(auto entity: randomableEntities) {
        const auto transform = entity.transformComponent;
        const auto sprite = entity.spriteComponent;

        SDL_Rect srcRect = sprite.srcRect;

        SDL_Rect dstRect =  {
          static_cast<int>(transform.position.x),
          static_cast<int>(transform.position.y),
          static_cast<int>(sprite.width * transform.scale.x),
          static_cast<int>(sprite.height * transform.scale.y)
        };

        SDL_RenderCopyEx(
            renderer,
            assetLib->GetTexture(sprite.assetId),
            &srcRect,
            &dstRect,
            transform.rotate,
            NULL,
            SDL_FLIP_NONE
        );

      }
    }
};

#endif

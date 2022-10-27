#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include <SDL2/SDL.h>

class RenderSystem: public System {
  public:
    RenderSystem(){
      RequireComponent<TransformComponent>();
      RequireComponent<SpriteComponent>();
    }

    void Update(SDL_Renderer* renderer, std::unique_ptr<AssetLib>& assetLib){
      for(auto entity: GetSystemEntities()) {
        const auto transform = entity.GetComponent<TransformComponent>();
        const auto sprite = entity.GetComponent<SpriteComponent>();

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

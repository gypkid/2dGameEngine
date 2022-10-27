#ifndef ASSETLIB_H
#define ASSETLIB_H

#include <map>
#include <string>
#include <SDL2/SDL.h>

class AssetLib {
  private:
    std::map<std::string, SDL_Texture*> textures;
  public:
    AssetLib();
    ~AssetLib();

    void ClearAssets();
    void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
    SDL_Texture* GetTexture(const std::string& assetId);
};

#endif

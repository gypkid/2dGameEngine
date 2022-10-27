#include "./AssetLib.h"
#include "../Logger/Logger.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

AssetLib::AssetLib() {
    Logger::Log("AssetLib constructor called.");
}

AssetLib::~AssetLib() {
    ClearAssets();
    Logger::Log("AssetLib destructor called.");
}

void AssetLib::ClearAssets(){
    for (auto texture: textures) {
        SDL_DestroyTexture(texture.second);
    }
    textures.clear();
}

void AssetLib::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath){
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    textures.emplace(assetId, texture);
}

SDL_Texture* AssetLib::GetTexture(const std::string& assetId) {
    return textures[assetId];
}
#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

struct SpriteComponent {
  std::string assetId;
  int width;
  int height;

  SpriteComponent(std::string assetId = "", int width = 32, int height = 32){
    this->assetId = assetId;
    this->width = width;
    this->height = height;
  }
};

#endif

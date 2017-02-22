#ifndef TEXTURES_H
#define TEXTURES_H

#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "typedefs.h"

enum texture_filtering 
{
  TEXTURE_MAG_NEAREST = 0,
  TEXTURE_MIN_NEAREST = 1,
  TEXTURE_NEAREST_MIPMAP_NEAREST = 2,
  TEXTURE_LINEAR_MIPMAP_NEAREST = 3,
  TEXTURE_NEAREST_MIPMA_LINEAR = 4,
  TEXTURE_LINEAR_MAPMAP_LINEAR = 5,
};

// Load and generate a texture
uint8 *
LoadTexture(const char *TexturePath)
{
  int32 Width, Height, NumBits;
  uint8 *Image = stbi_load(TexturePath, &Width, &Height, &NumBits, 0);
  
  if (Image == 0)
  {
    fprintf(stderr, "Error in loading Texture: %s", Image);
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Image);
  glGenerateMipmap(GL_TEXTURE_2D);

  return(Image);
}

// Creating the texture, binding and freeing it 
uint32
CreateTexture(const char *TexturePath)
{
  uint32 Texture;
  glGenTextures(1, &Texture);
  glBindTexture(GL_TEXTURE_2D, Texture);
  uint8 *Image = LoadTexture(TexturePath);
  stbi_image_free(Image);
  glBindTexture(GL_TEXTURE_2D, 0);

  return(Texture);
}

uint8 *
LoadImage(const char *ImagePath)
{
  int32 Width, Height, NumBits;
  uint8 *Image = stbi_load(ImagePath, &Width, &Height, &NumBits, 0);

  if (Image == 0)
  {
    fprintf(stderr, "Error in loading image: %s", Image);
  }

  return(Image);
}


#endif

#ifndef TEXTURES_H
#define TEXTURES_H

#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "typedefs.h"

enum texture_filtering 
{
  TEXTURE_NEAREST_MIPMAP_NEAREST = 0,
  TEXTURE_LINEAR_MIPMAP_NEAREST = 1,
  TEXTURE_NEAREST_MIPMAP_LINEAR = 2,
  TEXTURE_LINEAR_MIPMAP_LINEAR = 3,
  TEXTURE_FILTERING_NONE = 4,
};

// Load an image
u8 *
LoadImage(const char *ImagePath)
{
  i32 Width, Height, NumBits;
  u8 *Image = stbi_load(ImagePath, &Width, &Height, &NumBits, 0);

  if (Image == 0)
  {
    fprintf(stderr, "Error in loading image: %s\n", Image);
    return((u8*)1);
  }

  return(Image);
}

// Load and generate a texture
u8 *
LoadImageTexture(const char *ImagePath)
{
  i32 Width, Height, NumBits;
  u8 *Image = stbi_load(ImagePath, &Width, &Height, &NumBits, 0);
  
  if (Image == 0)
  {
    fprintf(stderr, "Error in loading Texture: %s", Image);
    return((u8*)1);
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Image);
  glGenerateMipmap(GL_TEXTURE_2D);

  return(Image);
}

// Gets the loaded texture, binds it and frees the image
u32
LoadTexture(const char *TexturePath, enum texture_filtering Filter)
{
  u32 Texture;
  glGenTextures(1, &Texture);
  glBindTexture(GL_TEXTURE_2D, Texture);
  switch (Filter)
  {
    case TEXTURE_LINEAR_MIPMAP_LINEAR:
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;

    case TEXTURE_NEAREST_MIPMAP_LINEAR:
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;

    case TEXTURE_LINEAR_MIPMAP_NEAREST:
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;

    case TEXTURE_NEAREST_MIPMAP_NEAREST:
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;

    default:
      break;
  }

  u8 *Image = LoadImageTexture(TexturePath);
  stbi_image_free(Image);
  glBindTexture(GL_TEXTURE_2D, 0);
  
  return(Texture);
}

#endif

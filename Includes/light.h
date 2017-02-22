#ifndef LIGHT_H
#define LIGHT_H

#include "math_lib.h"

struct light
{
  struct vec3 Direction;
  struct vec3 Ambient;
  struct vec3 Diffuse;
  struct vec3 Specular;
};

#endif

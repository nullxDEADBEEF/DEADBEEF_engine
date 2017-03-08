#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>

#include "typedefs.h"
#include "camera.h"

SDL_Event Event;
bool Running = true;

f32 LastX = 0.0f;
f32 LastY = 0.0f;
f32 LastScrollY = 0.0f;
bool FirstMouse = true;

struct key_state
{
  bool Forward;
  bool Backward;
  bool Left;
  bool Right;
};

ProcessMouseMovement(struct camera *Camera, f32 XOffset, f32 YOffset, bool ConstrainPitch)
{
  ConstrainPitch = true;
  XOffset *= Camera->MouseSensitivity;
  YOffset *= Camera->MouseSensitivity;

  Camera->Yaw += XOffset;
  Camera->Pitch += YOffset;

  if (ConstrainPitch)
  {
    if (Camera->Pitch > 89.0f)
    {
      Camera->Pitch = 89.0f;
    }
    if (Camera->Pitch < -89.0f)
    {
      Camera->Pitch = -89.0f;
    }
  }

  UpdateCameraVectors(Camera);
}


void
ProcessKeyboard(struct camera *Camera, struct key_state *Ks,
  f32 DeltaTime)
{

  f32 Velocity = Camera->MovementSpeed * DeltaTime;
  if (Ks->Forward)
  {
    Camera->Position.X += Camera->Front.X * Velocity;
    Camera->Position.Y += Camera->Front.Y * Velocity;
    Camera->Position.Z += Camera->Front.Z * Velocity;
  }
  if (Ks->Backward)
  {
    Camera->Position.X -= Camera->Front.X * Velocity;
    Camera->Position.Y -= Camera->Front.Y * Velocity;
    Camera->Position.Z -= Camera->Front.Z * Velocity;
  }
  if (Ks->Left)
  {
    Camera->Position.X -= Camera->Right.X * Velocity;
    Camera->Position.Y -= Camera->Right.Y * Velocity;
    Camera->Position.Z -= Camera->Right.Z * Velocity;
  }
  if (Ks->Right)
  {
    Camera->Position.X += Camera->Right.X * Velocity;
    Camera->Position.Y += Camera->Right.Y * Velocity;
    Camera->Position.Z += Camera->Right.Z * Velocity;
  }
}

void
ProcessInput(struct camera *Camera, struct key_state *Ks, f32 DeltaTime)
{
  ProcessKeyboard(Camera, Ks, DeltaTime);
}

void
ProcessInputStates(struct camera *Camera, struct key_state *Ks)
{
  while (SDL_PollEvent(&Event))
  {
    switch (Event.type)
    {
    case SDL_QUIT:
      Running = false;
      break;

    case SDL_KEYDOWN:
      switch (Event.key.keysym.sym)
      {
        case SDLK_ESCAPE:
          printf("Exiting...\n");
          Running = false;
          break;

        case SDLK_w:
          Ks->Forward = true;
          break;

        case SDLK_s:
          Ks->Backward = true;
          break;

        case SDLK_a:
          Ks->Left = true;
          break;

        case SDLK_d:
          Ks->Right = true;
          break;

        case SDLK_1:
          // Wireframe mode
          glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
          printf("Wireframe mode\n");
          break;

        case SDLK_2:
          // Filled mode
          glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
          printf("Filled mode\n");
          break;
      }
      break;
 
    case SDL_KEYUP:
      switch (Event.key.keysym.sym)
      {
        case SDLK_w:
          Ks->Forward = false;
          break;

        case SDLK_s:
          Ks->Backward = false;
          break;

        case SDLK_a:
          Ks->Left = false;
          break;

        case SDLK_d:
          Ks->Right = false;
          break;
      }
      break;

    case SDL_MOUSEMOTION:
    {
      ProcessMouseMovement(Camera, (f32)Event.motion.xrel, (f32)-Event.motion.yrel, true);
    }
    break;

    case SDL_MOUSEBUTTONDOWN:
      switch (Event.button.button)
      {
        case SDL_BUTTON_LEFT:
          printf("Left button was clicked\n");
          break;

        case SDL_BUTTON_RIGHT:
          printf("Right button was clicked\n");
          break;
      }
      break;

    case SDL_MOUSEBUTTONUP:
      switch (Event.button.button)
      {
        case SDL_BUTTON_LEFT:
          printf("Left button was released\n");
          break;

        case SDL_BUTTON_RIGHT:
          printf("Right button was released\n");
          break;
      }
      break;

    case SDL_MOUSEWHEEL:
    {
      ProcessMouseScroll(Camera, (f32)Event.wheel.y);
    }
   }
  }
}

#endif
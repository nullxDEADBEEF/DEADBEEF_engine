#ifndef CAMERA_H
#define CAMERA_H

#include "math_lib.h"
#include "typedefs.h"

// Defines possible options for movement
enum camera_movement
{
  FORWARD  = 0,
  BACKWARD = 1,
  LEFT     = 2,
  RIGHT    = 3,
};

void
UpdateCameraVectors(struct camera *const Camera);

// Camera values
const f32         YAW = -90.0f;
const f32       PITCH = 0.0f;
const f32       SPEED = 3.0f;
const f32 SENSITIVITY = 0.1f;
const f32        ZOOM = 45.0f;

// Structure that processes input and calculates the corresponding euler angles
struct camera
{
  // Camera attributes
  struct vec3 Position;
  struct vec3 Front;
  struct vec3 Up;
  struct vec3 Right;
  struct vec3 WorldUp;

  // Euler angles
  f32 Yaw;
  f32 Pitch;

  // Camera options
  f32 MovementSpeed;
  f32 MouseSensitivity;
  f32 Zoom;
};

struct camera 
CreateCamera(const struct vec3 Position, const struct vec3 Front,  
             const struct vec3 Up)
{
  struct camera Camera;
  Camera.Position = Position;
  Camera.WorldUp = Up;
  Camera.Yaw = YAW;
  Camera.Pitch = PITCH;
  Camera.Front = Front;
  Camera.MovementSpeed = SPEED;
  Camera.MouseSensitivity = SENSITIVITY;
  Camera.Zoom = ZOOM;
  UpdateCameraVectors(&Camera);

  return(Camera);
}

struct mat4 
GetViewMatrix(struct camera *Camera)
{
  struct mat4 M = Mat4LookAt(Camera->Position, Vec3Add(Camera->Position, Camera->Front), Camera->Up);
  return(M);
}

void
ProcessMouseScroll(struct camera *const Camera, f32 YOffset)
{
  if (Camera->Zoom >= 1.0f && Camera->Zoom <= 45.0f)
  {
    Camera->Zoom -= YOffset;
  }
  if (Camera->Zoom <= 1.0f)
  {
    Camera->Zoom = 1.0f;
  }
  if (Camera->Zoom >= 45.0f)
  {
    Camera->Zoom = 45.0f;
  }
}

// Calculates front vector from camera's updated euler angles
void
UpdateCameraVectors(struct camera *const Camera)
{
  struct vec3 Front;
  Front.X = cosf(ToRadians(Camera->Yaw)) * cosf(ToRadians(Camera->Pitch));
  Front.Y = sinf(ToRadians(Camera->Pitch));
  Front.Z = sinf(ToRadians(Camera->Yaw)) * cosf(ToRadians(Camera->Pitch));
  Camera->Front = NormalizeVec3(Front);

  Camera->Right = NormalizeVec3(Vec3Cross(Camera->Front, Camera->WorldUp));
  Camera->Up = NormalizeVec3(Vec3Cross(Camera->Right, Camera->Front));
}

#endif

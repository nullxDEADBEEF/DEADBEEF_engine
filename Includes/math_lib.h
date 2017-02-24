#ifndef MATH_LIB_H
#define MATH_LIB_H

#ifdef __linux
#pragma GCC diagnostic ignored "-Wmissing-braces"
#endif

/*
      IMPORTANT THAT YOU DO #define DEADBEEF_MATH_IMPLEMENTATION before including the file, like this:
      #define DEADBEEF_MATH_IMPLEMENTATION
      #include "math_lib.h"

      In one C/C++ file that includes this header
*/

/*
math_lib.h - v0.2.0
*/

#include <math.h>

#include "typedefs.h"

/* ------------------------------------------------------
*
*                       UTILITY STUFF
*
*  -----------------------------------------------------*/

#define PI 3.14159265359f

inline f32
ToRadians(f32 Angle)
{
  return(f32) { Angle * (PI / 180.0f) };
}

inline f32
ToDegrees(f32 Angle)
{
  return(f32) { Angle * (180.0f / PI) };
}

/* ------------------------------------------------------
*
*                       STRUCTURES
*
*  -----------------------------------------------------*/

struct vec2
{
  f32 X;
  f32 Y;
};

struct vec3
{
  f32 X;
  f32 Y;
  f32 Z;
};

struct vec4
{
  f32 X;
  f32 Y;
  f32 Z;
  f32 W;
};

struct mat2
{
  f32 Element[2][2];
};

struct mat4
{
  f32 Element[4][4];
};

/* ------------------------------------------------------
*
*                       VECTOR MATH
*
*  -----------------------------------------------------*/

#ifdef DEADBEEF_MATH_IMPLEMENTATION

// VEC 2 functions
inline struct vec2 
Vec2Add(const struct vec2 V0, const struct vec2 V1)
{
  return (struct vec2) { V0.X + V1.X, V0.Y + V1.Y };
}

inline struct vec2 
Vec2Sub(const struct vec2 V0, const struct vec2 V1)
{
  return (struct vec2) { V0.X - V1.X, V0.Y - V1.Y };
}

inline struct vec2 
Vec2Div(const struct vec2 V0, const struct vec2 V1)
{
  return (struct vec2) { V0.X / V1.X, V0.Y / V1.Y };
}

inline f32 
Vec2Length(const struct vec2 V)
{
  return (f32) { (float)sqrt((V.X * V.X) + (V.Y * V.Y)) };
}

inline f32 
Vec2Dot(const struct vec2 V0, const struct vec2 V1)
{
  return (f32) { (V0.X * V1.X) + (V0.Y * V1.Y) };
}

inline struct vec2 
NormalizeVec2(const struct vec2 V)
{
  return (struct vec2) { (V.X / Vec2Length(V)),
                         (V.Y / Vec2Length(V)) };
}

// VEC3 functions
inline struct vec3 
Vec3Add(const struct vec3 V0, const struct vec3 V1)
{
  return (struct vec3) { V0.X + V1.X, V0.Y + V1.Y, V0.Z + V1.Z };
}

inline struct vec3 
Vec3Sub(const struct vec3 V0, const struct vec3 V1)
{
  return (struct vec3) { V0.X - V1.X, V0.Y - V1.Y, V0.Z - V1.Z };
}

inline struct vec3 
Vec3Div(const struct vec3 V0, const struct vec3 V1)
{
  return (struct vec3) { V0.X / V1.X, V0.Y / V1.Y, V0.Z / V1.Z };
}

inline f32 
Vec3Length(const struct vec3 V)
{
  return (f32) { (float)sqrt((V.X * V.X) + (V.Y * V.Y) + (V.Z * V.Z)) };
}

inline f32 
Vec3Dot(const struct vec3 V0, const struct vec3 V1)
{
  return (f32) { (V0.X * V1.X) + (V0.Y * V1.Y) + (V0.Z * V1.Z) };
}

inline struct vec3 
Vec3Cross(const struct vec3 V0, const struct vec3 V1)
{
  return (struct vec3) { (V0.Y * V1.Z) - (V0.Z * V1.Y),
                         (V0.Z * V1.X) - (V0.X * V1.Z),
                         (V0.X * V1.Y) - (V0.Y * V1.X) }; 
}

inline struct vec3 
NormalizeVec3(const struct vec3 V)
{
  return (struct vec3) { (V.X / Vec3Length(V)),
                         (V.Y / Vec3Length(V)),
                         (V.Z / Vec3Length(V)) };
}

// VEC4 functions
inline struct vec4 
Vec4Add(const struct vec4 V0, const struct vec4 V1)
{
  return (struct vec4) { V0.X + V1.X, V0.Y + V1.Y, V0.Z + V1.Z,
                         V0.W + V1.W };
}

inline struct vec4 
Vec4Sub(const struct vec4 V0, const struct vec4 V1)
{
  return (struct vec4) { V0.X - V1.X, V0.Y - V1.Y, V0.Z - V1.Z,
                         V0.W - V1.W };
}

inline struct vec4 
Vec4Div(const struct vec4 V0, const struct vec4 V1)
{
  return (struct vec4) { V0.X / V1.X, V0.Y / V1.Y, V0.Z / V1.Z,
                         V0.W / V1.W };
}

inline f32 
Vec4Length(const struct vec4 V)
{
  return (f32) { (float)sqrt((V.X * V.X) + (V.Y * V.Y) + (V.Z * V.Z) +
                               (V.W * V.W)) };
}

inline f32 
Vec4Dot(const struct vec4 V0, const struct vec4 V1)
{
  return (f32) { (V0.X * V1.X) + (V0.Y * V1.Y) + (V0.Z * V1.Z) +
                   (V0.W * V1.W) };
}

inline struct vec4 
NormalizeVec4(const struct vec4 V)
{
  return (struct vec4) { (V.X / Vec4Length(V)),
                         (V.Y / Vec4Length(V)),
                         (V.Z / Vec4Length(V)),
                         (V.W / Vec4Length(V)) };
}

/* ------------------------------------------------------
*
*                       MATRIX MATH
*
*  -----------------------------------------------------*/

// MATRIX 2
inline struct mat2 
Matrix2Add(const struct mat2 M0, const struct mat2 M1)
{
  return (struct mat2) { M0.Element[0][0] + M1.Element[0][0],
                         M0.Element[0][1] + M1.Element[0][1],
                         M0.Element[1][0] + M1.Element[1][0],
                         M0.Element[1][1] + M1.Element[1][1] };
}

inline struct mat2 
Matrix2ScalarAdd(const f32 Value, const struct mat2 M1)
{
  return (struct mat2) { Value + M1.Element[0][0],
                         Value + M1.Element[0][1],
                         Value + M1.Element[1][0],
                         Value + M1.Element[1][1] };
}

inline struct mat2 
Matrix2Sub(const struct mat2 M0, const struct mat2 M1)
{
  return (struct mat2) { M0.Element[0][0] - M1.Element[0][0],
                         M0.Element[0][1] - M1.Element[0][1],
                         M0.Element[1][0] - M1.Element[1][0],
                         M0.Element[1][1] - M1.Element[1][1] };
}

inline struct mat2 
Matrix2ScalarSub(const f32 Value, const struct mat2 M1)
{
  return (struct mat2) { M1.Element[0][0] - Value,
                         M1.Element[0][1] - Value,
                         M1.Element[1][0] - Value,
                         M1.Element[1][1] - Value };
}

inline struct mat2 
Matrix2Mul(const struct mat2 M0, const struct mat2 M1)
{
  return (struct mat2) { M0.Element[0][0] * M1.Element[0][0] +
                         M0.Element[0][1] * M1.Element[1][0] ,

                         M0.Element[0][0] * M1.Element[0][1] +
                         M0.Element[0][1] * M1.Element[1][1] ,

                         M0.Element[1][0] * M1.Element[0][0] +
                         M0.Element[1][1] * M1.Element[1][0] ,

                         M0.Element[1][0] * M1.Element[0][1] +
                         M0.Element[1][1] * M1.Element[1][1] };
}

inline struct mat2 
Matrix2ScalarProduct(const f32 Value, const struct mat2 M1)
{
  return (struct mat2) { Value * M1.Element[0][0],
                         Value * M1.Element[0][1],
                         Value * M1.Element[1][0],
                         Value * M1.Element[1][1] };
}

// MATRIX 4
inline struct mat4
InitializeMat4(const f32 Value)
{
  struct mat4 M;
  M.Element[0][0] = Value;
  M.Element[0][1] = 0.0f;
  M.Element[0][2] = 0.0f;
  M.Element[0][3] = 0.0f;
  M.Element[1][0] = 0.0f;
  M.Element[1][1] = Value;
  M.Element[1][2] = 0.0f;
  M.Element[1][3] = 0.0f;
  M.Element[2][0] = 0.0f;
  M.Element[2][1] = 0.0f;
  M.Element[2][2] = Value;
  M.Element[2][3] = 0.0f;
  M.Element[3][0] = 0.0f;
  M.Element[3][1] = 0.0f;
  M.Element[3][2] = 0.0f;
  M.Element[3][3] = Value;
  return(M);
}

inline struct mat4 
Mat4Translate(const struct vec3 *V)
{
  struct mat4 Result = InitializeMat4(1.0f);
  Result.Element[3][0] = V->X;
  Result.Element[3][1] = V->Y;
  Result.Element[3][2] = V->Z;
  return(Result);
}

inline struct mat4
Mat4Rotate(const f32 Angle, struct vec3 *V)
{
  struct mat4 M = InitializeMat4(1.0f);
  f32 Radian = ToRadians(Angle);
  f32 C = cosf(Radian);
  f32 S = sinf(Radian);
  f32 T = 1 - C;
  *V = NormalizeVec3(*V);
  M.Element[0][0] = (V->X * V->X * T) + C;
  M.Element[0][1] = (V->X * V->Y * T) - V->Z * S;
  M.Element[0][2] = (V->X * V->Z * T) + V->Y * S;

  M.Element[1][0] = (V->Y * V->X * T) + V->Z * S;
  M.Element[1][1] = (V->Y * V->Y * T) + C;
  M.Element[1][2] = (V->Y * V->Z * T) - V->X * S;

  M.Element[2][0] = (V->Z * V->X * T) - V->Y * S;
  M.Element[2][1] = (V->Z * V->Y * T) + V->X * S;
  M.Element[2][2] = (V->Z * V->Z * T) + C;
  return(M);
}

inline struct mat4 
Mat4RotX(const f32 Angle)
{
  struct mat4 M = InitializeMat4(1.0f);
  f32 Radian = ToRadians(Angle);
  M.Element[1][1] = cosf(Radian);
  M.Element[1][2] = -sinf(Radian);
  M.Element[2][1] = sinf(Radian);
  M.Element[2][2] = cosf(Radian);
  return(M);
}

inline struct mat4
Mat4RotY(const f32 Angle)
{
  struct mat4 M = InitializeMat4(1.0f);
  f32 Radian = ToRadians(Angle);
  M.Element[0][0] = cosf(Radian);
  M.Element[0][2] = sinf(Radian);
  M.Element[2][0] = -sinf(Radian);
  M.Element[2][2] = cosf(Radian);
  return(M);
}

inline struct mat4
Mat4RotZ(const f32 Angle)
{
  struct mat4 M = InitializeMat4(1.0f);
  f32 Radian = ToRadians(Angle);
  M.Element[0][0] = cosf(Radian);
  M.Element[0][1] = -sinf(Radian);
  M.Element[1][0] = sinf(Radian);
  M.Element[1][1] = cosf(Radian);
  return(M);
}

inline struct mat4 
Mat4Scale(const f32 X, const f32 Y, const f32 Z)
{
  struct mat4 M = InitializeMat4(1.0f);
  M.Element[0][0] = X;
  M.Element[1][1] = Y;
  M.Element[2][2] = Z;
  return(M);
}

inline struct mat4
Mat4Orthographic(const f32 Left, const f32 Right, 
                 const f32 Bottom, const f32 Top,
                 const f32 Far, const f32 Near)
{
  struct mat4 M = InitializeMat4(1.0f);
  M.Element[0][0] = 2 / Right - Left;
  M.Element[0][3] = -((Right + Left) / Right - Left);
  M.Element[1][1] = 2 / Top - Bottom;
  M.Element[1][3] = -((Top + Bottom) / Top - Bottom);
  M.Element[2][2] = -2 / (Far - Near);
  M.Element[2][3] = -((Far + Near) / (Far - Near));
  return(M);
}

inline struct mat4
Mat4Perspective(const f32 FOV , const f32 Aspect, 
                const f32 Near, const f32 Far)
{
  struct mat4 M = InitializeMat4(1.0f);
  f32 S = 1 / tanf((FOV / 2) * (PI / 180));
  M.Element[0][0] = Aspect * S;
  M.Element[1][1] = Aspect * S;
  M.Element[2][2] = -(Far / (Far - Near));
  M.Element[2][3] = -1.0f;
  M.Element[3][2] = -((Far * Near) / (Far - Near));
  return(M);
}

inline struct mat4
Mat4LookAt(const struct vec3 Eye, const struct vec3 Center, const struct vec3 Up)
{
  struct mat4 M = InitializeMat4(1.0f);
  // F = The direction you look at
  struct vec3 F = NormalizeVec3(Vec3Sub(Center, Eye));
  // U = Up
  struct vec3 U = NormalizeVec3(Up);
  // S = Right Vector
  struct vec3 S = NormalizeVec3(Vec3Cross(F, U));
  U = Vec3Cross(S, F);

  M.Element[0][0] = S.X;
  M.Element[0][1] = U.X;
  M.Element[0][2] = -F.X;

  M.Element[1][0] = S.Y;
  M.Element[1][1] = U.Y;
  M.Element[1][2] = -F.Y;
  
  M.Element[2][0] = S.Z;
  M.Element[2][1] = U.Z;
  M.Element[2][2] = -F.Z;

  M.Element[3][0] = -Vec3Dot(S, Eye);
  M.Element[3][1] = -Vec3Dot(U, Eye);
  M.Element[3][2] = Vec3Dot(F, Eye);
  return(M);
}

/* ------------------------------------------------------
*
*                       QUATERNION MATH
*
*  -----------------------------------------------------*/

#endif // DEADBEEF_MATH_IMPLEMENTATION

#endif // math_lib.h

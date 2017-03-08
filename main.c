#define GLEW_STATIC
#include <GL/glew.h>
#include <stdio.h>
#include <SDL.h>

#define DEADBEEF_MATH_IMPLEMENTATION
#include "includes/math_lib.h"
#include "includes/shader.h"
#include "includes/camera.h"
#include "includes/input.h"
#include "includes/textures.h"
#include "includes/light.h"
#include "includes/typedefs.h"

struct screen
{
  i16 W;
  i16 H;
};

void
Init();

#undef main

// Window resolution
struct screen WindowSize = { 1028, 768 };
#define TITLE "DEADBEEF Engine"

// Camera
struct camera Camera;

f64       DeltaTime = 0.0f;
f64       LastFrame = 0.0f;
const f32 FrameRate = 60.0f;

i32
main()
{
  Init();

  SDL_Window *Window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        WindowSize.W, WindowSize.H, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  if (Window == NULL)
  {
    fprintf(stderr, "Error in creating the window!\n %s\n", SDL_GetError());
    SDL_Quit();
    return(-1);
  }

  SDL_GLContext Context = SDL_GL_CreateContext(Window);
  SDL_GL_SetSwapInterval(1);

  SDL_SetRelativeMouseMode(true);

  glewExperimental = true;
  if (glewInit() != GLEW_OK)
  {
    fprintf(stderr, "Error in initializing GLEW!\n");
    return(-1);
  }

  glViewport(0, 0, WindowSize.W, WindowSize.H);
  glEnable(GL_DEPTH_TEST);

  struct vec3 CameraStartPosition = { 1.2f, 2.0f, 6.0f };
  struct vec3 CameraDirection = { 0.0f, 0.0f, -1.0f };
  struct vec3 CameraUp = { 0.0f, 1.0f, 0.0f };
  Camera = CreateCamera(CameraStartPosition, CameraDirection, CameraUp);

  struct shader InitShader = CreateShader("shaders/init_shader.vs", "shaders/init_shader.frag");

  f32 Vertices[] =
    {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

  // First, set the container VAO and VBO
  u32 VBO, ContainerVAO;
  glGenVertexArrays(1, &ContainerVAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

  glBindVertexArray(ContainerVAO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 5 * sizeof(f32), (GLvoid*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, 5 * sizeof(f32), 
                                                  (GLvoid*)(3 * sizeof(f32)));
  glEnableVertexAttribArray(2);
  
  glBindVertexArray(0);

  // Print version information to the command line
  SDL_version SDLVersion;
  SDL_GetVersion(&SDLVersion);
  printf("OpenGL %s\n", glGetString(GL_VERSION));
  printf("SDL %d.%d.%d\n\n", SDLVersion.major, SDLVersion.minor, SDLVersion.patch);

  u8 ExampleImage = LoadTexture("texture_and_images/youdontsay.jpg", TEXTURE_LINEAR_MIPMAP_LINEAR);

  const f32 ZNear = 0.1f;
  const f32 ZFar = 100.0f;

  struct vec3 ModelPos = { 1.2f, 1.0f, 4.0f };
  
  struct key_state Ks = { false, false, false, false };

  while (Running)
  {
    f64 CurrentFrame = SDL_GetTicks();
    DeltaTime = (CurrentFrame - LastFrame) / 1000.0f;
    LastFrame = CurrentFrame;

    ProcessInputStates(&Camera, &Ks);
    ProcessInput(&Camera, &Ks, (f32)DeltaTime);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    UseShader(InitShader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ExampleImage);

    struct mat4 ViewLookAt = GetViewMatrix(&Camera);
    struct mat4 Projection = Mat4Perspective(Camera.Zoom, (f32)WindowSize.W / (f32)WindowSize.H, ZNear, ZFar);

    i32 ModelLoc = glGetUniformLocation(InitShader.Program, "model");
    i32 ViewLoc = glGetUniformLocation(InitShader.Program, "view");
    i32 ProjLoc = glGetUniformLocation(InitShader.Program, "projection");

    glUniformMatrix4fv(ViewLoc, 1, GL_FALSE, (f32*)&ViewLookAt);
    glUniformMatrix4fv(ProjLoc, 1, GL_FALSE, (f32*)&Projection);
    
    glBindVertexArray(ContainerVAO);
    struct mat4 Model = Mat4Translate(&ModelPos);
    glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, (f32*)&Model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    SDL_GL_SwapWindow(Window);
  }
  glDeleteVertexArrays(1, &ContainerVAO);
  glDeleteBuffers(1, &VBO);
  SDL_GL_DeleteContext(Context);
  SDL_DestroyWindow(Window);
  SDL_Quit();
  
  return(0);
}

void
Init()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    printf("Error in initializing SDL...\n %s\n", SDL_GetError());
    SDL_Quit();
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}
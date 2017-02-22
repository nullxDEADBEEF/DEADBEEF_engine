#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "includes/shader.h"
#include "includes/math_lib.h"
#include "includes/camera.h"
#include "includes/textures.h"
#include "includes/light.h"
#include "includes/typedefs.h"

void
KeyCallback(GLFWwindow *Window, i32 Key, i32 ScanCode, i32 Action, i32 Mode);

void
CameraMovement();

void
MouseCallback(GLFWwindow *Window, f64 XPos, f64 YPos);

void
MouseButtonCallback(GLFWwindow *Window, i32 Button, i32 Action, i32 Mode);

void
ScrollCallback(GLFWwindow *Window, f64 XOffset, f64 YOffset);

// Window resolution
#define WIDTH 1028
#define HEIGHT 768
#define TITLE "DEADBEEF Engine"

// Camera
struct camera Camera;
f32 LastX = WIDTH / 2.0f;
f32 LastY = HEIGHT / 2.0f;
bool Keys[1024];

bool FirstMouse = true;

f64       DeltaTime = 0.0f;
f64       LastFrame = 0.0f;
const f32 FrameRate = 60.0f;

i32
main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow *Window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
  if (Window == NULL)
  {
    fprintf(stderr, "Error in creating the window!\n");
    glfwTerminate();
    return(-1);
  }
  glfwSetWindowPos(Window, 200, 100);

  glfwMakeContextCurrent(Window);
  glfwSetKeyCallback(Window, KeyCallback);
  glfwSetCursorPosCallback(Window, MouseCallback);
  glfwSetMouseButtonCallback(Window, MouseButtonCallback);
  glfwSetScrollCallback(Window, ScrollCallback);
  
  glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glewExperimental = true;
  if (glewInit() != GLEW_OK)
  {
    fprintf(stderr, "Error in initializing GLEW!\n");
    return(-1);
  }

  glViewport(0, 0, WIDTH, HEIGHT);
  glEnable(GL_DEPTH_TEST);

  struct vec3 CameraStartPosition = { 1.2f, 2.0f, 6.0f };
  struct vec3 CameraDirection = { 0.0f, 0.0f, -1.0f };
  struct vec3 CameraUp = { 0.0f, 1.0f, 0.0f };
  Camera = CreateCamera(CameraStartPosition, CameraDirection, CameraUp);

  struct shader InitShader = CreateShader("../shaders/init_shader.vs", "../shaders/init_shader.frag");

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
  printf("%s\n", glGetString(GL_VERSION));

  u8 ContainerImage = CreateTexture("../texture_and_images/container.jpg");

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, ContainerImage);

  struct vec3 ModelPos = { 1.2f, 1.0f, 4.0f };

  while (!glfwWindowShouldClose(Window))
  {
    f64 CurrentFrame = glfwGetTime();
    DeltaTime = CurrentFrame - LastFrame;
    LastFrame = CurrentFrame;

    glfwPollEvents();
    CameraMovement();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    UseShader(InitShader);

    struct mat4 ViewLookAt = GetViewMatrix(&Camera);
    struct mat4 Projection = Mat4Perspective(Camera.Zoom, (f32)WIDTH / (f32)HEIGHT, 0.1f, 100.0f);

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

    glfwSwapBuffers(Window);
  }
  glDeleteVertexArrays(1, &ContainerVAO);
  glDeleteBuffers(1, &VBO);
  glfwDestroyWindow(Window);
  
  glfwTerminate();
  return(0);
}

void
KeyCallback(GLFWwindow *Window, i32 Key, i32 ScanCode, i32 Action, i32 Mode)
{
  if (Action == GLFW_PRESS || Action == GLFW_REPEAT)
  {
    Keys[Key] = true;
  }
  else if (Action == GLFW_RELEASE)
  {
    Keys[Key] = false;
  }

  if (Keys[GLFW_KEY_ESCAPE])
  {
    glfwSetWindowShouldClose(Window, true);
    printf("Exiting...\n");
  }

  // Wireframe mode
  if (Keys[GLFW_KEY_1])
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    printf("Wireframe mode\n");
  }
  // Filled mode
  if (Keys[GLFW_KEY_2])
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    printf("Filled mode\n");
  }
}

void 
CameraMovement()
{
  // Camera movement
  if (Keys[GLFW_KEY_W])
  {
    ProcessKeyboard(&Camera, FORWARD, (f32)DeltaTime);
  }
  if (Keys[GLFW_KEY_S])
  {
    ProcessKeyboard(&Camera, BACKWARD, (f32)DeltaTime);
  }
  if (Keys[GLFW_KEY_A])
  {
    ProcessKeyboard(&Camera, LEFT, (f32)DeltaTime);
  }
  if (Keys[GLFW_KEY_D])
  {
    ProcessKeyboard(&Camera, RIGHT, (f32)DeltaTime);
  }
}

void
MouseCallback(GLFWwindow *Window, f64 XPos, f64 YPos)
{
  if (FirstMouse)
  {
    LastX = (f32)XPos;
    LastY = (f32)YPos;
    FirstMouse = false;
  }

  f32 XOffset = (f32)XPos - LastX;
  f32 YOffset = LastY - (f32)YPos;

  LastX = (f32)XPos;
  LastY = (f32)YPos;

  ProcessMouseMovement(&Camera, XOffset, YOffset, true);
}

void
MouseButtonCallback(GLFWwindow *Window, i32 Button, i32 Action, i32 Mode)
{
  if(Button == GLFW_MOUSE_BUTTON_LEFT && Action == GLFW_PRESS)
  {
    printf("Left mouse button was clicked\n");
  }
  if(Button == GLFW_MOUSE_BUTTON_LEFT && Action == GLFW_RELEASE)
  {
    printf("Left mouse button was released\n");
  }

  if(Button == GLFW_MOUSE_BUTTON_RIGHT && Action == GLFW_PRESS)
  {
    printf("Right mouse button was clicked\n");
  }
  if(Button == GLFW_MOUSE_BUTTON_RIGHT && Action == GLFW_RELEASE)
  {
    printf("Right mouse button was released\n");
  }
}

void
ScrollCallback(GLFWwindow *Window, f64 XOffset, f64 YOffset)
{
  ProcessMouseScroll(&Camera, (f32)YOffset);
}

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "Includes/shader.h"
#include "Includes/math_lib.h"
#include "Includes/camera.h"
#include "Includes/textures.h"
#include "Includes/light.h"
#include "Includes/typedefs.h"

void
KeyCallback(GLFWwindow *Window, int32 Key, int32 ScanCode, int32 Action, int32 Mode);

void
CameraMovement();

void
MouseCallback(GLFWwindow *Window, float64 XPos, float64 YPos);

void
MouseButtonCallback(GLFWwindow *Window, int32 Button, int32 Action, int32 Mode);

void
ScrollCallback(GLFWwindow *Window, float64 XOffset, float64 YOffset);

// Window resolution
#define WIDTH 1028
#define HEIGHT 768
#define TITLE "DEADBEEF Engine"

// Camera
struct camera Camera;
float32 LastX = WIDTH / 2.0f;
float32 LastY = HEIGHT / 2.0f;
bool Keys[1024];

bool FirstMouse = true;

float64       DeltaTime = 0.0f;
float64       LastFrame = 0.0f;
const float32 FrameRate = 60.0f;

int32
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

  struct shader InitShader = CreateShader("../Shaders/init_shader.vs", "../Shaders/init_shader.frag");

  float32 Vertices[] =
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
  uint32 VBO, ContainerVAO;
  glGenVertexArrays(1, &ContainerVAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

  glBindVertexArray(ContainerVAO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 5 * sizeof(float32), (GLvoid*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, 5 * sizeof(float32), 
                                                  (GLvoid*)(3 * sizeof(float32)));
  glEnableVertexAttribArray(2);
  
  glBindVertexArray(0);

  // Print version information to the command line
  printf("%s\n", glGetString(GL_VERSION));

  uint8 ContainerImage = CreateTexture("../texture_and_images/container.jpg");

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, ContainerImage);

  struct vec3 ModelPos = { 1.2f, 1.0f, 4.0f };

  while (!glfwWindowShouldClose(Window))
  {
    float64 CurrentFrame = glfwGetTime();
    DeltaTime = CurrentFrame - LastFrame;
    LastFrame = CurrentFrame;

    glfwPollEvents();
    CameraMovement();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    UseShader(InitShader);

    struct mat4 ViewLookAt = GetViewMatrix(&Camera);
    struct mat4 Projection = Mat4Perspective(Camera.Zoom, (float32)WIDTH / (float32)HEIGHT, 0.1f, 100.0f);

    int32 ModelLoc = glGetUniformLocation(InitShader.Program, "model");
    int32 ViewLoc = glGetUniformLocation(InitShader.Program, "view");
    int32 ProjLoc = glGetUniformLocation(InitShader.Program, "projection");

    glUniformMatrix4fv(ViewLoc, 1, GL_FALSE, (float32*)&ViewLookAt);
    glUniformMatrix4fv(ProjLoc, 1, GL_FALSE, (float32*)&Projection);
    
    glBindVertexArray(ContainerVAO);
    struct mat4 Model = Mat4Translate(&ModelPos);
    glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, (float32*)&Model);
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
KeyCallback(GLFWwindow *Window, int32 Key, int32 ScanCode, int32 Action, int32 Mode)
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
    ProcessKeyboard(&Camera, FORWARD, (float32)DeltaTime);
  }
  if (Keys[GLFW_KEY_S])
  {
    ProcessKeyboard(&Camera, BACKWARD, (float32)DeltaTime);
  }
  if (Keys[GLFW_KEY_A])
  {
    ProcessKeyboard(&Camera, LEFT, (float32)DeltaTime);
  }
  if (Keys[GLFW_KEY_D])
  {
    ProcessKeyboard(&Camera, RIGHT, (float32)DeltaTime);
  }
}

void
MouseCallback(GLFWwindow *Window, float64 XPos, float64 YPos)
{
  if (FirstMouse)
  {
    LastX = (float32)XPos;
    LastY = (float32)YPos;
    FirstMouse = false;
  }

  float32 XOffset = (float32)XPos - LastX;
  float32 YOffset = LastY - (float32)YPos;

  LastX = (float32)XPos;
  LastY = (float32)YPos;

  ProcessMouseMovement(&Camera, XOffset, YOffset, true);
}

void
MouseButtonCallback(GLFWwindow *Window, int32 Button, int32 Action, int32 Mode)
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
ScrollCallback(GLFWwindow *Window, float64 XOffset, float64 YOffset)
{
  ProcessMouseScroll(&Camera, (float32)YOffset);
}

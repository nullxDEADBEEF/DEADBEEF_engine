#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h> // Include glew to get all the required OpenGL headers

#include "typedefs.h"

struct shader
{
  int32 Program;
};

// Loads, compiles and attach shaders to a program
struct shader 
CreateShader(const GLchar *VertexPath, const GLchar *FragmentPath)
{
  struct shader Shader;
  // Retrieve the vertex/fragment source code from the filepaths
  FILE *VShaderFile;
  FILE *FShaderFile;

  // Open files
  VShaderFile = fopen(VertexPath, "r");
  FShaderFile = fopen(FragmentPath, "r");

  if (VShaderFile == NULL)
  {
    fprintf(stderr, "Error opening vertex shader file: %s\n", VertexPath);
  }
  if (FShaderFile == NULL)
  {
    fprintf(stderr, "Error opening fragment shader file: %s\n", FragmentPath);
  }

  // Get the size of files
  // We seek the end of the file then ask for its position
  fseek(VShaderFile, 0, SEEK_END);
  fseek(FShaderFile, 0, SEEK_END);
  int64 VShaderFileSize = ftell(VShaderFile);
  int64 FShaderFileSize = ftell(FShaderFile);
  // Then we seek back to the beginning
  rewind(VShaderFile);
  rewind(FShaderFile);

  // Allocate memory for the file's contents
  // and set the memory to 0 to avoid garbage data
  GLchar *VertexBuffer = (GLchar*)calloc(VShaderFileSize + 1, sizeof(GLchar));
  GLchar *FragmentBuffer = (GLchar*)calloc(FShaderFileSize + 1, sizeof(GLchar));

  // Read the file's contents
  fread(VertexBuffer, 1, VShaderFileSize, VShaderFile);
  fread(FragmentBuffer, 1, FShaderFileSize, FShaderFile);

  VertexBuffer[strlen(VertexBuffer)] = '\0';
  FragmentBuffer[strlen(FragmentBuffer)] = '\0';

  // Close the file handlers
  fclose(VShaderFile);
  fclose(FShaderFile);
 

  // Compiler shaders
  int32 Vertex;
  uint32 Fragment;
  int32 Success;
  GLchar InfoLog[512];

  int32 VertexBufferLength = (int32)strlen(VertexBuffer);
  int32 FragmentBufferLength = (int32)strlen(FragmentBuffer);

  // Vertex Shader
  Vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(Vertex, 1, (const GLchar**)&VertexBuffer, &VertexBufferLength);
  glCompileShader(Vertex);
  // Print compile errors if any
  glGetShaderiv(Vertex, GL_COMPILE_STATUS, &Success);
  if (!Success)
  {
    glGetShaderInfoLog(Vertex, 512, NULL, InfoLog);
    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", InfoLog);
  };

  // Fragment Shader
  Fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(Fragment, 1, (const GLchar**)&FragmentBuffer, 
                                              &FragmentBufferLength);
  glCompileShader(Fragment);
  // Print compile errors if any
  glGetShaderiv(Fragment, GL_COMPILE_STATUS, &Success);
  if (!Success)
  {
    glGetShaderInfoLog(Fragment, 512, NULL, InfoLog);
    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", InfoLog);
  };

  // Shader Program
  Shader.Program = glCreateProgram();
  glAttachShader(Shader.Program, Vertex);
  glAttachShader(Shader.Program, Fragment);
  glLinkProgram(Shader.Program);
  // Print linking errors if any
  glGetProgramiv(Shader.Program, GL_LINK_STATUS, &Success);
  if (!Success)
  {
    glGetProgramInfoLog(Shader.Program, 512, NULL, InfoLog);
    printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", InfoLog);
  };

  // Delete shaders as they are linked into our program
  // now and no longer necessery
  glDeleteShader(Vertex);
  glDeleteShader(Fragment);

  // Returning our earlier allocated memory
  free(VertexBuffer);
  free(FragmentBuffer);

  return(Shader);
}

void 
UseShader(const struct shader Shader)
{
  glUseProgram(Shader.Program);
}

#endif

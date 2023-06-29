// SPDX-FileCopyrightText: Joey De Vries
//
// SPDX-License-Identifier: CC-BY-4.0

#include <Shader.hpp>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>

int main()
{
  if (!glfwInit())
  {
    std::cout << "failed to initialize glfw" << std::endl;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  const int initial_width = 800;
  const int initial_height = 600;

  GLFWwindow* window =
    glfwCreateWindow(
      initial_width,
      initial_height,
      "LearnOpenGL",
      NULL,
      NULL
    );
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  glfwSetFramebufferSizeCallback(
    window,
    [](GLFWwindow* window, int width, int height)
    {
      glViewport(0, 0, width, height);
    }
  );

  glfwSetKeyCallback(
    window,
    [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
      if(
        key == GLFW_KEY_ESCAPE &&
        action == GLFW_PRESS
      )
      {
        glfwSetWindowShouldClose(window, true);
      }
    }
  );

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, initial_width, initial_height);

  ogl::Shader shader;
  shader.Load("shader.vs.glsl", "shader.fs.glsl");

  GLfloat vertices[] =
    {
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top
    };

  GLuint vbo, vao;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // This will not undo the vbo being bound to the vao.
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  while(!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.Use();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(GL_NONE);
    ogl::Shader::UseNone();

    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  shader.Unload();

  glfwTerminate();
  return 0;
}

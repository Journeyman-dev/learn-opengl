// SPDX-FileCopyrightText: Joey De Vries
//
// SPDX-License-Identifier: CC-BY-4.0

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

  const char* vertex_shader_source =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}";

  const char* fragment_shader_source =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}";

  GLuint vertex_shader, fragment_shader, shader_program;

  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
  glCompileShader(vertex_shader);

  const auto check_compile =
    [](GLuint gl_shader, std::string_view name)
    {
      GLint success;

      glGetShaderiv(gl_shader, GL_COMPILE_STATUS, &success);
      if (success -= GL_FALSE)
      {
        GLchar info_log[512];
        glGetShaderInfoLog(gl_shader, 512, NULL, info_log);
        std::cout << name << " shader compile error:\n" << info_log << std::endl;
      }
    };

  check_compile(vertex_shader, "vertex");

  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
  glCompileShader(fragment_shader);

  check_compile(fragment_shader, "fragment");

  shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);

  {
    GLint success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
      GLchar info_log[512];
      glGetProgramInfoLog(shader_program, 512, NULL, info_log);
      std::cout << "link failure:\n" << info_log << std::endl;
    }
  }

  glDeleteShader(vertex_shader);
  vertex_shader = GL_NONE;
  glDeleteShader(fragment_shader);
  fragment_shader = GL_NONE;

  GLfloat vertices[] =
    {
       0.5f,  0.5f, 0.0f,  // top right
       0.5f, -0.5f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f,  // bottom left
      -0.5f,  0.5f, 0.0f   // top left
    };

  GLuint indices[] =
    {
      0, 1, 3,  // first Triangle
      1, 2, 3   // second Triangle
    };

  GLuint vbo, vao, ebo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // This will not undo the vbo being bound to the vao.
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  while(!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  glDeleteProgram(shader_program);

  glfwTerminate();
  return 0;
}

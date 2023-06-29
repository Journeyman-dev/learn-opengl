// SPDX-FileCopyrightText: Joey De Vries
//
// SPDX-License-Identifier: CC-BY-4.0

#include <stb_image.h>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <filesystem>

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

  auto read_file_text =
    [](const std::filesystem::path& path)
    {
      try
      {
          std::ifstream file;
          file.open(path);
          std::stringstream stream;
          stream << file.rdbuf();
          file.close();
          return stream.str();
      }
      catch(std::ifstream::failure& e)
      {
          std::cout << "failed to read file \"" << path << "\": " << e.what();
      }
      std::unreachable();
    };

  GLuint vertex_shader, fragment_shader, shader_program;

  auto vertex_source = read_file_text("shader.vs.glsl");
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  GLchar* vertex_source_ptr = vertex_source.data();
  glShaderSource(vertex_shader, 1, &vertex_source_ptr, NULL);
  glCompileShader(vertex_shader);

  const auto check_compile =
    [](GLuint gl_shader, std::string_view name)
    {
      GLint success;
      glGetShaderiv(gl_shader, GL_COMPILE_STATUS, &success);
      if (success == GL_FALSE)
      {
        GLchar info_log[512];
        glGetShaderInfoLog(gl_shader, 512, NULL, info_log);
        std::cout << name << " shader compile error:\n" << info_log << std::endl;
      }
    };

  check_compile(vertex_shader, "vertex");

  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  auto fragment_source = read_file_text("shader.fs.glsl");
  GLchar* fragment_source_ptr = fragment_source.data();
  glShaderSource(fragment_shader, 1, &fragment_source_ptr, NULL);
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
  glDeleteShader(fragment_shader);

  GLfloat vertices[] =
    {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

  GLuint indices[] =
    {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
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
  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture coord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  GLuint texture1, texture2;
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  int width, height, channels;
  stbi_set_flip_vertically_on_load(true);
  stbi_uc* data = stbi_load("container.jpg", &width, &height, &channels, 3);
  if (data)
  {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
      std::cout << "failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  stbi_set_flip_vertically_on_load(true);
  data = stbi_load("awesomeface.png", &width, &height, &channels, 4);
  if (data)
  {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
      std::cout << "failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  while(!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glUniform1i(
      glGetUniformLocation(
        shader_program,
        "texture1"
      ),
      0
    );
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glUniform1i(
      glGetUniformLocation(
        shader_program,
        "texture2"
      ),
      1
    );
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  glDeleteProgram(shader_program);
  glDeleteTextures(1, &texture1);
  glDeleteTextures(1, &texture2);

  glfwTerminate();
  return 0;
}

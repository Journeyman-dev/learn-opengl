// SPDX-FileCopyrightText: Joey De Vries
//
// SPDX-License-Identifier: CC-BY-4.0

#include <Shader.hpp>
#include <iostream>
#include <fstream>
#include <utility>

namespace ogl
{
    void Shader::Load(const std::filesystem::path& vertex_shader_path, const std::filesystem::path& fragment_shader_path)
    {
        const auto vertex_shader_source = this->getFileText(vertex_shader_path);
        const auto fragment_shader_source = this->getFileText(fragment_shader_path);
        GLuint vertex_shader, fragment_shader;
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        const GLchar* vertex_shader_source_ptr = vertex_shader_source.data();
        glShaderSource(vertex_shader, 1, &vertex_shader_source_ptr, NULL);
        glCompileShader(vertex_shader);

        this->checkCompile(vertex_shader, ogl::Shader::Type::Vertex);

        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        const GLchar* fragment_shader_source_ptr = fragment_shader_source.data();
        glShaderSource(fragment_shader, 1, &fragment_shader_source_ptr, NULL);
        glCompileShader(fragment_shader);

        this->checkCompile(fragment_shader, ogl::Shader::Type::Fragment);

        this->gl_program = glCreateProgram();
        glAttachShader(this->gl_program, vertex_shader);
        glAttachShader(this->gl_program, fragment_shader);
        glLinkProgram(this->gl_program);
    }

    bool Shader::GetIsLoaded() const noexcept
    {
        return this->gl_program != GL_NONE;
    }

    void Shader::Unload() noexcept
    {
        if (!this->GetIsLoaded()) return;
        glDeleteProgram(this->gl_program);
    }

    void Shader::Use() noexcept
    {
        glUseProgram(this->gl_program);
    }

    void Shader::UseNone() noexcept
    {
        glUseProgram(GL_NONE);
    }

    void Shader::SetBool(std::string_view name, bool value) noexcept
    {
        glUniform1i(
            glGetUniformLocation(this->gl_program, name.data()),
            static_cast<GLint>(value)
        );
    }

    void Shader::SetInt(std::string_view name, int value) noexcept
    {
        glUniform1i(
            glGetUniformLocation(this->gl_program, name.data()),
            value
        );
    }

    void Shader::SetFloat(std::string_view name, float value) noexcept
    {
        glUniform1f(
            glGetUniformLocation(this->gl_program, name.data()),
            value
        );
    }

    void Shader::checkCompile(GLuint gl_shader, ogl::Shader::Type type) const
    {
      GLint success;
      glGetShaderiv(gl_shader, GL_COMPILE_STATUS, &success);
      if (success == GL_FALSE)
      {
        GLchar info_log[512];
        glGetShaderInfoLog(gl_shader, 512, NULL, info_log);
        std::cout << ogl::Shader::TypeToString(type) << " shader compile error:\n" << info_log << std::endl;
      }
    }

    void Shader::checkLink() const
    {
        GLint success;
        glGetProgramiv(this->gl_program, GL_LINK_STATUS, &success);
        if (success == GL_FALSE)
        {
            GLchar info_log[512];
            glGetProgramInfoLog(this->gl_program, 512, NULL, info_log);
            std::cout << "link failure:\n" << info_log << std::endl;
        }
    }

    std::string Shader::getFileText(const std::filesystem::path& path) const
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
    }
}
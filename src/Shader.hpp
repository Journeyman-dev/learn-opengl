// SPDX-FileCopyrightText: Joey De Vries
//
// SPDX-License-Identifier: CC-BY-4.0

#pragma once

#include <glad/glad.h>
#include <filesystem>
#include <string>

namespace ogl
{
    class Shader
    {
        public:
            enum class Type
            {
                Vertex,
                Fragment
            };

            static constexpr std::string_view TypeToString(ogl::Shader::Type type)
            {
                switch(type)
                {
                    case ogl::Shader::Type::Vertex:
                        return "vertex";
                    case ogl::Shader::Type::Fragment:
                        return "fragment";
                }
                return "unknown";
            }
        public:
            constexpr Shader() noexcept = default;
            Shader(const ogl::Shader& that) = delete;

            ogl::Shader& operator=(const ogl::Shader& rhs) = delete;

            void Load(const std::filesystem::path& vertex_shader_path, const std::filesystem::path& fragment_shader_path);
            bool GetIsLoaded() const noexcept;
            void Unload() noexcept;

            void Use() noexcept;
            static void UseNone() noexcept;

            void SetBool(std::string_view name, bool value) noexcept;
            void SetInt(std::string_view name, int value) noexcept;
            void SetFloat(std::string_view name, float value) noexcept;
        private:
            GLuint gl_program = GL_NONE;

            void checkCompile(GLuint gl_shader, ogl::Shader::Type type) const;
            void checkLink() const;
            std::string getFileText(const std::filesystem::path& path) const;
    };
}
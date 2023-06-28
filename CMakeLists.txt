# SPDX-FileCopyrightText: Joey De Vries
#
# SPDX-License-Identifier: CC-BY-4.0

cmake_minimum_required(VERSION 3.26.3)

set(CMAKE_CXX_STANDARD 23)

project(learn_opengl)

add_executable(${PROJECT_NAME} "")

add_subdirectory(src)

include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY          https://github.com/glfw/glfw
    GIT_TAG                 3.3.8
    FIND_PACKAGE_ARGS       "3.3.8" NAMES glfw3 CONFIG
)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
FetchContent_MakeAvailable(glfw)

include(FetchContent)
FetchContent_Declare(
    glad
    GIT_REPOSITORY          https://github.com/Dav1dde/glad
    GIT_TAG                 v0.1.36
    FIND_PACKAGE_ARGS       "0.1.36" NAMES glad CONFIG
)
FetchContent_MakeAvailable(glad)

include(FetchContent)
FetchContent_Declare(
    glm
    GIT_REPOSITORY			https://github.com/g-truc/glm
    GIT_TAG					0.9.9.8
    FIND_PACKAGE_ARGS		"0.9.9.8" NAMES glm CONFIG
)
FetchContent_MakeAvailable(glm)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        glfw
        glad
        glm
)



<!--
SPDX-FileCopyrightText: Joey De Vries

SPDX-License-Identifier: CC-BY-4.0
-->

# Learn OpenGL Tutorial

[![REUSE status](https://api.reuse.software/badge/git.fsfe.org/reuse/api)](https://api.reuse.software/info/git.fsfe.org/reuse/api)

This repository contains code examples that follow the tutorial from <https://learnopengl.com/> by [Joey de Vries](https://twitter.com/JoeyDeVriez). The code here is not official, but was created by me (Daniel Aimé Valcour). The branches of this repository shows the state of my code after completing each page of the tutorial.

# How To Use
Instead of using Visual Studio, the project is defined in a cross platform manner using CMake. If you wish to generate it yourself, you need to run the following bash commands:

    git clone https://github.com/Journeyman-dev/learn-opengl
    cd learn-opengl
    # replace the -G option with the generator you want from https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html
    cmake -S . -B ./build/ -G "Visual Studio 17 2022" 
    # You can find the project in the folder named "build"

If you want to build the project without opening it in an IDE, you can use this command:

    cmake --build ./build/ --config Debug

If you want to add new source files, add them to the `src` folder and add their file names to the `target_sources` command in the <src/CMakeLists.txt>. If you do this, you may need to delete the build folder and re-generate the project. If you want to add include files, just add them to the `src` folder and you are good to go.
If you want to simplify this process, I recommend that you use the "CMake Tools" in Visual Studio Code. It comes as a part of the "C/C++ Extension Pack" (<https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack>), along with everything else you need.


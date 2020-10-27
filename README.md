# sundile

sundile is a 100% open-source, Modern C++, ECS-based game engine prototype written by [Phoenix Mandala](https://github.com/ada-rose-dev).

It depends on the following libraries:

* [assimp](http://assimp.org/)
* [DearImGUI](https://github.com/ocornut/imgui)
* [EnTT](https://github.com/skypjack/entt)
* [GLFW](https://www.glfw.org/)
* [glm](https://glm.g-truc.net/0.9.9/index.html)
* [stb](https://github.com/nothings/stb)

## installation
The CMake files used to build this project on my personal machines are provided as a basis for your own installations. I am using Clang 10.0.0 for MSVC on Windows and Apple Clang for OS X.
The headers for EnTT, stb, and ImGui GLFW/OpenGL implementations are shipped with the project. You will need to include the rest of the dependencies on your own machine. I recommend using [vcpkg](https://github.com/Microsoft/vcpkg).

## usage
Sundile is an ECS-based engine. To create a component, add your component header file to src/components. To add a system, add your system header file to src/headers. There are plenty of examples and I like to think they're fairly self-explanatroy :) In order for your components and systems to be visible to sundile, you will need to include them in src/components/AllComponents.h and src/systems/AllSystems.h, respectively.

## current status
Development is active. Currently implementing GUI inspection tools.
Sundile is being developed cross-platform in Visual Studio Code for Windows 10 and OS X Catalina. Linux port coming eventually.
### TODO:
* GUI Implementation
    * Automatically register entities for GUI inspection on creation
    * Allow GUI to modify values
    * Create entities from sundile
    * Hot load changes to preexisting systems/components
    * Hot load systems/components on creation (requires automated inclusion)
* Project System
    * Create, load, and save scenes
    * Implement scene tools

## future plans / currently out of scope
Depending on interest, I would love to create the following for this project:
* Precompiled systems/component headers
* Automated header inclusion
* C++20 Module integration
* Package manager for components, systems, and assets

## contribution
yeah i mean if you want to! go for it! just like! let me know :)

## license
sundile is distributed under GPL3:
```
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
```

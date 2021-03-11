# sundile

sundile is a 100% open-source, Modern C++, ECS-based game engine prototype written by [ada](https://github.com/ada-rose-dev).

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

## Usage
Sundile is an ECS-based engine. To create a component, add your component header file to src/components. To add a system, add your system header file to src/headers. There are plenty of examples and I like to think they're fairly self-explanatroy :) In order for your components and systems to be visible to sundile, you will need to include them in src/components/AllComponents.h and src/systems/AllSystems.h, respectively.

### Intended 1.0 Workflow
* Create entities, components, and systems from an intuitive GUI.
* Drag'n'drop components onto entities to build game elements.
    * Components are lists of data structures.
    * Entities are lists of components with specified default values.
    * Systems input components, perform changes on their values, and returns them to all the relevant entities.
* Create systems and edit C++ code to create truly interactive, systematic gameplay
* Manipulate entities in 3D space through a modifiable visual editor
* Save and edit projects.
* Run your game and view component values in real time.

### Currently out of scope
Depending on interest, I would love to create the following for this project:
* Precompiled systems/component headers
* Automated header inclusion
* C++20 Module integration
* Package manager for components, systems, and assets
* Hotloading

### TODO:
* GUI Implementation
    * Clipboard functionality
    * Create and edit entities, components, and systems from GUI
    * Allow users to create custom types for component editor
    * Open systems in external editor and hotload changes
* Project System
    * Create, load, and save scenes
    * Implement scene tools


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

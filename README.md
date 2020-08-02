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
The CMake files used to build this project on my personal machines are provided as a basis for your own installations.
The EnTT header is included in the source. You will need to include the rest of the dependencies on your own machine. I recommend using [vcpkg](https://github.com/Microsoft/vcpkg).

## current status
Development is active.
It is being developed cross-platform on Windows 10 and OS X Catalina. Linux port coming soon.

## future plans
Depending on interest, I would love to create the following for this project:
* GUI-based interface
* Component package manager

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

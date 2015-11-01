![Parcel Game Engine](https://github.com/DonaldWhyte/parcel-game-engine/raw/wiki/parcel_logo.png)

**WARNING:** I stopped actively maintaining this project in 2009. Therefore, no new versions or bug fixes will be provided. The project's source code is provided for people who wish to use the engine, or its components, for their own game or project.

Parcel Game Engine's main goal is to abstract and simplify the low level details of a game (e.g. graphics rendering and resource management) away from the user, so that they may focus on developing the actual game instead. It is primary aimed towards aiding the development of quick prototypes, but can be used for full-fledged games as well.

Parcel has few dependencies, so the amount of setup required to start using it is very small. All you need is a standards compliant C++ compiler and the included header files and binaries provided in the Downloads section. For a quick code example on how to get a Parcel application up and running, check out the ExampleApplication page.

### Screenshots

[!<img src="https://github.com/DonaldWhyte/parcel-game-engine/raw/wiki/https://github.com/DonaldWhyte/parcel-game-engine/raw/wiki/Parcel%20Game%20Engine%20Screenshot%201.png" width="220" alt="rendering mesh with custom shaders" />](https://github.com/DonaldWhyte/parcel-game-engine/raw/wiki/Parcel%20Game%20Engine%20Screenshot%201.png)

[![custom shaders in use in a 3D space space](https://github.com/DonaldWhyte/parcel-game-engine/raw/wiki/TODO)](https://github.com/DonaldWhyte/parcel-game-engine/raw/wiki/Parcel%20Game%20Engine%20Screenshot%202.png)

[![hardware accelerated 2D sprite and text rendering](z)](https://github.com/DonaldWhyte/parcel-game-engine/raw/wiki/Parcel%20Game%20Engine%20Screenshot%203.png)

### Features

##### Hardware Accelerated 2D Graphics

* Bitmaps and sprites
* Dynamic font loading and rendering

##### Hardware Accelerated 3D Graphics

* Texture loading and automatic mipmap generation
* Rendering textured 3D meshes and other geometry
* Supports multiple camera types, such as first person and orbit
* GLSL shader support

##### General

* Debugging support through exceptions and logging
* OpenGL context and window management done for you
* Resource management (e.g. for textures and shaders) so you don't have to worry about memory leaks
* In-built vector, matrix and quaternion classes for common math problems
* Lua scripting support for creating game logic easily
* Playing WAV and OGG sound files

##### Data File Support

* Loading Data From Files
* Image data from TGA files
* Mesh and material data from OBJ or DMF (custom format) files
* Font data from TTF files
* Vertex and fragment shaders from text files

### Build Steps

TODO

### Example Application

TODO

### License

This library is licensed under the MIT License. See [LICENSE](https://github.com/DonaldWhyte/parcel-game-engine/raw/master/LICENSE) for the exact license used.

![Parcel Game Engine](https://github.com/DonaldWhyte/parcel-game-engine/raw/wiki/parcel_logo.png)

**WARNING:** I stopped actively maintaining this project in 2009. Therefore, no new versions or bug fixes will be provided. The project's source code is provided for people who wish to use the engine, or its components, for their own game or project.

Parcel Game Engine's main goal is to abstract and simplify the low level details of a game (e.g. graphics rendering and resource management) away from the user, so that they may focus on developing the actual game instead. It is primary aimed towards aiding the development of quick prototypes, but can be used for full-fledged games as well.

Parcel has few dependencies, so the amount of setup required to start using it is very small. All you need to do is build Parcel using a standards compliant C++ compiler (ensuring you have the dependencies listed in the [Building Parcel](#building-parcel) section) and you're good to go!

See the [**Screenshots**](#screenshots) section below to see the scenes which have been rendered using Parcel.

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

### Building Parcel

To build Parcel, ensure:

* the standard C++ library and the headers of the dependencies listed below are added to your compiler's include path
* static libraries for the dependencies are added to the compiler's library path, or DLLs of those libraries are in the working directory when you execute applications which use Parcel

Dependencies:

* windows.h -- Parcel Game Engine is currently Windows only
* GLu -- OpenGL utility library
* [GLee](https://www.opengl.org/sdk/libs/GLee/)
* [irrKlang](http://www.ambiera.com/irrklang/)
* [freetype2](http://freetype.sourceforge.net/freetype2/)
* [lua](http://www.lua.org/) -- development headers and built libraries for the Lua interpreter

### Example Application

See [`examples/helloworld.cpp`](https://github.com/DonaldWhyte/parcel-game-engine/blob/master/examples/helloworld.cpp) for small example program that shows how to get a Parcel application up and running. The example loads a font from a file called `arial.ttf` and renders the text "Hello world!" on the screen.

### License

This library is licensed under the MIT License. See [LICENSE](https://github.com/DonaldWhyte/parcel-game-engine/raw/master/LICENSE) for the exact license used.

### Screenshots

Rendering a mesh with custom shaders and post processor effects for shadows, reflection and anti-aliasing. Car mesh created by [Ali Fayez](http://www.ali3d.8m.com/).

[![rendering mesh with custom shaders](https://github.com/DonaldWhyte/parcel-game-engine/raw/wiki/Parcel%20Game%20Engine%20Screenshot%201.png)](https://github.com/DonaldWhyte/parcel-game-engine/raw/wiki/Parcel%20Game%20Engine%20Screenshot%201.png)

Parcel's custom shader feature again, with a simple cel shader and multiple light sources.

[![custom shaders in use for a 3D space space](https://github.com/DonaldWhyte/parcel-game-engine/raw/wiki/Parcel%20Game%20Engine%20Screenshot%202.png)](https://github.com/DonaldWhyte/parcel-game-engine/raw/wiki/Parcel%20Game%20Engine%20Screenshot%202.png)

Parcel's hardware accelerated 2D sprite and font rendering.

[![hardware accelerated 2D sprite and text rendering](https://github.com/DonaldWhyte/parcel-game-engine/raw/wiki/Parcel%20Game%20Engine%20Screenshot%203.png)](https://github.com/DonaldWhyte/parcel-game-engine/raw/wiki/Parcel%20Game%20Engine%20Screenshot%203.png)

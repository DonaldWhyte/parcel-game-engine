![http://wiki.parcel-game-engine.googlecode.com/hg/parcel_logo.png](http://wiki.parcel-game-engine.googlecode.com/hg/parcel_logo.png)

_**WARNING:**_ All development on this project has ended, so no new versions or bug fixes will be provided. The project's binaries and source code are merely provided for people who wish to use the engine, or its components, for their own game or project.

Parcel Game Engine's main goal is to abstract and simplify the low level details of a game  (e.g. graphics rendering and resource management) away from the user, so that they may focus on developing the actual game instead. It is primary aimed towards aiding the development of quick prototypes, but can be used for full-fledged games as well.

Parcel has few dependencies, so the amount of setup required to start using it is very small. All you need is a standards compliant C++ compiler and the included header files and binaries provided in the **Downloads** section. For a quick code example on how to get a Parcel application up and running, check out the [ExampleApplication](ExampleApplication.md) page.

# Features #

  * **Hardware Accelerated 2D Graphics**
    * Bitmaps and sprites
    * Dynamic font loading and rendering
  * **Hardware Accelerated 3D Graphics**
    * Texture loading and automatic mipmap generation
    * Rendering textured 3D meshes and other geometry
    * Supports multiple camera types, such as first person and orbit
    * GLSL shader support
  * **General**
    * Debugging support through exceptions and logging
    * OpenGL context and window management done for you
    * Resource management (e.g. for textures and shaders) so you don't have to worry about memory leaks
    * In-built vector, matrix and quaternion classes for common math problems
    * Lua scripting support for creating game logic easily
    * Playing WAV and OGG sound files
  * **Loading Data From Files**
    * Image data from TGA files
    * Mesh and material data from OBJ or DMF (custom format) files
    * Font data from TTF files
    * Vertex and fragment shaders from text files

# Screenshots #

| <a href='http://wiki.parcel-game-engine.googlecode.com/hg/Parcel%20Game%20Engine%20Screenshot%201.png'><img src='http://wiki.parcel-game-engine.googlecode.com/hg/Parcel%20Game%20Engine%20Screenshot%201.png' width='20%' height='20%' /></a> | Rendering a mesh with custom shaders, shadows and post processing effects for reflection and anti-aliasing. Car model made by [Ali Fayez](http://www.ali3d.8m.com). |
|:-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|:--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| <a href='http://wiki.parcel-game-engine.googlecode.com/hg/Parcel%20Game%20Engine%20Screenshot%202.png'><img src='http://wiki.parcel-game-engine.googlecode.com/hg/Parcel%20Game%20Engine%20Screenshot%202.png' width='20%' height='20%' /></a> <a href='http://wiki.parcel-game-engine.googlecode.com/hg/Parcel%20Game%20Engine%20Screenshot%202%20%28Old%29.png'><img src='http://wiki.parcel-game-engine.googlecode.com/hg/Parcel%20Game%20Engine%20Screenshot%202%20%28Old%29.png' width='20%' height='20%' /></a> | Demonstrating Parcel's custom shader feature again, with a simple cel shader and multiple light sources. |
| <a href='http://wiki.parcel-game-engine.googlecode.com/hg/Parcel%20Game%20Engine%20Screenshot%203.png'><img src='http://wiki.parcel-game-engine.googlecode.com/hg/Parcel%20Game%20Engine%20Screenshot%203.png' width='20%' height='20%' /></a> | Parcel's hardware accelerated 2D sprite and font rendering. |

All 3D models were acquired from [TurboSquid](http://turbosquid.com) unless explicitly mentioned. No ownership on said models is implied, all credit goes to the original authors - they were merely used to demonstrate Parcel Game Engine's capabilities.

# Notes #

Parcel Game Engine uses the New BSD License - the full license is available on the [License](License.md) page.

Project icon created by Thvg and is licensed under CC Attribution-Noncommercial-No Derivative Works 3.0.
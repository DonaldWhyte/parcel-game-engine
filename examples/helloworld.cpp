/**
 * Parcel Example -- Hello World
 *
 * This example loads a TTF font from a file and uses it to render the text
 * "Hello World" in a 3D scene.
 *
 * For this example to work, Parcel's include directory must be to the list of
 * include subdirectories used at compilation. A valid TTF font file called
 * `arial.tff` must also be present in the program's working directory.
**/

#include <Logger.h>
#include <Game.h>

#include <RenderDevice.h>
#include <FontRenderer.h>
#include <Matrix.h>

#include <DeviceParameters.h>
#include <GLWindow.h>

using namespace parcel;

/* Blank logic class. No logic is required for this basic example. */
class Logic : public ILogic
{
public:
    bool Update()
    {
        // Returns true to keep game constantly running
        return true;
    }
};

/* Handles graphical data and rendering tasks. */
class Graphics : public IGraphics
{
public:

    Graphics(windows::GLWindow* window) :
        log(),
        device(&log),
        font("arial.ttf", 12)
    {
        // Generates and sets orthographic matrix for 2D rendering
        maths::matrixf ortho;
        ortho.ToOrthoProjection(0.0, glw->GetWidth(), 0.0, glw->GetHeight(), -1.0, 1.0);
        device.SetProjectionMatrix(graphics::RENDERMODE_2D, ortho);
        // Sets up device's viewport size to be the same as the window
        device.SetViewportSize(maths::vector2i(glw->GetWidth(), glw->GetHeight()));
    }

    void Update()
    {
        device.StartRendering();
            // Clears view matrix, sets render mode to 2D and disables lighting
            device.SetViewMatrix(maths::matrixf::Identity(4));
            device.SetRenderMode(graphics::RENDERMODE_2D);
            device.GetLighting()->DisableLighting();
            // Now resets FontRenderer's start position and draws the text (with red
            font.SetHMargin(0.0f); font.SetVMargin(0.0f);
            font << graphics::colourf(1.0f, 0.0f, 0.0f, 1.0f) << "Hello world!";
        device.EndRendering();
    }

private:
    debug::Logger log;
    graphics::RenderDevice device;
    graphics::FontRenderer font;
};

/* Windows Entry Point */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    // Creates the window and OpenGL context
    windows::DeviceParameters params = { true, 32, 0, 16, 0 };
    windows::GLWindow* wind = new GLWindow(hInstance, "ParcelEngine",
        "Parcel Game Engine - Example Application", 512, 512,
        32, false, params);
    wind->Show(nCmdShow);

    // Sets up the logic and graphic subsystems
    Graphics* tg = new Graphics(wind);
    Logic* tl = new Logic();

    // Now adds it all to the game structure and starts the game
    game::Game game(wind, tl, tg);
    game.Start();
}

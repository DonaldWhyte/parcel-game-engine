#include "Matrix.h"
#include "Vector.h"
#include "Logger.h"
#include "Exceptions.h"
#include "Colour.h"
#include "Material.h"
#include "Skin.h"
#include "SkinManager.h"
#include "Image.h"
#include "OpenGLDevice.h"
#include "RenderDevice.h"
#include "Sound.h"
#include "GLWindow.h"
#include "Font.h"
#include "Util.h"
#include "DeviceParameters.h"
#include "Quaternion.h"
#include "ARenderer.h"
#include "VBORenderer.h"
#include "FixedFunctionLighting.h"
#include "Primitives.h"
#include "IndexedVBORenderer.h"
#include "Shader.h"
#include "Program.h"
#include "Quaternion.h"
#include "FontRenderer.h"
#include "Game.h"
#include "Camera.h"
#include "Test.h"
#include <iostream>

using namespace std;
using namespace parcel::debug;
using namespace parcel::maths;
using namespace parcel::general;
using namespace parcel::graphics;
using namespace parcel::sound;
using namespace parcel::game;

// Entry point for application.
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    /* Parcel skeleton test. */
    DeviceParameters params = { true, 32, 0, 16, 0 };
    GLWindow* wind = new GLWindow(hInstance, "ParcelEngine",
        "Test", 512, 512, 32, false, params);
    wind->Show(nCmdShow);
    TestGraphics* tg = new TestGraphics(wind);
    TestLogic* tl = new TestLogic(tg);
    Game game(wind, tl, tg);
    game.Start();

//    /*--Game Setup--*/
//    // The logger, manages file streams for logging application status
//    Logger log;
//
//    // The window of the game
//    DeviceParameters parameters = { true, 32, 0, 16, 0 };
//    GLWindow window(hInstance, "ParcelEngine",
//        //"Swishy Window!!", 2568, 2056, 32, false,
//        "Swishy Window!!", 640, 480, 32, false,
//        parameters);
//    window.Show(nCmdShow);
//
//    // Objects used to get input state
//    Keyboard* keyboard = window.GetInput()->GetKeyboard();
//    Mouse* mouse = window.GetInput()->GetMouse();
//
//    // The render device is created, this handles the global OpenGL state
//    RenderDevice device(&window, &log);
//
//    // This variables are used for the game loop
//    DWORD nextGameTick = 0; nextGameTick = GetTickCount();
//    unsigned int loops;
//
//    // NOTE: May add the following three variables to a header
//    // The amount of CPU ticks per second
//    static const unsigned int ticksPerSecond = 50;
//    // How many ticks to skipafter every logic update
//    static const unsigned int skipTicks = (1000 / ticksPerSecond);
//    // The maximum amount of frames to skip
//    static const unsigned int maxFrameSkip = 10;
//
//    /*--End of game setup--*/
//
//    /*--Initialises test stuff--*/
//
//    // Lighting stuff
//    FixedFunctionLighting* lighting = dynamic_cast<FixedFunctionLighting*>(device.GetLighting());
//    unsigned int lightID1, lightID2, lightID3;
//    lightID1 = lighting->AddLight(new TestLight(colourf(1.0f, 1.0f, 1.0f, 1.0f), vector3f(-0.5f, 0.0f, -1.0f)));
//    lightID2 = lighting->AddLight(new TestLight2(colourf(0.0f, 1.0f, 0.0f, 1.0f), vector3f(0.0f, 0.0f, 0.0f)));
//    lightID3 = lighting->AddLight(new TestLight3(colourf(0.0f, 0.5f, 0.5f, 1.0f), vector3f(20.0f, 0.0f, 20.0f), vector3f(0.0f, 0.0f, -1.0f)));
//    lighting->SetGlobalAmbientColour(colourf(0.3f, 0.3f, 0.7f, 1.0f));
//    lighting->SetGenerateNormals(true);
//
//    // Skin stuff
//    SkinManager* skinManager = device.GetSkinManager();
//    std::string skinID = "chacha", skinID2 = "chacha2";
//    skinManager->AddSkin(skinID, presetcolours::Red, presetcolours::Blue,
//        presetcolours::White, presetcolours::Blue, 10.0f);
//    skinManager->AddTextureToSkin(skinID, "chachatex", "yay.tga", 1.0f, true);
//    skinManager->AddSkin(skinID2, presetcolours::Black, presetcolours::Black,
//        presetcolours::Black, presetcolours::Black, 0.0f);
//
//    // NEW Font stuff
//    FontRenderer ffont("heygorgeous.ttf", 16);
//
//    // Camera test
//    cameraf camera;
//    camera.SetBehavior(CAMERA_BEHAVIOR_FIRST_PERSON);
//    camera.SetPosition(vector3f(0.0f, 0.0f, 3000.0));
//    camera.Perspective(65.0, (600.0 / 600.0), 1.0, 6000.0);
//    // Used to create orhtographic projection
//    matrixf orth;
//    orth.ToOrthoProjection(0.0, window.GetWidth(), 0.0, window.GetHeight(), -1.0, 1.0);
//    device.SetProjectionMatrix(RENDERMODE_2D, orth);
//    device.SetProjectionMatrix(RENDERMODE_3D, camera.GetProjectionMatrix());
//
//    // Render system stuff
//    VBORenderer renderer(&device, &log, true);
//
//    Test* t = new Test(skinID);
//    Test2* t2 = new Test2();
//    Test3* t3 = new Test3(skinID);
//
//    TestGroup* group = new TestGroup();
//    group->AddRenderable(t);
//    group->AddRenderable(t2);
//    group->AddRenderable(t3);
//
//    TestGroup2* group2 = new TestGroup2();
//    group2->AddRenderable(t);
//    group2->AddRenderable(t2);
//    group2->AddRenderable(t3);
//
//    renderer.AddRenderable(group);
//    renderer.AddRenderable(group2);
//
//    // Indexed render system stuff and model tests
//    DMFModelLoader model(skinManager); model.WriteToFile(model, "square.dmf");
//    TestModelIndex* mod = new TestModelIndex("test.dmf", skinManager, 100.0f);
//    IndexedVBORenderer indexRenderer(&device, &log, true);
//    indexRenderer.AddRenderable(mod);
//
//    // Misc. stuff like FPS and mouse position
//    float fps = 0.0f;
//    float oldTickCount = 0.0f;
//    int mouseX, mouseY;
//
//    // Shader stuff
//    Program program;
//    program.AddShader(new Shader(SHADERTYPE_VERTEX, "test.vert"),
//        "TestVertex", true);
//    program.AddShader(new Shader(SHADERTYPE_FRAGMENT, "test.frag"), "TestFragment", true);
//    Shader* vert = program.GetShader("TestVertex");
//    Shader* frag = program.GetShader("TestFragment");
//
//    vert->Compile(); vert->Attach();
//    frag->Compile(); frag->Attach();
//
//    program.Link();
//    program.Enable();
//
//    /*--End of test code--*/
//
//    /* Main loop of the program. Uses Constant Game Speed with Maximum FPS model. */
//    while (window.Update())
//    {
//            /* Game code. */
//            loops = 0; // Resets the loop counter for this next frame
//
//            // Updates 25 times regardless of render speed
//            while((GetTickCount() > nextGameTick) && (loops < maxFrameSkip))
//            {
//                keyboard->Update();
//                mouse->Update();
//
//                // If escape was pressed, close the window
//                if (keyboard->KeyDown(KEY_ESCAPE))
//                    window.Close();
//                // Camera position controls
//                if (keyboard->KeyPress(KEY_DOWN)) camera.RotateSmoothly(0.0f, 3.0f, 0.0f);
//                if (keyboard->KeyPress(KEY_UP)) camera.RotateSmoothly(0.0f, -3.0f, 0.0f);
//                if (keyboard->KeyPress(KEY_RIGHT)) camera.RotateSmoothly(3.0f, 0.0f, 0.0f);
//                if (keyboard->KeyPress(KEY_LEFT)) camera.RotateSmoothly(-3.0f, 0.0f, 0.0f);
//                if (keyboard->KeyPress(KEY_W)) camera.Move(camera.GetZAxis(), vector3f(10.0f, 10.0f, 10.0f));
//                if (keyboard->KeyPress(KEY_A)) camera.Move(camera.GetXAxis(), vector3f(-10.0f, -10.0f, -10.0f));
//                if (keyboard->KeyPress(KEY_S)) camera.Move(camera.GetZAxis(), vector3f(-10.0f, -10.0f, -10.0f));
//                if (keyboard->KeyPress(KEY_D)) camera.Move(camera.GetXAxis(), vector3f(10.0f, 10.0f, 10.0f));
//                if (keyboard->KeyPress(KEY_Q)) camera.Move(camera.GetYAxis(), vector3f(10.0f, 10.0f, 10.0f));
//                if (keyboard->KeyPress(KEY_E)) camera.Move(camera.GetYAxis(), vector3f(-10.0f, -10.0f, -10.0f));
//                // Lighting keyboard
//                if (keyboard->KeyDown(KEY_1))
//                    lighting->RemoveLight(lightID1);
//                if (keyboard->KeyDown(KEY_2))
//                    lighting->RemoveLight(lightID2);
//                if (keyboard->KeyDown(KEY_3))
//                    lighting->RemoveLight(lightID3);
//
//                // Gets mouse position
//                vector2f deltaRotation = mouse->GetRelativePosition();
//                // Uses mouse to determine how much to rotate the camera
//                camera.RotateSmoothly(-deltaRotation.x, -deltaRotation.y, 0.0f);
//                mouse->SmoothMouse(true);
//                // Resets mouse position
//                mouse->SetPosition(vector2i((window.GetWidth()) / 2, (window.GetHeight())));
//
//                // Increases loop data
//                nextGameTick += skipTicks;
//                ++loops;
//            }
//
//            // Calculates FPS
//            float newTickCount = GetTickCount();
//            float delta = newTickCount - oldTickCount; // Calculates time elapsed since last frame
//            fps = delta / 1000.0f; // Uses that to get the fps
//            oldTickCount = newTickCount;
//
//            /* End of game code. */
//
//            /* Render code */
//            device.StartRendering();
//
//            // Makes sure lighting is enabled
//            lighting->EnableLighting();
//            // Also enables shaders for 3D scene
//            program.Disable();
//
//            // Starts rendering 3D scene
//            device.SetViewMatrix(camera.GetViewMatrix());
//            device.SetRenderMode(RENDERMODE_3D);
//
//            // Updates renderer and then draws everything stored in it
//            try
//            {
//                static bool chaa1 = true, chaa2 = true;
//
//                if (chaa1) renderer.Update();
//                renderer.Render();
//                if (chaa2) indexRenderer.Update();
//                indexRenderer.Render();
//
//                chaa1 = chaa2 = false;
//            }
//            catch (Exception& ex) { ex.PrintMessage(); }
//            catch (...) { std::cout << "VBORenderer - Failed for unknown reason!\n"; }
//
//            // Switches to 2D to render the text
//            device.SetViewMatrix(maths::matrixf::Identity(4));
//            device.SetRenderMode(RENDERMODE_2D);
//            // Disables lighting
//            lighting->DisableLighting();
//            // Disables shaders in order to render text correctly
//            program.Disable();
//            // Draws text
//            ffont.SetHMargin(0.0f);
//            ffont.SetVMargin(80.0f);
//            vector3f pos = camera.GetPosition();
//            ffont << "Camera Position: " << pos.x << ", " << pos.y << ", " << pos.z << "\n"
//                << "Mouse Position: " << mouseX << ", " << mouseY << "\n";
//
//            device.EndRendering();
//            /* End of render code. */
//    }

    /* Window and OpenGL device context are destroyed automatically by their respective classes. */
    /* Same goes for RenderDevice, SkinManager and renderers. */

    return 0;
}

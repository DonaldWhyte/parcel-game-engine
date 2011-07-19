/*
 * File:   Test.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on March 21, 2009, 10:03 AM
 */

// DO NOT INCLUDE IN ENGINE RELEASES!!!

#ifndef TEST_H
#define TEST_H

#include <iomanip>
#include <GLee.h>
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
#include "DMFModelLoader.h"
#include "GameInterfaces.h"
#include "Camera.h"
#include "SpriteRenderer.h"
#include "LuaManager.h"
#include "Animator.h"
#include <windows.h>
#include <iostream>
#include <gl/gl.h>

using namespace std;
using namespace parcel;
using namespace parcel::debug;
using namespace parcel::maths;
using namespace parcel::general;
using namespace parcel::graphics;
using namespace parcel::sound;
using namespace parcel::game;
using namespace parcel::lua;
using namespace parcel::windows;

class Test : public IRenderable, public IGeometry, public ISkinned
{
public:
    vector<Vertex> v;
    std::string skin;

    Test(const std::string& skinID) : skin(skinID)
    {
        v.push_back(
            Vertex(vector3f(200.0f, 200.0f, 5.0f), vector2f(0.0f, 0.0f), vector3f(0.0f, 0.0f, 0.0f)));

        v.push_back(
            Vertex(vector3f(300.0f, 200.0f, 5.0f), vector2f(1.0f, 0.0f), vector3f(0.0f, 0.0f, 0.0f)));

        v.push_back(
            Vertex(vector3f(200.0f, 300.0f, 5.0f), vector2f(0.0f, 1.0f), vector3f(0.0f, 0.0f, 0.0f)));
    }
    virtual ~Test() {}

    const vector<Vertex>& GetVertices() { return v; }

    bool IsStatic() { return true; }

    int GetMemorySize() { return sizeof(Vertex) * 3; }

    std::string& GetSkinID() { return skin; }

    int GetPrimitiveType() { return PRIMITIVETYPE_TRIANGLE; }

};

class Test2 : public IRenderable, public IGeometry, public IMatrix
{
public:
    vector<Vertex> v;
    matrixf matrix;

    Test2()
    {
        v.push_back(
            Vertex(vector3f(-1.0f, -1.0f, 1.0f), vector2f(0.0f, 0.0f), vector3f(0.0f, 0.0f, 1.0f)));
        v.push_back(
            Vertex(vector3f(1.0f, -1.0f, 1.0f), vector2f(1.0f, 0.0f), vector3f(0.0f, 0.0f, 1.0f)));
        v.push_back(
            Vertex(vector3f(1.0f, 1.0f, 1.0f), vector2f(1.0f, 1.0f), vector3f(0.0f, 0.0f, 1.0f)));
        v.push_back(
            Vertex(vector3f(-1.0f, 1.0f, 1.0f), vector2f(0.0f, 1.0f), vector3f(0.0f, 0.0f, 1.0f)));

        v.push_back(
            Vertex(vector3f(-1.0f, -1.0f, -1.0f), vector2f(1.0f, 0.0f), vector3f(0.0f, 0.0f, -1.0f)));
        v.push_back(
            Vertex(vector3f(-1.0f, 1.0f, -1.0f), vector2f(1.0f, 1.0f), vector3f(0.0f, 0.0f, -1.0f)));
        v.push_back(
            Vertex(vector3f(1.0f, 1.0f, -1.0f), vector2f(0.0f, 1.0f), vector3f(0.0f, 0.0f, -1.0f)));
        v.push_back(
            Vertex(vector3f(1.0f, -1.0f, -1.0f), vector2f(1.0f, 0.0f), vector3f(0.0f, 0.0f, -1.0f)));

        v.push_back(
            Vertex(vector3f(-1.0f, 1.0f, -1.0f), vector2f(0.0f, 1.0f), vector3f(0.0f, 1.0f, 0.0f)));
        v.push_back(
            Vertex(vector3f(-1.0f, 1.0f, 1.0f), vector2f(0.0f, 0.0f), vector3f(0.0f, 1.0f, 0.0f)));
        v.push_back(
            Vertex(vector3f(1.0f, 1.0f, 1.0f), vector2f(1.0f, 0.0f), vector3f(0.0f, 1.0f, 0.0f)));
        v.push_back(
            Vertex(vector3f(1.0f, 1.0f, -1.0f), vector2f(1.0f, 1.0f), vector3f(0.0f, 1.0f, 0.0f)));

        v.push_back(
            Vertex(vector3f(-1.0f, -1.0f, -1.0f), vector2f(1.0f, 1.0f), vector3f(0.0f, -1.0f, 0.0f)));
        v.push_back(
            Vertex(vector3f(1.0f, -1.0f, -1.0f), vector2f(0.0f, 1.0f), vector3f(0.0f, -1.0f, 0.0f)));
        v.push_back(
            Vertex(vector3f(1.0f, -1.0f, 1.0f), vector2f(0.0f, 0.0f), vector3f(0.0f, -1.0f, 0.0f)));
        v.push_back(
            Vertex(vector3f(-1.0f, -1.0f, 1.0f), vector2f(1.0f, 1.0f), vector3f(0.0f, -1.0f, 0.0f)));

        v.push_back(
            Vertex(vector3f(1.0f, -1.0f, -1.0f), vector2f(1.0f, 0.0f), vector3f(1.0f, 0.0f, 0.0f)));
        v.push_back(
            Vertex(vector3f(1.0f, 1.0f, -1.0f), vector2f(1.0f, 1.0f), vector3f(1.0f, 0.0f, 0.0f)));
        v.push_back(
            Vertex(vector3f(1.0f, 1.0f, 1.0f), vector2f(0.0f, 1.0f), vector3f(1.0f, 0.0f, 0.0f)));
        v.push_back(
            Vertex(vector3f(1.0f, -1.0f, 1.0f), vector2f(0.0f, 0.0f), vector3f(1.0f, 0.0f, 0.0f)));

        v.push_back(
            Vertex(vector3f(-1.0f, -1.0f, -1.0f), vector2f(0.0f, 0.0f), vector3f(-1.0f, 0.0f, 0.0f)));
        v.push_back(
            Vertex(vector3f(-1.0f, -1.0f, 1.0f), vector2f(1.0f, 0.0f), vector3f(-1.0f, 0.0f, 0.0f)));
        v.push_back(
            Vertex(vector3f(-1.0f, 1.0f, 1.0f), vector2f(1.0f, 1.0f), vector3f(-1.0f, 0.0f, 0.0f)));
        v.push_back(
            Vertex(vector3f(-1.0f, 1.0f, -1.0f), vector2f(0.0f, 1.0f), vector3f(-1.0f, 0.0f, 0.0f)));

        float data[16] =
        {
            200, 0, 0, 0,
            0, 200, 0, 0,
            0, 0, 200, 0,
            -400, 0, 0, 1
        };

        matrix = matrixf(4, 4, data);
    }
    virtual ~Test2() {}

    virtual const vector<Vertex>& GetVertices()
    {
        return v;
    }

    virtual bool IsStatic() { return true; }

    virtual int GetMemorySize() { return sizeof(Vertex) * 24; }

    virtual void SetMatrix(const matrixf& m) { matrix = m; }

    virtual const matrixf& GetMatrix() { return matrix; }

    virtual void GetMatrixAsArray(float* a)
    {
        matrix.ToArray(a);
    }

    int GetPrimitiveType() { return PRIMITIVETYPE_QUAD; }

};

class Test3 : public IRenderable, public IGeometry, public ISkinned, public IMatrix
{
public:
    vector<Vertex> v;
    matrixf matrix;
    std::string skin;

    Test3(const std::string& skinID) : skin(skinID)
    {
        v.push_back(
            Vertex(vector3f(0.0f, 0.0f, 50.0f), vector2f(0.0f, 0.0f), vector3f(0.0f, 0.0f, 0.0f)));

        v.push_back(
            Vertex(vector3f(100.0f, 0.0f, 50.0f), vector2f(0.5f, 0.0f), vector3f(0.0f, 0.0f, 0.0f)));

        v.push_back(
            Vertex(vector3f(0.0f, 100.0f, 50.0f), vector2f(0.0f, 0.5f), vector3f(0.0f, 0.0f, 0.0f)));

        v.push_back(
            Vertex(vector3f(100.0f, 100.0f, 50.0f), vector2f(0.0f, 0.0f), vector3f(0.0f, 0.0f, 0.0f)));

        v.push_back(
            Vertex(vector3f(150.0f, 100.0f, 50.0f), vector2f(1.0f, 0.0f), vector3f(0.0f, 0.0f, 0.0f)));

        v.push_back(
            Vertex(vector3f(100.0f, 150.0f, 50.0f), vector2f(0.0f, 1.0f), vector3f(0.0f, 0.0f, 0.0f)));

        float data[16] =
        {
            2, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            400, 0, 0, 1
        };

        matrix = matrixf(4, 4, data);
    }
    virtual ~Test3() {}

    const vector<Vertex>& GetVertices() { return v; }

    bool IsStatic() { return true; }

    int GetMemorySize() { return sizeof(Vertex) * 6; }

    const std::string& GetSkinID() { return skin; }

    virtual void SetMatrix(const matrixf& m) { matrix = m; }

    virtual const matrixf& GetMatrix() { return matrix; }

    virtual void GetMatrixAsArray(float* a)
    {
        matrix.ToArray(a);
    }

    int GetPrimitiveType() { return PRIMITIVETYPE_TRIANGLE; }

};

class TestGroup : public IRenderable, public IGroupRenderable, public IMatrix
{
public:
    vector<IRenderable*> renderables;
    matrixf matrix;

    TestGroup()
    {
        float data[16] =
        {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 250, 0, 1
        };

        matrix = matrixf(4, 4, data);
    }
    virtual ~TestGroup()
    {
                for (unsigned int i = 0; (i < renderables.size()); i++)
                {
                    delete renderables[i];
                    renderables[i] = NULL;
                }
    }

        void AddRenderable(IRenderable* renderable)
        {
            renderables.push_back(renderable);
        }

    bool IsStatic() { return true; }

    int GetMemorySize()
    {
        int size = 0;

                for (unsigned int i = 0; (i < renderables.size()); i++)
                {
                    size += renderables[i]->GetMemorySize();
                }

        return size;
    }

    unsigned int GetAmountOfRenderables()
    {
        return renderables.size();
    }

    IRenderable* GetRenderable(unsigned int i)
    {
        return renderables[i];
    }

    virtual void SetMatrix(const matrixf& m) { matrix = m; }

    virtual const matrixf& GetMatrix() { return matrix; }

    virtual void GetMatrixAsArray(float* a)
    {
        matrix.ToArray(a);
    }

};
class TestGroup2 : public IRenderable, public IGroupRenderable, public IMatrix
{
public:
    vector<IRenderable*> renderables;
    matrixf matrix;

    TestGroup2()
    {
        float data[16] =
        {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, -250, 0, 1
        };

        matrix = matrixf(4, 4, data);
    }
    virtual ~TestGroup2()
    {
                for (unsigned int i = 0; (i < renderables.size()); i++)
                {
                    delete renderables[i];
                    renderables[i] = NULL;
                }
    }

        void AddRenderable(IRenderable* renderable)
        {
            renderables.push_back(renderable);
        }

    bool IsStatic() { return true; }

    int GetMemorySize()
    {
        int size = 0;

                for (unsigned int i = 0; (i < renderables.size()); i++)
                {
                    size += renderables[i]->GetMemorySize();
                }

        return size;
    }

    unsigned int GetAmountOfRenderables()
    {
        return renderables.size();
    }

    IRenderable* GetRenderable(unsigned int i)
    {
        return renderables[i];
    }

    virtual void SetMatrix(const matrixf& m) { matrix = m; }

    virtual const matrixf& GetMatrix() { return matrix; }

    virtual void GetMatrixAsArray(float* a)
    {
        matrix.ToArray(a);
    }

};

class TestLight : public ALight
{
public:
    colourf colour;
    vector3f position;

    TestLight(const colourf& col, const vector3f& pos) : colour(col), position(pos)
    {
    }


    virtual bool IsEnabled()
    {
        return true;
    }

    virtual LightType GetType()
    {
        return LIGHTTYPE_DIRECTION;
    }

    virtual colourf GetAmbientColour()
    {
        return colour;
    }

    virtual colourf GetDiffuseColour()
    {
        return colour;
    }

    virtual colourf GetSpecularColour()
    {
        return presetcolours::Green;
    }

    virtual vector3f GetDirection()
    {
        return position;
    }
};
class TestLight2 : public ALight
{
public:
    colourf colour;
    vector3f position;

    TestLight2(const colourf& col, const vector3f& pos) : colour(col), position(pos)
    {
    }


    virtual bool IsEnabled()
    {
        return true;
    }

    virtual LightType GetType()
    {
        return LIGHTTYPE_POSITION;
    }

    virtual colourf GetAmbientColour()
    {
        return colour;
    }

    virtual colourf GetDiffuseColour()
    {
        return colour;
    }

    virtual colourf GetSpecularColour()
    {
        return presetcolours::Green;
    }

    virtual vector3f GetPosition()
    {
        return position;
    }
    virtual float GetConstantAttenuation()
    {
        return 1.0f;
    }

    virtual float GetLinearAttenuation()
    {
        return 0.0f;
    }

    virtual float GetQuadraticAttenuation()
    {
        return 0.0f;
    }
};
class TestLight3 : public ALight
{
public:
    colourf colour;
    vector3f position, direction;

    TestLight3(const colourf& col, const vector3f& pos, const vector3f& dir) : colour(col), position(pos), direction(dir)
    {
    }


    virtual bool IsEnabled()
    {
        return true;
    }

    virtual LightType GetType()
    {
        return LIGHTTYPE_SPOTLIGHT;
    }

    virtual colourf GetAmbientColour()
    {
        return colour;
    }

    virtual colourf GetDiffuseColour()
    {
        return colour;
    }

    virtual colourf GetSpecularColour()
    {
        return presetcolours::Green;
    }

    virtual vector3f GetPosition()
    {
        return position;
    }

    virtual vector3f GetDirection()
    {
        return direction;
    }

    virtual float GetSpotlightCutoff()
    {
        return 90.0f;
    }

    virtual float GetSpotlightFocus()
    {
        return 10.0f;
    }

    virtual float GetConstantAttenuation()
    {
        return 1.0f;
    }

    virtual float GetLinearAttenuation()
    {
        return 0.0f;
    }

    virtual float GetQuadraticAttenuation()
    {
        return 0.0f;
    }
};

class TestModel : public IRenderable, public IGeometry, public ISkinned, public IMatrix
{
public:
    matrixf matrix;
    DMFModelLoader model;

    TestModel(const std::string& modelFilename, SkinManager* skinManager) : model(skinManager)
    {
        // Loads the model
        model.LoadFromFile(modelFilename);

        float data[16] =
        {
            600, 0, 0, 0,
            0, 600, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };

        matrix = matrixf(4, 4, data);
    }
    virtual ~TestModel() {}

    const vector<Vertex>& GetVertices() { return model.GetVertices(); }

    bool IsStatic() { return true; }

    int GetMemorySize() { return sizeof(Vertex) * model.GetVertices().size(); }

    const std::string& GetSkinID() { return model.GetSkins()[0]; }

    virtual void SetMatrix(const matrixf& m) { matrix = m; }

    virtual const matrixf& GetMatrix() { return matrix; }

    virtual void GetMatrixAsArray(float* a)
    {
        matrix.ToArray(a);
    }

    int GetPrimitiveType() { return PRIMITIVETYPE_QUAD; }

};

class TestModelIndex : public IRenderable, public IIndexedGeometry, public ISkinned, public IMatrix
{
public:
    matrixf matrix;
    DMFModelLoader model;

    TestModelIndex(const std::string& modelFilename, SkinManager* skinManager,
        float x) : model(skinManager)
    {
        // Loads the model
        model.LoadFromFile(modelFilename);

        float data[16] =
        {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, x, 1
        };

        matrix = matrixf(4, 4, data);
    }
    virtual ~TestModelIndex() {}

    const vector<Vertex>& GetVertices() { return model.GetVertices(); }

    const vector<Triangle>& GetFaces() { return model.GetTriangles(); }

    int GetTriangleMemorySize() { return (sizeof(Triangle) * model.GetTriangles().size()); }

    bool IsStatic() { return true; }

    int GetMemorySize() { return sizeof(Vertex) * model.GetVertices().size(); }

    const std::string& GetSkinID() { return model.GetSkins()[0]; }

    virtual void SetMatrix(const matrixf& m) { matrix = m; }

    virtual const matrixf& GetMatrix() { return matrix; }

    virtual void GetMatrixAsArray(float* a)
    {
        matrix.ToArray(a);
    }

    int GetPrimitiveType() { return PRIMITIVETYPE_QUAD; }

};


class TestGraphics : public IGraphics
{


public:

    Logger log;
    RenderDevice device;
    IndexedVBORenderer vbo;
    SpriteRenderer spriteRenderer;
    cameraf camera;
    FontRenderer font;

    Animator animator;

    TestGraphics(GLWindow* glw) :
        log(), // Here to ensure that it gets constructed before the render device
        device(&log),
        vbo(&device, &log, true),
        spriteRenderer(&device, &log, true),
        font("heygorgeous.ttf", 12)
    {
        // Loads model into renderer
        TestModelIndex* mod = new TestModelIndex("Lasha.obj.dmf", device.GetSkinManager(), -300.0f);
        vbo.AddRenderable(mod);

        // Generates projection matrices
        maths::matrixf ortho;
        ortho.ToOrthoProjection(0.0, glw->GetWidth(), 0.0, glw->GetHeight(), -1.0, 1.0);
        camera.Perspective(65.0, (600.0 / 600.0), 1.0, 2000.0);
        // Sets projection matrices
        device.SetProjectionMatrix(RENDERMODE_2D, ortho);
        device.SetProjectionMatrix(RENDERMODE_3D, camera.GetProjectionMatrix());
        // Sets camera's properties
        camera.SetBehavior(CAMERA_BEHAVIOR_FIRST_PERSON);
        camera.SetPosition(vector3f(0.0f, 0.0f, 50.0f));
        // Sets device's viewport size]
        device.SetViewportSize(vector2i(glw->GetWidth(), glw->GetHeight()));
    }

    virtual ~TestGraphics() {}

    void Update()
    {
        device.StartRendering();
            // Draw 2D text and HUD
            device.SetViewMatrix(maths::matrixf::Identity(4));
            device.SetRenderMode(RENDERMODE_2D);
            device.GetLighting()->DisableLighting();
            spriteRenderer.Update();
            spriteRenderer.Render();
            font.SetHMargin(0.0f); font.SetVMargin(0.0f);
            // Draws 3D scene, stuff like models
            device.SetViewMatrix(camera.GetViewMatrix());
            device.SetRenderMode(RENDERMODE_3D);
            device.GetLighting()->EnableLighting();
            vbo.Update();
            vbo.Render();
        device.EndRendering();
    }

};

class TestLogic : public ILogic
{

private:

    TestGraphics* gfx;
    LuaManager scripts;
    int i;

public:

    TestLogic(TestGraphics* graphics) : gfx(graphics), i(0)
    {
        // LUA SCRIPT TEST BELOW
//        try
//        {
//            // Executes test script
//            scripts.LoadAndExecuteScript("test.lua");
//            // Passes other values to the vm
//            scripts.SetFloatToTable("names", "setted", 82.0074f);
//            // Retrives values from the vm and prints them
//            cout << scripts.GetFloatFromTable("names", "setted") << endl;
//        }
//        catch (debug::Exception& ex)
//        {
//            ex.PrintMessage();
//        }
    }

    virtual ~TestLogic() {}

    bool Update()
    {
        if (++i > 10000)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
};

#endif

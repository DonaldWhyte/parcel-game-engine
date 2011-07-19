/*
 * File:   RenderDevice.cpp
 * Author: Donald "Datriot" Whyte
 *
 * Created on January 2, 2009, 3:15 PM
 */

#include <windows.h>
#include <GLee.h>
#include "ALighting.h"
#include "FixedFunctionLighting.h"
#include "MCommon.h"
#include "RenderDevice.h"

namespace parcel
{

using namespace maths;
using namespace debug;

namespace graphics
{

    const std::string RenderDevice::noSkin = "NOSKIN!";
    const std::string RenderDevice::noTexture = "NOTEXTURE!";

    RenderDevice::RenderDevice(Logger* log)
        :
        skinManager(SkinManager(log)), lighting(new FixedFunctionLighting(log, true, true)), // Managers
        logger(log), // Logger
        mode(RENDERMODE_3D), renderStarted(false), // State of device
        currentSkinID(noSkin), currentTextureID(noTexture), // Sets current skin and texture IDs to none
        viewportSize(512, 512)
    {
        // Starts the log for the render device
        logID = logger->StartLog("RenderDevice");
        logger->WriteTextAndNewLine(logID, "RenderDevice has been created.");

        // Initialises device and sets the default state of the device in 3D mode
        Initialise();
        Reset();
    }


    RenderDevice::~RenderDevice()
    {
        logger->WriteTextAndNewLine(logID, "RenderDevice has been destroyed.");

        // Releases any resources used by RenderDevice
        delete lighting;

        // Make null pointers
        logger = NULL;

        // All managers are cleaned up automatically in their destructors
        // OpenGL device context is also cleaned up automatically
    }




    void RenderDevice::Initialise()
    {
        // Sets default OpenGL state
        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Sets the clear colour and clears the buffers
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Sets world/view the matrices to identity
        worldMatrix = matrixf::Identity(4);
        viewMatrix = matrixf::Identity(4);
        // Sets default projections for the two render modes
        projectionMatrix2D = matrixf::Identity(4);
        projectionMatrix3D = matrixf::Identity(4);

        // Builds a viewport with dimensions of the device context
        BuildViewport();
    }


    void RenderDevice::SetRenderMode(RenderMode renderMode)
    {
        // Sets new render mode...
        mode = renderMode;
        // ...then resets the device's state
        Reset();
    }


    void RenderDevice::Reset()
    {
        float array[16]; // Temporarily holds matrix values

        // Loads the projection matrix for this render mode
        glMatrixMode(GL_PROJECTION);
        if (mode == RENDERMODE_2D) projectionMatrix2D.ToArray(array);
        if (mode == RENDERMODE_3D) projectionMatrix3D.ToArray(array);
        glLoadMatrixf(array);

        // Sets up the world (model) and view matrices
        glMatrixMode(GL_MODELVIEW); // Switches to the modelview stack
        // Loads the world matrix and then multiplies it with the view (camera) matrix
        worldMatrix.ToArray(array);
        glLoadMatrixf(array);
        viewMatrix.ToArray(array);
        glMultMatrixf(array);

        /* Enables/disables certain states depending on the render mode the device
         * is being swtiched to. */
        if (mode == RENDERMODE_2D)
        {
            if (glIsEnabled(GL_DEPTH_TEST)) glDisable(GL_DEPTH_TEST);
        }
        else if (mode == RENDERMODE_3D)
        {
            if (!glIsEnabled(GL_DEPTH_TEST)) glEnable(GL_DEPTH_TEST);
        }

        // Logs the event
        if (mode == RENDERMODE_2D)
        {
            logger->WriteTextAndNewLine(logID, "Render mode switched to 2D.");
        }
        else if (mode == RENDERMODE_3D)
        {
            logger->WriteTextAndNewLine(logID, "Render mode switched to 3D.");
        }
    }


    void RenderDevice::StartRendering()
    {
        // Starts rendering if has not started already
        if (!renderStarted) renderStarted = true;
        // Otherwise, log an error message and return
        else
        {
            logger->WriteTextAndNewLine(logID,
                "RenderDevice::StartRendering - Render pass has already started.");
            return;
        }


        // Also wipes texture and material clean
        ClearSkinAndTexture();
        // Updates the lighting
        lighting->Update();


        // Clears the colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }


    void RenderDevice::EndRendering()
    {
        // Stops rendering if has actually started
        if (renderStarted) renderStarted = false;
        // Otherwise, log an error message and return
        else
        {
            logger->WriteTextAndNewLine(logID,
                "RenderDevice::StartRendering - Render pass cannot end before being started.");
            return;
        }
    }


    void RenderDevice::SetActiveSkin(const std::string& skinID)
    {
        try
        {
            // Gets the needed skin from the skin manager
            Skin* skin = skinManager.GetSkin(skinID);
            // Now retrieves the material and list of textures
            Material* mat = skinManager.GetMaterial(skin->material);
            std::string* textures = NULL;
            textures = &skin->textures[0];
            // Sets the material of the skin
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat->ambient.values);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse.values);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular.values);
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat->emmisive.values);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->specularPower);
            // Binds the skin's primary texture if it exists
            if (textures[0] != skinNoTexture)
            {
                SetActiveTexture(textures[0]);
            }
            // Sets the current active skin and texture
            currentSkinID = skinID;
            currentTextureID = textures[0];
        }
        catch(debug::Exception& ex)
        {
            // Print error message carried by the thrown exception
            ex.PrintMessage();
        }
        catch (...) { }
    }


    void RenderDevice::SetActiveTexture(const std::string& textureID)
    {
        // Binds the texture to the currently active texture unit
        glBindTexture(GL_TEXTURE_2D, skinManager.GetTexture(textureID)->glID);
        // Also sets the texture's transparency
        SetTextureTransparency(textureID);

        currentTextureID = textureID;
    }
    void RenderDevice::SetActiveTextureWithColourMask(const std::string& textureID, const colourf& colour)
    {
        // Binds the texture to the currently active texture unit
        glBindTexture(GL_TEXTURE_2D, skinManager.GetTexture(textureID)->glID);
        // Also sets the texture's transparency with an additional colour mask
        SetTextureTransparencyWithColourMask(textureID, colour);

        currentTextureID = textureID;
    }


    void RenderDevice::SetTextureTransparency(const std::string& textureID)
    {
        // Gets the needed texture
        Texture* tex = skinManager.GetTexture(textureID);
        // If texture was not found, return
        if (!tex) return;

        // Applies transparency by directly modifying the source colour
        glColor4f(1.0f, 1.0f, 1.0f, tex->transparency);
    }
    void RenderDevice::SetTextureTransparencyWithColourMask(const std::string& textureID, const colourf& colour)
    {
        Texture* tex = skinManager.GetTexture(textureID);
        if (!tex) return;
        /* Applies transparency AND colour mask by directly modifying the source colour.
         * The colour mask can include alpha transparency, the alpha component of
         * the colour is mutliplied by the texture's preset transprency. */
        glColor4f(colour.r, colour.g, colour.b, (colour.a * tex->transparency));
    }


    void RenderDevice::ClearSkinAndTexture()
    {
        // Sets colour to default just in case a colour was set that may affect other textures
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

        // Sets all material properties to white if lighting is enabled
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, presetcolours::White.values);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, presetcolours::White.values);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, presetcolours::White.values);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, presetcolours::White.values);

        // Unbinds texture in current texture unit
        glBindTexture(GL_TEXTURE_2D, 0);

        // Sets current active skin and texture to nothing
        currentSkinID = currentTextureID = -1;
    }


    bool RenderDevice::IsRenderMode(const RenderMode& rMode)
    {
        if (rMode == mode) return true;
        else return false;
    }


    void RenderDevice::SetViewMatrix(const matrixf&mat)
    {
        viewMatrix = mat;
    }


    void RenderDevice::SetProjectionMatrix(RenderMode renderMode,
        const maths::matrixf& newProjMatrix)
    {
        // Sets the appropriate projection for the specified render mode
        if (renderMode == RENDERMODE_2D)
        {
            projectionMatrix2D = newProjMatrix;
        }
        else if (renderMode == RENDERMODE_3D)
        {
            projectionMatrix3D = newProjMatrix;
        }
    }


    void RenderDevice::SetViewportSize(const maths::vector2i& newSize)
    {
        // Assigns new viewport size...
        viewportSize = newSize;
        // ...then rebuilds the viewport
        BuildViewport();
    }

    const maths::matrixf& RenderDevice::GetProjectionMatrix(RenderMode renderMode)
    {
        if (renderMode == RENDERMODE_2D)
        {
            return projectionMatrix2D;
        }
        else if (renderMode == RENDERMODE_3D)
        {
            return projectionMatrix3D;
        }
        else
        {
            return maths::matrixf::Identity(4);
        }
    }

    void RenderDevice::BuildViewport()
    {
        // Changes viewport to the desired size
        glViewport(0, 0, viewportSize.x, viewportSize.y);
    }

}

}

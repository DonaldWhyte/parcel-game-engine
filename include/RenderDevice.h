/*
 * File:   RenderDevice.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on January 2, 2009, 3:15 PM
 */

#ifndef RENDERDEVICE_H
#define RENDERDEVICE_H

#include "Matrix.h"
#include "Vector.h"

#include "SkinManager.h"
#include "ALighting.h"

#include "GLWindow.h"
#include "RenderMode.h"

/* TODO: Create a class that is held by the RenderDevice which handles activating textures,
 * multitextures, texture blending, combining, interpolating and so on. This class will NOT
 * load textures. That is the SkinManager's job. SkinManager may also be renamed to something
 * more generic in the future. */

namespace parcel
{

namespace graphics
{


    /* This class provides the many of the engine's rendering capabilities and usually holds
     * most of the other managers, such as material and texture management, 3D meshes,
     * lighting and so on. It encapsulates many OpenGL functions. */
    class RenderDevice
    {


    private:

        // The two matrices used with the modelview matrix stack
        maths::matrixf worldMatrix; // Matrix for world space
        maths::matrixf viewMatrix; // View (camera) matrix
        maths::matrixf projectionMatrix2D; // Projection matrix for 2D portion of scene
        maths::matrixf projectionMatrix3D; // Projection matrix for 3D portion of scene

        maths::vector2i viewportSize; // Size of the viewport

        SkinManager skinManager; // Manages all skins, textures, materials and all that other stuff
        ALighting* lighting; // Holds a pointer to the manager that handles the scene's lighting

        debug::Logger* logger; // Used to log render device's state
        unsigned int logID; // The ID of the log that was created for this class


        /* State variables. */

        RenderMode mode; // The current mode of the render device, set to 3D upon creation
        bool renderStarted; // Determines if rendering has started yet and is allowed

        /* Constants that determine the ID currentSkin/TextureID contain if NO texture or
         * skin is active. Implementing in .cpp file. */
        static const std::string noSkin;
        static const std::string noTexture;
        std::string currentSkinID; // ID of currently active skin. "NOSKIN!" = NoSkin
        std::string currentTextureID; // ID of currently active texture. "NOTEXTURE!" = NoTexture



        /* Private Methods. */


        /* Since it uses a double buffered mode, this function swaps buffers,
         * presenting rendered content to screen. */
        void SwapDeviceBuffers();

        /* This function resets the modelview and projection matrices to switch to 3D/2D.
         * This can be done in the middle of a rendering pass, so you can start with 3D,
         * render the scene, then swap to 2D to render the HUD.
         * It is good practice for this to be called before rendering in 3D/2D, while it is not
         * always neccesery, it makes sure that the state has been reset correctly. */
        void Reset();

        /* Builds the viewport with dimensions stored in 'viewportSize'. */
        void BuildViewport();

        /* This method sets the transparency by using a texture's transparency value.
         * Simply returns if it fails to find the texture. */
        void SetTextureTransparency(const std::string& textureID);
        /* Applies transparency AND the RGB values of the given colour. */
        void SetTextureTransparencyWithColourMask(const std::string& textureID, const colourf& colour);


    public:

        /* Constructor and Destructor. */
        RenderDevice(debug::Logger* log); // Initialises RenderDevice's state
        ~RenderDevice(); // Releases any resources that may have been used for the device

        void DeleteAll(); // Deletes everything created by managers and such



        /* State related methods. */


        /* Initialises the viewport, clear colour and state. Is made public because it can
         * be called at any time everything needs to be cleared. */
        void Initialise();

        /* Changes the render mode to the one specified. It calls Reset() and loads up
         * the matrices and apprporiate projection of the render mode.
         * NOTE: If you set the mode to 3D when it is already 3D, it will reset the state
         * again with fresh new matrices and such. */
        void SetRenderMode(RenderMode renderMode);

        /* These two methods start and end the device's rendering pass respectively. */
        void StartRendering();
        void EndRendering();



        /* Model, view and projection matrix/vector operations. */


        /* Creates a look-at view matrix that will be used with the device. */
        void SetViewMatrix(const maths::matrixf& mat);
        /* Used to set the given mode's (renderMode) projection's properties */
        void SetProjectionMatrix(RenderMode renderMode, const maths::matrixf& newProjMatrix);
        /* Sets viewport size. */
        void SetViewportSize(const maths::vector2i& newSize);
        /* Getting view and projection matrices. */
        const maths::matrixf& GetViewMatrix() { return viewMatrix; }
        const maths::matrixf& GetProjectionMatrix(RenderMode renderMode);
        const maths::vector2i& GetViewportSize() { return viewportSize; }



        /* Skin management and rendering. */


        /* Sets active mateirals and textures to the ones used by the skin with the ID given.
         * If it cannot find the skin/texture/material, then it throws an exception.*/
        void SetActiveSkin(const std::string& skinID);

        /* Sets the currently active texture to the current texture unit. Keeps the set materials
         * as they are. If you need to set a texture and material, use SetActiveSkin(). */
        void SetActiveTexture(const std::string& textureID);
        /* Sets the active texture, but with the specified colour mask. */
        void SetActiveTextureWithColourMask(const std::string& textureID, const colourf& colour);

        /* Clears the texture and material state. */
        void ClearSkinAndTexture();



        /* Accessor methods. */


        /* Checks if the current render mode is the one specified. */
        bool IsRenderMode(const RenderMode& rMode);

        SkinManager* GetSkinManager() { return &skinManager; }
        ALighting* GetLighting() { return lighting; }

        const std::string& GetCurrentSkinID() { return currentSkinID; }
        const std::string& GetCurrentTextureID() { return currentTextureID; }


    };


    /* Exception thrown if a certain extension is not availible. */
    class GLExtensionUnavailableException : public debug::Exception
    {
    public:
        GLExtensionUnavailableException(const char* errorMessage) : Exception(errorMessage) {}

    };

}

}

#endif

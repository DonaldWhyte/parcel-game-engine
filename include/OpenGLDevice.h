/*
 * File:   OpenGLDevice.h
 * Author: Donald
 *
 * Created on December 31, 2008, 1:22 PM
 */

#ifndef OPENGLDEVICE_H
#define OPENGLDEVICE_H

#include <windows.h>
#include "DeviceParameters.h"
#include "Exceptions.h"

namespace parcel
{

namespace windows
{

    /* This class provides management of the OpenGl device, its version and any availible extensions
     * on the user's hardware vendor with a little help from GLee.
     * It uses both the GLee and WIN32 APIs. */
    class OpenGLDevice
    {


    private:

        HDC deviceContext; // OpenGL's device context for the window
        HGLRC renderContext; // OpenGL's render context

        HWND windowHandle; // Handle to the window device context will be assigned to

        bool doublebuffered; // If device context will use two buffers, this is true

        bool deviceCreated; // Flag that tells program if the OpenGL device has been created yet
        // These are flags that tell the RenderDevice if the buffers are active
        bool stencilBufferEnabled; // Stencil buffer
        bool accumBufferEnabled; // Accumulation buffer
        bool depthBufferEnabled; // Depth buffer

        // The following members are related to OpenGL's extension mechanism
        bool extensionsEnabled; // If GLEE failed to initialise, then we cannot use any extensions


        /* This methods enables the OpenGL device context in the given window handle. */
        void CreateDevice(const DeviceParameters& parameters);


    public:

        /* Constructor. Creates the OpenGL device context by using the given window handle and
         * width/height. */
        OpenGLDevice(HWND windowHandle, const DeviceParameters& parameters);
        /* Destructor. Destroys the OpenGL device context. */
        ~OpenGLDevice();

        /* Destroys the device and render context.
         * It is public because it can be called explicitly as well as through the destructor. */
        void DestroyDevice();


        /* Getters. */
        HDC GetDeviceContext() { return deviceContext; } // Device context.
        const bool& ExtensionsEnabled() const { return extensionsEnabled; } // Extensions enabled
        const bool& IsDoubleBuffered() const { return doublebuffered; }


    };


}

}

#endif

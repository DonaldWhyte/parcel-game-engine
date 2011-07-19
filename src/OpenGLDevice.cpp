 /*
 * File:   OpenGLDevice.cpp
 * Author: Donald
 *
 * Created on December 31, 2008, 1:41 PM
 */

#include <GLee.h>
#include "OpenGLDevice.h"
#include "Exceptions.h"

namespace parcel
{

namespace windows
{

    OpenGLDevice::OpenGLDevice(HWND wHandle, const DeviceParameters& parameters)
        : windowHandle(wHandle), doublebuffered(parameters.doublebuffered),
        deviceCreated(false), stencilBufferEnabled(false), accumBufferEnabled(false), depthBufferEnabled(false)
    {
        // Attempts to create the OpenGL device.
        CreateDevice(parameters);

        /* Attempts to initialises the OpenGL Easy Extension Library.
         * If it succeeds, checks extension availibility.
         * If it doesn't, print error message and close the program. */
        extensionsEnabled = (bool)GLeeInit();
        if (!extensionsEnabled)
        {
            // Throws exception to tell code to close the program
            throw debug::Exception("OpenGLDevice::OpenGLDevice - There was an error initialising the graphics device.\n"
                "Please make sure you have a compatible graphics card and the appropriate OpenGL drivers are installed.");
        }
        else
        {
            std::string missingExtensions = "";

            // Checks the availibility of every extension on
            if (!GLEE_VERSION_2_0) missingExtensions += "OpenGL Version 2.0\n";

            // If there are some missing extensions
            if (missingExtensions != "")
            {
                // Combines error message and missing extensions into one message
                std::string message =
                    "Cannot run program because the following OpenGL extensions are not supported by your driver:\n" +
                    missingExtensions;
                // Throws exception containing message
                throw debug::Exception(message);
            }
        }
    }

    OpenGLDevice::~OpenGLDevice()
    {
        /* Destroys the device context to prevent memory leaks.
         * Makes sure that a device was created in the first place though. */
        if (deviceCreated) DestroyDevice();
    }


    void OpenGLDevice::CreateDevice(const DeviceParameters& parameters)
    {
        // Return if the device has been created already
        if (deviceCreated) return;

        // Gets the window's device context
        deviceContext = GetDC(windowHandle);
        // This describes the format of the pixels that will be used by OpenGL
        PIXELFORMATDESCRIPTOR pfd;
        memset(&pfd, 0, sizeof(pfd));

        // Sets size of structure and the version of OpenGL to use
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        // If double buffered was requested, add that to the flags
        if (doublebuffered)
        {
            pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
        }
        else
        {
            pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
        }
        // The amount of bitplanes for ther accumulation buffer
        pfd.cAccumBits = parameters.accumulationBufferBits;
        // The depth of the depth buffer (z-axis)
        pfd.cDepthBits = parameters.depthBufferBits;
        // Specifies the amount of bits used in the stencil buffer, this determines its depth
        pfd.cStencilBits = parameters.stencilBufferBits;

        // Pixel type is always RGBA with the specified colour depth
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = parameters.colourDepth;
        // Sets layer type of the pixel format
        pfd.iLayerType = PFD_MAIN_PLANE;

        // Chooses the best match to the format we requested
        int pixelFormat = ChoosePixelFormat(deviceContext, &pfd);
        // Sets the pixel format to use
        SetPixelFormat(deviceContext, pixelFormat, &pfd);

        // Creates the OpenGL device
        renderContext = wglCreateContext(deviceContext);
        // And makes it the current device ( the device the window will use )
        wglMakeCurrent(deviceContext, renderContext);

        // Checks which buffers are enabled
        stencilBufferEnabled = (parameters.stencilBufferBits > 0);
        accumBufferEnabled = (parameters.accumulationBufferBits > 0);
        depthBufferEnabled = (parameters.depthBufferBits > 0);

        deviceCreated = true;
    }

    void OpenGLDevice::DestroyDevice()
    {
        // If the device has not been created yet, then it cannot be destroyed, so we return
        if (!deviceCreated) return;

        // Deselects device rendering context and deletes it
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(renderContext);
        // Releases device context from the window
        ReleaseDC(windowHandle, deviceContext);

        deviceCreated = false;
    }
}

}

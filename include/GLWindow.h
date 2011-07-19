 /*
 * File:   GLWindow.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on October 15, 2008, 13:20 PM
 * Updated to get rid of OpenGL device creation and add
 * input management on January 7, 2009, 14:16 PM
 * Updated to place OpenGL device creation back inside and
 * tidy up a lot of the redundant code. Also, buffer swapping
 * has been placed in here too. Both the class and .h/.cpp
 * files have been renamed from Window to GLWindow too. This
 * was done on May 13 2009, 19:17 PM.
 */

#ifndef GLWINDOW_H
#define GLWINDOW_H

#include "GameInterfaces.h"
#include "OpenGLDevice.h"
#include "Input.h"
#include <windows.h>

namespace parcel
{

namespace windows
{

    /* This class handles all windows functionality, handling all events like moving window,
     * mouse/keyboard input. It acts as a wrapper, so all I will have to do to get it working with
     * other operating systems is change the API used inside this class. NO Win32 API code should
     * leave this header or source files! Except for the main loop!
     *
     * The OpenGLDevice is also contained inside the class. After the window is created, the
     * window also creates the OpenGL Device. External code can access the OpenGLDevice by
     * using GetOpenGLDevice() to get a pointer to it.
     *
     * This class implements the IWindow interface so it can be used with the parcel Game class. */
    class GLWindow : public IWindow
    {


    private:

        /* Win32 structs. */
        HINSTANCE applicationInstance; // Contains handle to this application's instance
        WNDCLASS wndClass; // Contains all the needed information for the window in Windows
        HWND hWnd; // Handle for the winow

        Input input; // This is used to manage keyboard/mouse input

        /* Window Information. */
        const char* className; // Name of the window's class that is registered in Win32
        int colourDepth; // How many bits is used for each pixel
        int width, height; // Width and height of window
        bool fullscreen; // Window's fullscreen flag
        const char* title; // The title of the window to display

        /* Flag that tells the program if he has been created yet, this is used
         * so I don't accidently call Show() before Create(). */
        bool created;

        /* Used to create and store the OpenGL device context. It was made a pointer so
         * the time of construction and descruction can be explicity defined, stopping
         * the object from being delete accidently. This is important for such a vital
         * component. */
        OpenGLDevice* glDevice;


        /* Creates the window =) */
        void Create();
        /* Registers the window's class in the operating system. Called in Create(). */
        void Register();
        /* If the window is double buffered, swap the two buffers. This is called in Update(). */
        void SwapBuffers();

        /* Handles messages. Updates input. */
        static LRESULT CALLBACK MessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            switch (uMsg)
            {
                case WM_CLOSE:
                    PostQuitMessage(0);
                break;

                case WM_DESTROY:
                    return 0;

                /* If message is has not been dealt with, just call the default window procedure. */
                default:
                    return DefWindowProc(hwnd, uMsg, wParam, lParam);
            }

            // Returns 0
            return 0;
        }


    public:

        /* Constructor, creates the Win32 window class and sets its default properties.
         * If isFullScreen is true, the width and height are ignored. */
        GLWindow(HINSTANCE hInstance,
            const char* nameOfClass, const char* windowTitle,
            int windowWidth, int windowHeight, int bitsPerPixel, bool isFullscreen,
            const DeviceParameters& glParameters);
        /* Destructor, deletes window. */
        virtual ~GLWindow();

        /* Makes the window visible. OpenGL device creeation is handled here since the
         * window must be shown before creating the device. =) */
        void Show(int cmdShow);
        /* Updates the window, checks incoming messages and events as well as acting like
         * a message pump Returns false to tell main loop that the window is being destroye
         * and the application has to end.*/
        bool Update();
        /* This method sends a quit message to the OS, so the window gets destroyed. */
        void Close();

        /* Accessor methods. */

        /* Returns a constant pointer of the window's handle. */
        HWND GetHandle()
        {
            // If window has been created, return the window's handle
            if (created) return hWnd;
            // Otherwise, return a null pointer =(
            else return NULL;
        }
        /* Returns pointer to window's input message pump. */
        Input* GetInput()
        {
            return &input; // Returns pointer to input manager
        }
        /* Returns width and height of window. */
        const int& GetWidth() const
        {
            return width;
        }
        const int& GetHeight() const
        {
            return height;
        }
        /* Returns flag that determines if the window is fullscreen. */
        const bool& IsFullscreen() const
        {
            return fullscreen;
        }


    };

}

}

#endif

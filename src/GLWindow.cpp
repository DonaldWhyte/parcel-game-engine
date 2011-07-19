 /*
 * File:   GLWindow.cpp
 * Author: Donald "Datriot" Whyte
 *
 * Created on October 15, 2008, 14:20 PM
 * Updated on January 7, 2009, 14:16 PM
 * Updated and renamed from Window to GLWindow on
 * was done on May 13 2009, 19:17 PM.
 */

#include "GLWindow.h"

namespace parcel
{

namespace windows
{

    GLWindow::GLWindow(HINSTANCE hInstance, // Window parameters
        const char* nameOfClass, const char* windowTitle,
        int windowWidth, int windowHeight, int bitsPerPixel, bool isFullscreen,
        const DeviceParameters& glParameters) :
        // Sets default properties
        applicationInstance(hInstance),
        className(nameOfClass), colourDepth(bitsPerPixel), width(windowWidth),
        height(windowHeight), fullscreen(isFullscreen), title(windowTitle),
        created(false)
    {
        wndClass.style = 0;
        wndClass.lpfnWndProc = MessageHandler; // Must set! This is the window procedure
        wndClass.cbClsExtra = 0;
        wndClass.cbWndExtra = 0;
        wndClass.hInstance = hInstance; // Must set! ownder of the class.
        wndClass.hIcon = 0;
        wndClass.hCursor = LoadCursor(0, IDC_ARROW); // Optional, this loadsd up the default mouse cursor
        wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Optional, sets background colour
        wndClass.lpszMenuName = 0;
        wndClass.lpszClassName = nameOfClass; // Must set! Name of class

        // Now that window class has been created, create the window!
        Create();
        // Attaches the input to the created window
        input.AttachToWindow(hWnd);

        /* Now that the window has actually been created, create the
         * OpenGL device as well. If something goes wrong, catch the
         * exception, display the message it holds and close the window. */
        try
        {
            glDevice = new OpenGLDevice(hWnd, glParameters);
        }
        catch (debug::Exception& exception)
        {
            // Displays error message using windows message boxes
            MessageBox(hWnd,
                exception.Message().c_str(),
                "Error!",
                MB_OK);
            // Closes the window
            Close();
        }
    }

    /* Explicity destroys the window when this object goes out of scope by calling Close(). */
    GLWindow::~GLWindow()
    {
        if (hWnd) Close();
        if (glDevice) delete glDevice;
    }


    void GLWindow::Register()
    { RegisterClass(&wndClass); }

    void GLWindow::Create()
    {
        DWORD dwStyle; // Used to store the styles of the window depending on fullscreen
        int x, y; // Position of window

        // Makes sure that we have registered the window in the operating system
        Register();

        // If window will be fullscreen
        if (fullscreen)
        {
            dwStyle = WS_POPUP | WS_EX_TOPMOST;

            ShowCursor(false); // Disable cursor
            x = y = 0; // Window is at the top-left of the screen

            // Gets desktop dimensions
            RECT desktopDimensions;
            HWND desktop = GetDesktopWindow();
            GetWindowRect(desktop, &desktopDimensions);
            // Assigns them to the window's dimensions so the window will cover the entire screen
            width = desktopDimensions.right;
            height = desktopDimensions.bottom;
        }
        // Otherwise, we are in
        else
        {
            dwStyle = WS_MINIMIZEBOX | WS_SYSMENU;

            // Uses default position
            x = CW_USEDEFAULT;
            y = CW_USEDEFAULT;
        }

        // Creates the window wth the decided properties
        hWnd = CreateWindowEx(NULL,
            className,
            title,
            dwStyle,
            x, y,
            width,
            height,
            0,
            0,
            applicationInstance,
            0);

        // Window has now been created
        created = true;
    }

    void GLWindow::Show(int cmdShow)
    {
        if (created) // Only shows if created is true
        {
            // Shows and updates the window
            ShowWindow(hWnd, cmdShow);
            UpdateWindow(hWnd);
        }
    }


    bool GLWindow::Update()
    {
        MSG msg; // Messages that the window is receiving
        int status = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE); // Status of the window, -1 = Error, 0 = Quit, 1 = Other

        // If an error occured return false to tell program to abort
        if (status == -1)
            return false;
        // Also returns false if the message was to quit the application
        if (msg.message == WM_QUIT)
            return false;

        // Updates input. Gives it the received message to detect any changes in the key state
        input.Update(msg.message, msg.wParam, msg.lParam);

        // Dispatches the given message
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        // While here, we swap the buffers to display rendered content
        SwapBuffers();

        return true;
    }


    void GLWindow::Close()
    {
        // Posts a quit message to the OS
        PostQuitMessage(0);
        // Destroys the window
        DestroyWindow(hWnd);
        // And makes sure display settings are reverted back to their original
        ChangeDisplaySettings(NULL, 0);
    }


    void GLWindow::SwapBuffers()
    {
        /* Swaps buffers using the device context from OpenGLDevice.
         * NOTE: '::'operator used to access global namespace. */
        if (glDevice->IsDoubleBuffered())
        {
            ::SwapBuffers(glDevice->GetDeviceContext());
        }
    }

}

}

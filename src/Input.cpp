 /*
 * File:   Input.cpp
 * Author: Donald "Datriot" Whyte
 *
 * Created on January 7, 2009, 8:13 PM
 * Heavily revised for better keyboard and mouse detection on
 * June 22, 2009, 11:47 AM
 */

/* Forces RAWINPUTDEVICE and related Win32 APIs to be visible.
 * Only compatible with WIndows XP and above. */
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0501

/* These two macros are defined just in case they are defined elsewhere.
 * They are used to get mouse wheel information. */
#ifndef WHEEL_DELTA
    #define WHEEL_DELTA 120
#endif
#ifndef WM_MOUSEWHEEL
    #define WM_MOUSEWHEEL 0x020A
#endif

#include <windows.h>
#include "Input.h"

namespace parcel
{

namespace windows
{

        Keyboard::Keyboard() :
            currentKeyStates(keyStates[0]), previousKeyStates(keyStates[1]),
            lastCharacter(0)
        {
            // Clears key states buffers
            memset(currentKeyStates, 0, 256);
            memset(previousKeyStates, 0, 256);
        }

        void Keyboard::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
        {
            // If a character key was pressed, get that character and store it
            if (message == WM_CHAR)
            {
                lastCharacter = static_cast<int>(wParam);
            }
        }

        void Keyboard::Update()
        {
            // Makes current key states the previous key states
            BYTE* tempKeyStates = previousKeyStates;
            previousKeyStates = currentKeyStates;
            currentKeyStates = tempKeyStates;
            // Fills current key states with latest keyboard information using GetKeyboardState()
            GetKeyboardState(reinterpret_cast<BYTE*>(currentKeyStates));
        }


        /* Clears Mouse's static array, tempBuffer by filling it with 0s. */
        BYTE Mouse::tempBuffer[Mouse::tempBufferSize] = { 0 };

        Mouse::Mouse() :
            historyBufferSize(initialHistoryBufferSize),
            wheelDelta(0), previousWheelDelta(0), mouseWheel(0.0f),
            weightModifier(initialWeightModifier),
            cursorVisible(true), enableFiltering(true),
            absolutePosition(0, 0), relativePosition(0, 0)
        {
        }
        Mouse::~Mouse()
        {
            // Makes sure this mouse object is detached from any window
            Detach();
        }

        void Mouse::Attach(HWND wHandle)
        {
            // Makes sure given window handle is not a null pointer
            if (!wHandle) throw debug::NullPointerException("Mouse::Attach - Invalid window handle!");
            windowHandle = wHandle;

            // Creates raw input device and fills it with data
            RAWINPUTDEVICE rid[1] = { 0 };
            rid[0].usUsagePage = 1;
            rid[0].usUsage = 2;
            rid[0].dwFlags = 0;
            rid[0].hwndTarget = windowHandle;

            // Try to register the input device. Throws excpetion if it results in failure
            if (!RegisterRawInputDevices(rid, 1, sizeof(rid[0])))
                throw debug::Exception("Mouse::Attach - Cannot register the input device and get input data!");
            // If cursor is not suppoosed to be visible, make sure it isn't
            if (!cursorVisible)
                ShowCursor(false);

            // Initialises mouse's filtered position, omvenet and button states
            filtered = maths::vector2f(0.0f, 0.0f);
            currentButtonStates = buttonStates[0];
            previousButtonStates = buttonStates[1];
            mouseIndex = 0;
            mouseMovementX[0] = mouseMovementX[1] = 0.0f;
            mouseMovementY[0] = mouseMovementY[1] = 0.0f;
            // Clears history and button state buffers
            memset(history, 0, sizeof(history));
            memset(buttonStates, 0, sizeof(buttonStates));
        }

        void Mouse::Detach()
        {
            // If cursor is not visible, make it visible
            if (!cursorVisible)
            {
                ShowCursor(true);
            }
            // Clears window handle
            windowHandle = 0;
        }

        void Mouse::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
        {
            RAWINPUT* raw = NULL;
            unsigned int size = tempBufferSize;

            // Checks what the given message is and handles it appropriately
            switch (message)
            {
                case WM_INPUT:
                {
                    // Retrives raw input data
                    GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam),
                        RID_INPUT, tempBuffer, &size, sizeof(RAWINPUTHEADER));
                    // Converts temporary buffer to RAWINPUT structure
                    raw = reinterpret_cast<RAWINPUT*>(tempBuffer);
                    // If raw input type is a mouse, get data
                    if (raw->header.dwType == RIM_TYPEMOUSE)
                    {
                        // Gets relative position from the RAWINPUT structure
                        relativePosition.x = static_cast<float>(raw->data.mouse.lLastX);
                        relativePosition.y = static_cast<float>(raw->data.mouse.lLastY);
                        // If filtering is enabled, filter and smooth the relative position
                        if (enableFiltering)
                        {
                            PerformMouseFiltering(relativePosition);
                            relativePosition = filtered;
                            PerformMouseSmoothing(relativePosition);
                            relativePosition = filtered;
                        }
                    }

                    break;
                }
                case WM_MOUSEMOVE:
                {
                    // If mouse moves, get its absolute position
                    absolutePosition.x = static_cast<int>(static_cast<short>(LOWORD(lParam)));
                    absolutePosition.y = static_cast<int>(static_cast<short>(HIWORD(lParam)));

                    break;
                }
                case WM_MOUSEWHEEL:
                {
                    // If mouse wheel moves, get the wheel delta
                    wheelDelta += static_cast<int>(static_cast<int>(wParam) >> 16);

                    break;
                }
            }
        }

        void Mouse::ShowCursor(bool showIt)
        {
            // Shows or hides hte cursor depending on given flag
            if (showIt)
            {
                cursorVisible = true;
                // Pauses program until cursor is shown
                while (::ShowCursor(true) < 0) ;
            }
            else
            {
                cursorVisible = false;
                // Pauses program until cursor is hidden
                while (::ShowCursor(false) >= 0) ;
            }
        }

        void Mouse::ToggleCursor()
        {
            // Inverts visibility flag and calls ShowCursor()
            cursorVisible = !cursorVisible;
            ShowCursor(cursorVisible);
        }

        void Mouse::SetPosition(maths::vector2i position)
        {
            // Stores position in POINT structure so we can convert it to screen
            POINT pt = { position.x, position.y };
            /* Converts the position from client to screen, making the position
             * RELATIVE to the current window. Set cursor position only if the
             * conversion is successful. */
            if (ClientToScreen(windowHandle, &pt))
            {
                SetCursorPos(pt.x, pt.y);
                // Updates position data
                absolutePosition = position;
                relativePosition = maths::vector2f(0.0f, 0.0f);
            }
        }

        void Mouse::Update()
        {
            bool* tempButtonStates = previousButtonStates;
            // Makes current button states the previous states
            previousButtonStates = currentButtonStates;
            currentButtonStates = tempButtonStates;
            // Updates button statesby checking if any of the three buttons have been pressed
            currentButtonStates[0] = (GetKeyState(VK_LBUTTON) & 0x8000);
            currentButtonStates[1] = (GetKeyState(VK_RBUTTON) & 0x8000);
            currentButtonStates[2] = (GetKeyState(VK_MBUTTON) & 0x8000);
            // Updates mouse wheel's delta and makes previous wheel delta the current one
            mouseWheel = static_cast<float>(wheelDelta - previousWheelDelta) /
                static_cast<float>(WHEEL_DELTA);
            previousWheelDelta = wheelDelta;
        }

        void Mouse::PerformMouseFiltering(const maths::vector2f& position)
        {
            // Filters the relative position given
            for (unsigned int i = historyBufferSize - 1; (i > 0); --i)
            {
                history[i * 2] = history[(i - 1) * 2];
                history[i * 2 + 1] = history[(i - 1) * 2 + 1];
            }

            history[0] = position.x;
            history[1] = position.y;

            maths::vector2f average(0.0f, 0.0f);
            float averageTotal = 0.0f;
            float currentWeight = 1.0f;

            for (int i = 0; (i < historyBufferSize); ++i)
            {
                average.x = history[i * 2] * currentWeight;
                average.y = history[i * 2 + 1] * currentWeight;
                averageTotal += 1.0f * currentWeight;
                currentWeight *= weightModifier;
            }

            filtered.x = average.x / averageTotal;
            filtered.y = average.y / averageTotal;
        }

        void Mouse::PerformMouseSmoothing(const maths::vector2f& position)
        {
            // Smooths the relative position given
            mouseMovementX[mouseIndex] = position.x;
            mouseMovementY[mouseIndex] = position.y;

            filtered.x = (mouseMovementX[0] + mouseMovementX[1]) * 0.5f;
            filtered.y = (mouseMovementY[0] + mouseMovementY[1]) * 0.5f;

            mouseIndex ^= 1;
            mouseMovementX[mouseIndex] = 0.0f;
            mouseMovementY[mouseIndex] = 0.0f;
        }


        Input::Input() : windowHandle(NULL) // Initialises window handle to NULL
        {
        }

        Input::~Input()
        {
            // Makes sure input is detached from any window before being destroyed
            DetachFromWindow();
        }

        void Input::AttachToWindow(HWND wHandle)
        {
            // If null pointer given, throw exception
            if (wHandle == NULL)
            {
                throw debug::NullPointerException("Input::AttachToWindow - Given window handle is null!");
            }

            // Detaches from current window
            DetachFromWindow();
            // Attaches mouse to the window
            mouse.Attach(wHandle);
        }

        void Input::DetachFromWindow()
        {
            // Detaches mouse object from window
            mouse.Detach();
        }

        void Input::Update(UINT message, WPARAM wParam, LPARAM lParam)
        {
            // Passes messages to keyboard and mouse objects, then updates them
            keyboard.HandleMessage(message, wParam, lParam);
            keyboard.Update();
            mouse.HandleMessage(message, wParam, lParam);
            mouse.Update();
        }

}

}

 /*
 * File:   Input.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on January 7, 2009, 7:43 PM
 * Heavily revised for better keyboard and mouse detection on
 * June 22, 2009, 11:47 AM
 */

#ifndef INPUT_H
#define INPUT_H

#include "Vector.h"
#include "Exceptions.h"
#include <windows.h>

namespace parcel
{

namespace windows
{

    /* Enumerators for keyboard and mouse inputs. */
    enum Key
    {
        KEY_0               = 48,
        KEY_1               = 49,
        KEY_2               = 50,
        KEY_3               = 51,
        KEY_4               = 52,
        KEY_5               = 53,
        KEY_6               = 54,
        KEY_7               = 55,
        KEY_8               = 56,
        KEY_9               = 57,

        KEY_A               = 65,
        KEY_B               = 66,
        KEY_C               = 67,
        KEY_D               = 68,
        KEY_E               = 69,
        KEY_F               = 70,
        KEY_G               = 71,
        KEY_H               = 72,
        KEY_I               = 73,
        KEY_J               = 74,
        KEY_K               = 75,
        KEY_L               = 76,
        KEY_M               = 77,
        KEY_N               = 78,
        KEY_O               = 79,
        KEY_P               = 80,
        KEY_Q               = 81,
        KEY_R               = 82,
        KEY_S               = 83,
        KEY_T               = 84,
        KEY_U               = 85,
        KEY_V               = 86,
        KEY_W               = 87,
        KEY_X               = 88,
        KEY_Y               = 89,
        KEY_Z               = 90,

        KEY_F1              = 112,
        KEY_F2              = 113,
        KEY_F3              = 114,
        KEY_F4              = 115,
        KEY_F5              = 116,
        KEY_F6              = 117,
        KEY_F7              = 118,
        KEY_F8              = 119,
        KEY_F9              = 120,
        KEY_F10             = 121,
        KEY_F11             = 122,
        KEY_F12             = 123,

        KEY_BACKSPACE       = 8,
        KEY_TAB             = 9,
        KEY_ENTER           = 13,
        KEY_CAPSLOCK        = 20,
        KEY_ESCAPE          = 27,
        KEY_SPACE           = 32,
        KEY_APP             = 93,
        KEY_SEMICOLON       = 186,
        KEY_ADD             = 187,
        KEY_COMMA           = 188,
        KEY_SUBTRACT        = 189,
        KEY_PERIOD          = 190,
        KEY_FWDSLASH        = 191,
        KEY_TILDE           = 192,
        KEY_LBRACKET        = 219,
        KEY_BACKSLASH       = 220,
        KEY_RBRACKET        = 221,
        KEY_APOSTROPHE      = 222,

        KEY_SHIFT           = 16,
        KEY_CTRL            = 17,
        KEY_LWIN            = 91,
        KEY_RWIN            = 92,
        KEY_LALT            = 164,
        KEY_RALT            = 165,

        KEY_INSERT          = 45,
        KEY_DELETE          = 46,
        KEY_HOME            = 36,
        KEY_END             = 35,
        KEY_PAGEUP          = 33,
        KEY_PAGEDOWN        = 34,

        KEY_UP              = 38,
        KEY_DOWN            = 40,
        KEY_RIGHT           = 39,
        KEY_LEFT            = 37,

        KEY_NUMLOCK         = 144,
        KEY_NUMPAD_DIVIDE   = 111,
        KEY_NUMPAD_MULTIPLY = 106,
        KEY_NUMPAD_SUBTRACT = 109,
        KEY_NUMPAD_ADD      = 107,
        KEY_NUMPAD_DECIMAL  = 110,
        KEY_NUMPAD_0        = 96,
        KEY_NUMPAD_1        = 97,
        KEY_NUMPAD_2        = 98,
        KEY_NUMPAD_3        = 99,
        KEY_NUMPAD_4        = 100,
        KEY_NUMPAD_5        = 101,
        KEY_NUMPAD_6        = 102,
        KEY_NUMPAD_7        = 103,
        KEY_NUMPAD_8        = 104,
        KEY_NUMPAD_9        = 105,
    };

    enum MouseButton
    {
        BUTTON_LEFT,
        BUTTON_RIGHT,
        BUTTON_MIDDLE
    };


    /* This class manages all keyboard related states. It can be used to find
     * out what keys are being pressed and the last character that was pressed. */
    class Keyboard
    {


    private:

        BYTE keyStates[2][256]; // Stores the states of all the keys
        BYTE* currentKeyStates; // Stores the current state of the keys
        // Stores the previous state of the keys, used to find out which keys are being held
        BYTE* previousKeyStates;
        int lastCharacter; // Last character that has been pressed


    public:

        Keyboard();
        ~Keyboard() {}

        /* Handles the relevent messages appropriately. */
        void HandleMessage(UINT message, WPARAM wParam, LPARAM lParam);
        /* Updates key states by querying the new key states using Win32 API. */
        void Update();

        /* Getters. Used to check if a certain is being pressed or held and get the
         * last character that was pressed on the keyboard. */
        int GetLastCharacter() const { return lastCharacter; }
        bool KeyDown(Key key) const
        {
            return (currentKeyStates[key] & 0x80);
        }
        bool KeyUp(Key key) const
        {
            return !(currentKeyStates[key] & 0x80);
        }
        bool KeyPress(Key key) const
        {
            return ((currentKeyStates[key] & 0x80) && (previousKeyStates[key] & 0x80));
        }


    };


    /* This class manages mouse movement and input. It also provides filtering to
     * make mouse movement smoother. It stores the states of each mouse button,
     * the wheel and the mouse's absolute and relative position on the screen. */
    class Mouse
    {


    private:

        /* Following constants contain defaults for the mouse object. This
         * includes the initial history buffer size, weight modifier as well
         * as the size of the temporary buffer. */
        static const float initialWeightModifier = 2.0f;
        static const int initialHistoryBufferSize = 10;
        static const int tempBufferSize = 40;

        /* The temporary buffer holds temporary mouse information. Its size
         * is determiend by the constant 'tempBufferSize'. */
        static BYTE tempBuffer[tempBufferSize];

        HWND windowHandle; // Handle to the attached window

        int historyBufferSize; // Size of the history buffer
        int mouseIndex;
        int wheelDelta; // Change in mouse wheel position
        int previousWheelDelta; // Previous change in mouse wheel position

        float mouseWheel;
        float weightModifier; // Used for smoothing mouse
        maths::vector2f filtered; // Contains filtered/smoothed mouse position
        float history[initialHistoryBufferSize * 2];
        float mouseMovementX[2]; // Contains X movement direction of cursor
        float mouseMovementY[2]; // Contains Y movement direction of cursor

        bool cursorVisible; // Determines cursor visibility
        bool enableFiltering; // Decides if mouse movement is filtered/smoothed
        bool buttonStates[2][3]; // States of the three mouse buttons (left, middle, right)
        bool* currentButtonStates; // Current state of the three mouse buttons
        bool* previousButtonStates; // Previous state of the three mouse buttons

        maths::vector2i absolutePosition; // Absolute position of cursor in pixels
        maths::vector2f relativePosition; // Relative position of cursor in pixels


        /* Both of these methods are called within the Mouse class itself
         * and are used to smooth mouse movement if filtering is enabled. */
        void PerformMouseFiltering(const maths::vector2f& position);
        void PerformMouseSmoothing(const maths::vector2f& position);


    public:

        Mouse();
        ~Mouse();

        /* Attaches this object to a window so it can get the correct mouse state. */
        void Attach(HWND wHandle);
        /* Does opposite of above; detaches the object fron any window. */
        void Detach();

        /* Handles the relevent messages appropriately. */
        void HandleMessage(UINT message, WPARAM wParam, LPARAM lParam);
        /* Updates mouse button and wheel states. */
        void Update();

        /* Shows/hides cursor depending on boolean flag given. */
        void ShowCursor(bool showIt);
        /* Toggles cursor on and off. */
        void ToggleCursor();

        /* Sets mouse cursor's position, relative to the attached window. */
        void SetPosition(maths::vector2i position);
        /* Sets new weight modifier. Used for mouse smoothing/filtering. */
        void SetWeightModifier(float newWeightModifier) { weightModifier = newWeightModifier; }
        /* Enables/disables mouse smoothing (filtering). */
        void SmoothMouse(bool smoothIt) { enableFiltering = smoothIt; }

        /* Used to query if a button is pressed or not. */
        bool ButtonDown(MouseButton button) const
        { return currentButtonStates[button]; }
        bool ButtonPressed(MouseButton button) const
        { return (currentButtonStates[button] && !previousButtonStates[button]); }
        bool ButtonUp(MouseButton button) const
        { return !currentButtonStates[button]; }

        /* Used to query the cursor's visibility and if mouse smoothing is enabled. */
        bool CursorIsVisible() const
        { return cursorVisible; }
        bool IsMouseSmoothing() const
        { return enableFiltering; }

        /* Gets various properties of mouse object. These are the cursor's absolute
         * and relative position, weight modifier and wheel position. */
        const maths::vector2i& GetAbsolutePosition() const
        { return absolutePosition; }
        const maths::vector2f& GetRelativePosition() const
        { return relativePosition; }
        float GetWeightModifier() const
        { return weightModifier; }
        float GetWheelPosition() const
        { return mouseWheel; }


    };


    /* Used to receive Win32 messages and pass them to Keyboard and Mouse
     * objects. This is the class that is used inside GLWindow. */
    class Input
    {


    private:

        HWND windowHandle; // Handle to the attached window
        Keyboard keyboard; // Handles keyboard input
        Mouse mouse; // Handles mouse input


    public:

        Input();
        ~Input();

        /* Attaches all the needed input objects to the window. */
        void AttachToWindow(HWND wHandle);
        /* Opposite of above, called in destructor as well. */
        void DetachFromWindow();

        /* Receives messages from window class and passes them to
         * the keyboard and mouse objects, updating them. */
        void Update(UINT message, WPARAM wParam, LPARAM lParam);

        /* Used to get keyboard and mouse objects so user can query input states. */
        Keyboard* GetKeyboard() { return &keyboard; }
        Mouse* GetMouse() { return &mouse; }


    };

}

}

#endif

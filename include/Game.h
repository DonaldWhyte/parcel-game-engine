 /*
 * File:   Game.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on May 12, 2009, 6:13 PM
 */

#ifndef GAME_H
#define GAME_H

#include "GameInterfaces.h"
#include "Exceptions.h"
#include "Util.h"

namespace parcel
{

    namespace game
    {

        /* Class used as a skeleton for a game using the Parcel engine.
         * It takes four parameters, each is an interface to a specific
         * portion of a game; window, logic, graphics and sound. The
         * game starts when the Start() method is called, the main game
         * loop runs. When the game ends (when the Update() method of
         * IUpdate returns false), the loop ends and so does the game. */
        class Game
        {


        private:

            // The amount of CPU ticks per second
            static const unsigned int ticksPerSecond = 50;
            // How many ticks to skipafter every logic update
            static const unsigned int skipTicks = (1000 / ticksPerSecond);
            // The maximum amount of frames to skip
            static const unsigned int maxFrameSkip = 10;


            IWindow* window; // Creates and displays the window
            ILogic* logic; // All the game logic is managed inside this
            IGraphics* graphics; // Manages all graphics drawing in the game


        public:

            /* Constructor. All the interfaces for window, logic, sound,
             * etc. are passed to the constructor. It validates the inputs
             * and throws an exception if a NULL pointer was given. */
            Game(IWindow* gameWindow, ILogic* gameLogic,
                IGraphics* gameGraphics) :
                window(gameWindow), logic(gameLogic), graphics(gameGraphics)
            {
                if (window == NULL || logic == NULL || graphics == NULL)
                {
                    throw debug::NullPointerException("Game::Game - Given a null pointer!");
                }
            }
            /* Cleans up the game. */
            ~Game()
            {
                // Deletes all the interfaces
                delete window;
                delete logic;
                delete graphics;
            }

            /* Called once to start the game. The main game loop is contained
             * inside this method. After the game has finished, the loop breaks
             * and the method call ends. */
            void Start()
            {
                /* Game Loop!
                 * Loops until the window stops receiving messages and closes. */
                while (window->Update())
                {
                    // Gets elapsed time since last update
                    double elapsedTime = general::GetElapsedTime(true);

                    /* Updates game logic. If the update method returns false,
                     * we assume that the logic wants to end the game and close
                     * application, so we close the window to break the loop.
                     * The game logic is updated more than the graphics (which
                     * always takes long), so for a higher frame rate, we update
                     * the logic multiple times before updating the graphics. */
                    unsigned int updateCount = 0; // Counts how many logic updates there has been
                    DWORD nextGameTick = 0;
                    while((GetTickCount() > nextGameTick) && (updateCount < maxFrameSkip))
                    {

                        if (!logic->Update())
                        {
                            window->Close();
                            break;
                        }

                        nextGameTick += skipTicks;
                        ++updateCount; // Increases amount of updates
                    }

                    // After updating the logic, update the graphics once
                    graphics->Update();
                };
            }


        };

    }

}
#endif

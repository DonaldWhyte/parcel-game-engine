/*
 * File:   GameLoop.h
 * Author: Donald
 *
 * Created on January 9, 2008, 6:18 PM
 */

#ifndef GAMELOOP_H
#define GAMELOOP_H

/* This header simply provides a few variables and stuff for game loop handling.
 * Puts it all in the game state/logic namespace, simply called game.
 * NOTE: May get rid of this header and simply have the variables declared in a
 * struct to be used in the main loop. */
namespace game
{

    // The amount of CPU ticks per second
    static const unsigned int ticksPerSecond = 50;
    // How many ticks to skipafter every logic update
    static const unsigned int skipTicks = (1000 / ticksPerSecond);
    // The maximum amount of frames to skip
    static const unsigned int maxFrameSkip = 10;

}

#endif

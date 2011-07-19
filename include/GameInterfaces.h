 /*
 * File:   GameInterfaces.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on May 12, 2009, 6:46 PM
 */

#ifndef INTERFACES_H
#define INTERFACES_H

/* These interfaces are used by parcel::game::Game.
 * Ideally, IWindow is independant from the other two interfaces
 * and ILogic only has to know about IGraphics to give it instructions
 * on what to draw. IGraphics shouldn't need to know about ILogic.
 * This all depends on the user's implementation, however and is not
 * the responsiblity of the framework to enforce such designs. */

namespace parcel
{

    /* Interface used by the game's window. Any window that will be
     * used by Game needs to implement this.
     *
     * Update() is called at the beginning of every iteration of the
     * main game loop. If it returns false, the game loop ends and
     * Game calls the Close() method.
     *
     * Close() simply closes the window. */
    class IWindow
    {
    public:
        virtual bool Update() = 0;
        virtual void Close() = 0;
    };

    /* Interface that is used to link the game's logic to the Game
     * class. Update() is called many times per frame. If it returns,
     * false, it tells Game to end and close the window. */
    class ILogic
    {
    public:
        virtual bool Update() = 0;
    };

    /* Interface used to draw the game's graphics
     *
     * Update() is called once per loop, drawing every specified by
     * subclasses. */
    class IGraphics
    {
    public:
        virtual void Update() = 0;
    };
}

#endif

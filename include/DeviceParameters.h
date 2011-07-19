/*
 * File:   DeviceParameters.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on January 24, 2009, 9:01 PM
 */

#ifndef DEVICEPARAMETERS_H
#define DEVICEPARAMETERS_H

namespace parcel
{

namespace windows
{


    /* A struct that contains the parameters that are given to OpenGLDevice
     * when creating the device context. */
    struct DeviceParameters
    {
        // Is the application doublebuffered? If false, use single buffer
        bool doublebuffered;
        // The amount of bits for colour and the accumulation/depth/stencil buffers
        int colourDepth, accumulationBufferBits, depthBufferBits, stencilBufferBits;
    };


}

}

#endif

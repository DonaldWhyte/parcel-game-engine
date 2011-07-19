 /*
 * File:   ILight.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on February 24, 2009, 9:49 AM
 */

#ifndef ILIGHT_H
#define ILIGHT_H

#include "Colour.h"
#include "Vector.h"
#include "Exceptions.h"

namespace parcel
{

namespace graphics
{


    /* Enumerator that lists the different types of lights possible. */
    enum LightType
    {
        LIGHTTYPE_POSITION,
        LIGHTTYPE_DIRECTION,
        LIGHTTYPE_SPOTLIGHT,
    };


    /* Abstract light class. All lights used by the light manager uase this interface.
     * It contains all the methods that the manager uses. It's initial implementation
     * is just to throw an exception to say the operation is unsupported; the subclasses
     * have to overload and provide their own implementation. This is done instead of
     * abstract methods so the subclasses don't have to provide an implementation of
     * methods they won't even need, resulting in duplicate code.
     *
     * IsEnabled(), GetType() and the GetColour() methods should always be implemented,
     * regardless of the type of light.
     * GetPosition() are for position lights and spotlights.
     * GetDirection() are for directional lights and spotlights.
     * GetConstant/Linear/QuadraticAttenuation() are for position lights only.
     * GetSpotlightCutoff() and GetSpotlightFocus() are only for spotlights. */
    class ALight
    {

    public:

        virtual bool IsEnabled()
        {
            throw debug::UnsupportedOperationException("IsEnabled() is not supported by this class!");
        }

        virtual LightType GetType()
        {
            throw debug::UnsupportedOperationException("GetType() is not supported by this class!");
        }

        virtual colourf GetAmbientColour()
        {
            throw debug::UnsupportedOperationException("GetColour() is not supported by this class!");
        }

        virtual colourf GetDiffuseColour()
        {
            throw debug::UnsupportedOperationException("GetColour() is not supported by this class!");
        }

        virtual colourf GetSpecularColour()
        {
            throw debug::UnsupportedOperationException("GetColour() is not supported by this class!");
        }

        virtual maths::vector3f GetDirection()
        {
            throw debug::UnsupportedOperationException("GetDirection() is not supported by this class!");
        }

        virtual maths::vector3f GetPosition()
        {
            throw debug::UnsupportedOperationException("GetPosition() is not supported by this class!");
        }

        virtual float GetSpotlightCutoff()
        {
            throw debug::UnsupportedOperationException("GetSpotlightCutoff() is not supported by this class!");
        }

        virtual float GetSpotlightFocus()
        {
            throw debug::UnsupportedOperationException("GetSpotlightFocus() is not supported by this class!");
        }

        virtual float GetConstantAttenuation()
        {
            throw debug::UnsupportedOperationException("GetConstantAttenuation() is not supported by this class!");
        }

        virtual float GetLinearAttenuation()
        {
            throw debug::UnsupportedOperationException("GetLinearAttenuation() is not supported by this class!");
        }

        virtual float GetQuadraticAttenuation()
        {
            throw debug::UnsupportedOperationException("GetQuadraticAttenuation() is not supported by this class!");
        }

    };


}

}

#endif

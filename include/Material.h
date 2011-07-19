/*
 * File:   Material.h
 * Author: Donald
 *
 * Created on December 22, 2008, 8:28 PM
 */

#ifndef MATERIAL_H
#define MATERIAL_H

#include "Colour.h"

namespace parcel
{

namespace graphics
{

    /* Simple structure that holds the colour values for a surface material.
     * It stores the ambient, diffuse, specular and emmisive colours of the
     * material as well as a float that determines the power of the specular light. */
    struct Material
    {
        colourf ambient, diffuse, specular, emmisive;
        float specularPower; // Determines the power of the specular light

        /* Equality operators for the material. */
        inline bool operator==(const Material& mat)
        {
            return ((ambient == mat.ambient) && (diffuse == mat.diffuse) && (specular == mat.specular)
                && (emmisive == mat.emmisive) && (specularPower == mat.specularPower));
        }
        inline bool operator!=(const Material& mat)
        {
            return ((ambient != mat.ambient) && (diffuse != mat.diffuse) && (specular != mat.specular)
                && (emmisive != mat.emmisive) && (specularPower != mat.specularPower));
        }

    };

}

}

#endif

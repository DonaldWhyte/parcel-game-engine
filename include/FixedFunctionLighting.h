 /*
 * File:   FixedFunctionLighting.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on February 24, 2009, 9:45 AM
 */

#ifndef FIXEDFUNCTIONLIGHTING_H
#define FIXEDFUNCTIONLIGHTING_H

#include "ALight.h"
#include "ALighting.h"
#include "Logger.h"

namespace parcel
{

namespace graphics
{

    /* This class manages all the lights for fixed function OpenGL lighting. Global light settings
     * such as two sided lighting, global ambient light and so on can be set here as well. */
    class FixedFunctionLighting : public ALighting
    {


    private:

        colourf globalAmbient; // The  colour of the global ambient light of the scene
        bool twoSidedLighting;// If true, the back faces of the geometry is lit corretly as well
        bool useLocalViewer; // If true, lighting caclulatuions are based on the camera and not globally to increase quality
        bool generateNormals; // If true, this class enables OpenGL to generate normals for lighting


        /* Called before updating to disable all the lights before updating them.
         * This is done to make sure that any lights removed from the manager
         * are safely disabled without complications. */
        void DisableAllGLLights();


    public:

        /* The constructor takes a boolean that determines which state the
         * light manager will start it; enabled or disabled. */
        FixedFunctionLighting(debug::Logger* log, const bool& enabled, const bool& willDeleteAll);
        /* Disables lighting for being destroyed. Also */
        ~FixedFunctionLighting();


        /* Enables and disables lighting in a scene respectively. */
        void EnableLighting();
        void DisableLighting();


        /* This is best being called every frame. Activates and updates all the lights
         * stored in the array with the graphics API. */
        void Update();


        // Sets various properties of the light manager
        void SetGlobalAmbientColour(const colourf& colour);
        void SetTwoSidedLighting(const bool& twoSided);
        void SetUseLocalViewer(const bool& localViewer);
        void SetGenerateNormals(const bool& genNorms);


    };

}

}

#endif

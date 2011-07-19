 /*
 * File:   FixedFunctionLighting.cpp
 * Author: Donald "Datriot" Whyte
 *
 * Created on February 24, 2009, 10:10 AM
 */

#include <GLee.h>
#include "FixedFunctionLighting.h"
#include "Util.h"

namespace parcel
{

namespace graphics
{

    FixedFunctionLighting::FixedFunctionLighting(debug::Logger* log, const bool& enabled, const bool& willDeleteAll) :
        // Call the superclass, ALighting
        ALighting(log, willDeleteAll),
        // Gives other members default values
        globalAmbient(colourf(1.0f, 1.0f, 1.0f, 1.0f)),
        twoSidedLighting(false), useLocalViewer(false), generateNormals(false)
    {
        // Starts a log for this class
        logger->WriteTextAndNewLine(logID, "FixedFunctionLighting created.");

        // Depending on the value given, enable/disable lighting initially
        if (enabled)
        {
            EnableLighting();
        }
        else
        {
            DisableLighting();
        }


    }

    FixedFunctionLighting::~FixedFunctionLighting()
    {
        // Disables lighting before being destroyed
        DisableLighting();

        logger->WriteTextAndNewLine(logID,  "FixedFunctionLighting destroyed.");
    }


    void FixedFunctionLighting::EnableLighting()
    {
        // Enables lighting if it is not enabled already
        if (!glIsEnabled(GL_LIGHTING))
        {
            glEnable(GL_LIGHTING);

            // Logs event
            logger->WriteTextAndNewLine(logID,  "FixedFunctionLighting enabled!");
        }
    }

    void FixedFunctionLighting::DisableLighting()
    {
        // Same as EnableLighting(), but reversed
        if (glIsEnabled(GL_LIGHTING))
        {
            glDisable(GL_LIGHTING);

            // Logs event
            logger->WriteTextAndNewLine(logID,  "FixedFunctionLighting disabled!");
        }
    }


    void FixedFunctionLighting::DisableAllGLLights()
    {
        // Disables eight lights in OpenGL's fixed function lighting
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);
        glDisable(GL_LIGHT3);
        glDisable(GL_LIGHT4);
        glDisable(GL_LIGHT5);
        glDisable(GL_LIGHT6);
        glDisable(GL_LIGHT7);
    }


    void FixedFunctionLighting::Update()
    {
        // Enable normal generation if generateNormals is true and it hasn't already been enabled
        if (generateNormals && !glIsEnabled(GL_NORMALIZE)) glEnable(GL_NORMALIZE);

        // Sets the lighting model parameters
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient.values);
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, useLocalViewer);
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, twoSidedLighting);

        // Gets the maximum OpenGL lights possible for future use
        GLint maxGLLights;
        glGetIntegerv(GL_MAX_LIGHTS, &maxGLLights);

        // Disables all the lights in OpenGL before updating the ones stored in the manager
        DisableAllGLLights();

        // Iterates through every light
        for (unsigned int i = 0; (i < lights.size()); i++)
        {
            try
            {
                /* Before doing anything, make sure that we haven't exceeded the maximun amount
                 * of lights in this implementation of OpenGL. Break the loop if it has. */
                if (i >= (unsigned int)maxGLLights) break;

                int light; // Stores the enumerator for the OpenGL lights, such as GL_LIGHT0

                /* Because of OpenGL's awkward light enumerators, we have to go through a switch
                 * statement to get the enumerator and alter the correct light. */
                switch (i)
                {
                    case 0: light = GL_LIGHT0; break;
                    case 1: light = GL_LIGHT1; break;
                    case 2: light = GL_LIGHT2; break;
                    case 3: light = GL_LIGHT3; break;
                    case 4: light = GL_LIGHT4; break;
                    case 5: light = GL_LIGHT5; break;
                    case 6: light = GL_LIGHT6; break;
                    case 7: light = GL_LIGHT7; break;

                    // Throws an exception if the case could not be handled
                    default: throw debug::Exception("FixedFunctionLighting::Update() - No corresponding enumerator for light.");
                }

                // If the light exists and it is enabled
                if (lights[i] != NULL && lights[i]->IsEnabled())
                {
                    // Sets the light's colour
                    glLightfv(light, GL_AMBIENT, lights[i]->GetAmbientColour().values);
                    glLightfv(light, GL_DIFFUSE, lights[i]->GetDiffuseColour().values);
                    glLightfv(light, GL_SPECULAR, lights[i]->GetSpecularColour().values);

                    // Checks which type of light it is and acts appropriately
                    switch (lights[i]->GetType())
                    {
                        // If it's a positional light
                        case LIGHTTYPE_POSITION:
                        {
                            /* Gets the light's position.
                             * The fourth float is set to 1.0 to tell OpenGL that it's a positional light */
                            maths::vector3f position = lights[i]->GetPosition();
                            float values[4] = { position.x, position.y, position.z, 1.0f };

                            // Passes the type and values of the light to OpenGL
                            glLightfv(light, GL_POSITION, values);

                            // Sets the attenuation properties of the light
                            glLightf(light, GL_CONSTANT_ATTENUATION, lights[i]->GetConstantAttenuation());
                            glLightf(light, GL_LINEAR_ATTENUATION, lights[i]->GetLinearAttenuation());
                            glLightf(light, GL_QUADRATIC_ATTENUATION, lights[i]->GetQuadraticAttenuation());

                            break;
                        }
                        case LIGHTTYPE_DIRECTION:
                        {
                            /* Gets the light's direction.
                             * The fourth float is set to 0.0 to tell OpenGL that it's a directional light */
                            maths::vector3f direction = lights[i]->GetDirection();
                            float values[4] = { direction.x, direction.y, direction.z, 0.0f };

                            glLightfv(light, GL_POSITION, values);

                            break;
                        }
                        case LIGHTTYPE_SPOTLIGHT:
                        {
                            /* Gets the light's position.
                             * The fourth float is set to 1.0 to tell OpenGL that it's a positional light */
                            maths::vector3f position = lights[i]->GetPosition();
                            float values[4] = { position.x, position.y, position.z, 1.0f };

                            // Sets up a position light as normal
                            glLightfv(light, GL_POSITION, values);

                            // Sets the attenuation properties of the light
                            glLightf(light, GL_CONSTANT_ATTENUATION, lights[i]->GetConstantAttenuation());
                            glLightf(light, GL_LINEAR_ATTENUATION, lights[i]->GetLinearAttenuation());
                            glLightf(light, GL_QUADRATIC_ATTENUATION, lights[i]->GetQuadraticAttenuation());

                            /* Sets spotlight specific parameters, which are the direction, size (cutoff)
                             * and focus (exponent) of the spotlight. */
                            glLightfv(light, GL_SPOT_DIRECTION, lights[i]->GetDirection().values);
                            glLightf(light, GL_SPOT_CUTOFF, lights[i]->GetSpotlightCutoff());
                            glLightf(light, GL_SPOT_EXPONENT, lights[i]->GetSpotlightFocus());

                            break;
                        }

                        default: throw debug::Exception("FixedFunctionLighting::Update() - Cannot recognize this light's type.");
                    }

                    // Enable the light
                    glEnable(light);
                }
            }
            // Catch exceptions and log the message
            catch (debug::Exception& ex)
            {
                logger->WriteTextAndNewLine(logID, ex.Message());
            }
            // Catches anything and logs generic error message
            catch (...)
            {
                logger->WriteTextAndNewLine(logID, "FixedFunctionLighting::Update() - Failed for unknown reason!");
            }
        }
    }


    void FixedFunctionLighting::SetGlobalAmbientColour(const colourf& colour)
    {
        globalAmbient = colour;
    }

    void FixedFunctionLighting::SetTwoSidedLighting(const bool& twoSided)
    {
        twoSidedLighting = twoSided;
    }

    void FixedFunctionLighting::SetUseLocalViewer(const bool& localViewer)
    {
        useLocalViewer = localViewer;
    }

    void FixedFunctionLighting::SetGenerateNormals(const bool& genNorms)
    {
        generateNormals = genNorms;
    }

}

}

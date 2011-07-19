 /*
 * File:   ALighting.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on February 28, 2009, 6:16 PM
 */

#ifndef ALIGHTING_H
#define ALIGHTING_H

#include <vector>
#include "ALight.h"
#include "Logger.h" // For logging
#include "Util.h" // For logging as well

namespace parcel
{

namespace graphics
{

    /* Abstract lighting class. Provides an interface for the RenderDevide to use.
     * Classes can derive from this to create their own lighting managed, such as fixed
     * function lighting, shader lighting an so on without the RenderDevice knowing. */
    class ALighting
    {


    protected:

        debug::Logger* logger; // Pointer to the application's logger
        unsigned int logID; // ID of the renderer's log

        bool deleteAll; // Determines if all renderables get deleted when this renderer does

        std::vector<ALight*> lights; // Stores pointers to all the lights used by this class
        std::vector<unsigned int> lightIDs; // Stores the IDs of every light
        unsigned int idCount; // Amount of IDs distributed, used for giving renderable's correct clean IDs


    public:

        ALighting(debug::Logger* log, const bool& willDeleteAll) : logger(log), deleteAll(willDeleteAll), idCount(0)
        {
            logID = log->StartLog("Lighting");
            log->WriteTextAndNewLine(logID, "Light manager created.");
        }

        virtual ~ALighting()
        {
            // Makes sure to release all the resources used by the lights if deleteAll is true
            if (deleteAll)
            {

                for (unsigned int i = 0; (i < lights.size()); i++)
                {
                    delete lights[i];
                    lights[i] = NULL;
                }
            }

            logger->WriteTextAndNewLine(logID, "Light manager destroyed.");
        }


        virtual unsigned int AddLight(ALight* light)
        {
            // Pushes new light into the array and adds its new ID lightIDs array
            lights.push_back(light);
            idCount++;
            lightIDs.push_back(idCount);

            // Creates message and logs it
            std::string message = "Light #";
            message += general::ToString(idCount); message += " added.";
            logger->WriteTextAndNewLine(logID, message);

            // Returns the light's new ID
            return idCount;
        }

        virtual void RemoveLight(const unsigned int& lightID)
        {
            unsigned int i;
            bool deleted = false; // Turns to true if it finds the light and deletes it

            // Searches through every light
            for (i = 0; (i < lights.size()); i++)
            {
                // If ID of light is the same as the given one, erase it!
                if (lightIDs[i] == lightID)
                {
                    try
                    {
                        // Erase the light from the vector
                        lights.erase(lights.begin() + i);
                        // Also erase the light's ID from the array
                        lightIDs.erase(lightIDs.begin() + i);

                        deleted = true;
                    }
                    catch (...)
                    {
                        logger->WriteTextAndNewLine(logID, "Could not remove light! Vector erase failed!");
                    }
                }
            }

            // Start of log message
            std::string message = "Light #";
            // If it found the light and deleted it
            if (deleted)
            {
                message += general::ToString(lightID); message += " removed.";
                logger->WriteTextAndNewLine(logID, message);
            }
            // If it never found it
            else
            {
                message += general::ToString(lightID); message += " doesn't exist. Cannot remove.";
                logger->WriteTextAndNewLine(logID, message);
            }
        }


        /* Must be implemented by subclasses. Enables and disables lighting. */
        virtual void EnableLighting() = 0;
        virtual void DisableLighting() = 0;

        /* Updates the light's colour, positon and so on. Most likely called every frame. */
        virtual void Update() = 0;



    };

}

}

#endif

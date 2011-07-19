/*
 * File:   ARenderer.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on February 9, 2009, 5:10 PM
 */

#ifndef ARENDERER_H
#define ARENDERER_H

#include <string>
#include "RenderInterfaces.h"
#include "Logger.h"
#include "Util.h" // Included for ToString which is used for logging

namespace parcel
{

namespace graphics
{

    /* Abstract class that offers storage capabilities for renderables. Render-related operations (Update() and Render())
     * have to be overrided by subclasses. */
    class ARenderer
    {


    protected:

        debug::Logger* logger; // Pointer to the application's logger
        unsigned int logID; // ID of the renderer's log

        bool deleteAll; // Determines if all renderables get deleted when this renderer does

        std::vector<IRenderable*> renderables; // Holds all the renderables
        std::vector<unsigned int> renderableIDs; // Stores IDs of all renderables; IDs are used for removing renderables
        unsigned int idCount; // Amount of IDs distributed, used for giving renderable's correct clean IDs

    public:

        /* Constructor. Gives it default properties. */
        ARenderer(debug::Logger* log, const std::string& logName, const bool& willDeleteAll) :
            logger(log), deleteAll(willDeleteAll), idCount(0)
        {
            logID = log->StartLog(logName);
            log->WriteTextAndNewLine(logID, "Renderer created.");
        }


        /* Makes sure to delete all the remaining renderables. */
        virtual ~ARenderer()
        {
            // Makes sure to release all the resources used by the renderables if deleteAll is true
            if (deleteAll)
            {
                for (unsigned int i = 0; (i < renderables.size()); i++)
                {
                    delete renderables[i];
                    renderables[i] = NULL;
                }
            }

            logger->WriteTextAndNewLine(logID, "Renderer destroyed.");
        }


        /* Adds a renderable and returns its ID. */
        virtual unsigned int AddRenderable(IRenderable* renderable)
        {
            // Pushes new renderable into the array and adds its new ID into renderbleIDs array
            renderables.push_back(renderable);
            idCount++;
            renderableIDs.push_back(idCount);

            // Creates message and logs it
            std::string message = "Renderable #";
            message += general::ToString(idCount); message += " added.";
            logger->WriteTextAndNewLine(logID, message);

            // Returns the renderable's new ID
            return idCount;
        }


        /* Removes renderable with the specified ID.
         * If it cannot find a renderable with the ID given, it just does nothing. */
        virtual void RemoveRenderable(unsigned int renderableID)
        {
            unsigned int i;
            bool deleted = false; // Turns to true if it finds the renderable and deletes it

            // Searches through all the renderables
            for(i = 0; (i < renderables.size()); i++)
            {
                // If ID of renderable is the same as the given one, erase it!
                if (renderableIDs[i] == renderableID)
                {
                    try
                    {
                        // Makes sure that element exists at i for both renderables and renderableIDs
                        renderables.erase(renderables.begin() + i);
                        // Also erase the renderable's ID from the array
                        renderableIDs.erase(renderableIDs.begin() + i);

                        deleted = true;
                    }
                    catch (...)
                    {
                        logger->WriteTextAndNewLine(logID, "Could not remove renderable! Vector erase failed!");
                    }
                }
            }

            // Start of log message
            std::string message = "Renderable #";
            // If it found the renderable and deleted it
            if (deleted)
            {
                message += general::ToString(renderableID); message += " removed.";
                logger->WriteTextAndNewLine(logID, message);
            }
            // If it never found it
            else
            {
                message += general::ToString(renderableID); message += " doesn't exist. Cannot remove.";
                logger->WriteTextAndNewLine(logID, message);
            }
        }


        /* Updates the vertex array/other information. */
        virtual void Update() = 0;

        /* Renders every renderable stored in the renderer. */
        virtual void Render() = 0;


    };



}

}

#endif

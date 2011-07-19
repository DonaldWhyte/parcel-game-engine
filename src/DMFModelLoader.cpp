/*
 * File:   DMFModelLoader.cpp
 * Author: Donald "Datriot" Whyte
 *
 * Created on March 21, 2009, 9:52 AM
 */

#include "DMFModelLoader.h"
#include "Util.h"
#include "Exceptions.h"

namespace parcel
{

namespace graphics
{

    DMFModelLoader::DMFModelLoader(SkinManager* sManager) : skinManager(sManager)
    {
    }
    DMFModelLoader::~DMFModelLoader()
    {
    }


    bool DMFModelLoader::LoadFromFile(const std::string& filename)
    {
        try
        {
            std::ifstream file; // The file stream
            DMFHeader header; // The DMF's header, contains information about the memory, etc.
            char identifier = 0x0000; // Used to store DMF identifiers read from file
            bool end = false; //

            // Opens the file and if it fails to open, throw an exception
            file.open(filename.c_str(), std::ios::binary);
            if (!file.is_open())
            {
                throw debug::Exception("DMFModelLoader::LoadFromFile - Could not load specified file.");
            }

            // Loops until the end of the file to try and find the DMF_FILE identifier so it knows it's a DMF model
            while(!file.eof())
            {
                // Gets the next character in the file stream
                identifier = file.get();
                // If it finds the DMF identifier...
                if (identifier == DMF_FILE)
                {
                    // ...read the DMF's header
                    file.read((char*)&header, sizeof(header));
                    // And then break the loop
                    break;
                }
            }
            // If the identifier was NOT found, close file throw an exception
            if (identifier != DMF_FILE)
            {
                file.close();
                throw debug::Exception("DMFModelLoader::LoadFromFile - DMF header not found in file.");
            }

            // Now that we know it's a DMF file, we search for other DMF identifiers
            while(!file.eof())
            {
                // Once again, gets the next character in the file...
                identifier = file.get();

                // ...and checks what it is
                switch (identifier)
                {
                    // If it's the geometry specification chunk
                    case DMF_GEOMETRY_SPEC:
                    {
                        end = false;

                        while(!file.eof() && !end)
                        {
                            /* Tries to get the next identifier and check what it is. Because the DMF_GEOMETRY_SPEC
                             * was used previously, the mode loader will ONLY recognise related identifiers. */
                            identifier = file.get();

                            switch(identifier)
                            {
                                case DMF_VERTICES:
                                {
                                    // Resizes the vector (vertices) to the needed size
                                    vertices.resize(header.numVertices);
                                    // Reads the vertex data and stores it in vertices
                                    file.read((char*)&vertices[0], sizeof(Vertex) * header.numVertices);

                                    break;
                                }
                                case DMF_TRIANGLES:
                                {
                                    // Does the same as DMF_VERTICES, but for triangle faces instead
                                    triangles.resize(header.numTriangles);
                                    file.read((char*)&triangles[0], sizeof(Triangle) * header.numTriangles);

                                    break;
                                }
                                case DMF_END:
                                {
                                    // Stop reading commands in GEOMETRY_SPEC mode
                                    end = true;

                                    break;
                                }
                            }
                        }

                        break;
                    }

                    // If it's the skin specification chunk
                    case DMF_SKIN_SPEC:
                    {
                        end = false;

                        while(!file.eof() && !end)
                        {
                            identifier = file.get();

                            switch(identifier)
                            {
                                case DMF_SKINS:
                                {
                                    // Resizes skins array accordingly
                                    skins.resize(header.numSkins);
                                    // Reads the skin properties and puts it in skin
                                    file.read((char*)&skins[0], sizeof(DMFSkin) * header.numSkins);

                                    break;
                                }
                                case DMF_TEXTURES:
                                {
                                    // Resizes the texture array accordingly
                                    textures.resize(header.numTextures);

                                    for (unsigned int i = 0; (i < textures.size()); i++)
                                    {
                                        unsigned int id; // Stores texture ID
                                        char filename[100]; // Stores texture filename

                                        // Reads texture ID
                                        file.read((char*)&id, sizeof(id));
                                        // Gets the texture's filename
                                        for (unsigned int j = 0; (j < 100); j++)
                                        {
                                            // Gets next character and adds it to the texture's filename
                                            filename[j] = file.get();
                                            // If it the ? character is read
                                            if (filename[j] == '?')
                                            {
                                                // Deletes the ? character
                                                filename[j] = 0x0000;
                                                // Stops getting characters for the filename by breaking the loop
                                                break;
                                            }
                                        }

                                        // Puts texture's ID and filename in object and adds that object to the list
                                        DMFTexture texture = { id, filename };
                                        textures[i] = texture;
                                    }

                                    break;
                                }
                                case DMF_MATERIALS:
                                {
                                    materials.resize(header.numMaterials);
                                    // Reads the skin properties and puts it in skin
                                    file.read((char*)&materials[0], sizeof(DMFMaterial) * header.numMaterials);

                                    break;
                                }
                                case DMF_END:
                                {
                                    // Stop reading commands in SKIN_SPEC mode
                                    end = true;

                                    break;
                                }
                            }
                        }

                        break;
                    }

                    case DMF_ANIMATION_SPEC:
                    {
                        break;
                    }
                }
            }



            /* Using the gathered material and texture, it's time to load the materials
             * and textures into skins that can be used with the Parcel engine. */

            loadedSkins.clear(); // Clears all the loaded skins

            // Goes through all the DMF skins loaded from file
            for (unsigned int i = 0; (i < skins.size()); i++)
            {
                // Creates the new skin's ID and saves it loadedSkins
                loadedSkins.push_back(filename + "Skin" + general::ToString(i));
                // Temporarily stores texture and material properties
                std::string textureFilename;
                Material material;
                // The texture and material IDs stored in the skin
                unsigned int texID = skins[i].textureID;
                unsigned int matID = skins[i].materialID;
                /* Flags that determine whether or not the texture and material has been found.
                 * Both must be true for the skin to be created for the engine. */
                bool textureFound = false;
                bool materialFound = false;

                // Gets the texture filename using ID specified by DMFSkin
                for (unsigned int j = 0; (j < textures.size()); j++)
                {
                    if (textures[j].id == texID)
                    {
                        textureFilename = textures[j].filename;

                        textureFound = true; // Texture filename found, turn to true!
                    }
                }
                // Same as above, but for materials
                for (unsigned int j = 0; (j < materials.size()); j++)
                {
                    if (materials[j].id == matID)
                    {
                        material = materials[j].material;

                        materialFound = true;
                    }
                }

                // If the texture and the material were found
                if (textureFound && materialFound)
                {
                    // Determines ID of the skin's texture
                    std::string textureID = (loadedSkins[i] + "Texture" + general::ToString(i));
                    // Uses that skin ID to store the new skin
                    skinManager->AddSkin(loadedSkins[i], material);
                    // Loads the texture into the skin IF the filename is not blank
                    if (textureFilename != "")
                    {
                        skinManager->AddTextureToSkin(loadedSkins[i], textureID, textureFilename, 1.0f, true);
                    }
                }
                else
                {
                    throw debug::Exception("DMFModelLoader::LoadFromFile - Material or texture ID specified in skin cannot be found!");
                }
            }

            // Model loading was successful, return true!
            return true;
        }
        // If any Parcel/STL exceptions were thrown, print them on the command line
        catch (debug::Exception& ex)
        {
            ex.PrintMessage();

            return false;
        }
        catch (std::exception& ex)
        {
            std::cout << ex.what() << std::endl;

            return false;
        }
        catch (...)
        {
            std::cout << "Unknown error occured when loading model " << filename << "." << std::endl;

            return false;
        }
    }


    bool DMFModelLoader::WriteToFile(const DMFModelLoader& model, const std::string& filename)
    {
        try
        {
            std::ofstream file; // File stream used to write the data
            char id; // Contains DMF identifier to write

            // Attempts to open the file and if it fails, throw an exception
            file.open(filename.c_str(), std::ios::binary);
            if (!file.is_open())
            {
                throw debug::Exception("DMFModelLoader::WriteToFile - Could not open the specified file.");
            }

            // Before anything else, adds the DMF_FILE flag to the file
            id = DMF_FILE;
            file.write(&id, sizeof(id));
            // Creates the hader and writes it to the file
            DMFHeader header =
            {
                1, // Version of file

                // The amount of each data, such as vertices, normals or materials
                model.vertices.size(),
                model.triangles.size(),

                model.skins.size(),
                model.textures.size(),
                model.materials.size()
            };
            file.write((char*)&header, sizeof(header));

            // We begin with geometry, so we add the GEOMETRY_SPEC flag
            id = DMF_GEOMETRY_SPEC;
            file.write(&id, sizeof(id));

            // Now we write the model's vertices, texture coordinates and normals
            id = DMF_VERTICES;
            file.write(&id, sizeof(id));
            file.write((char*)&model.vertices[0], sizeof(Vertex) * model.vertices.size());

            // After the vertices, the face (triangle index) data is writtten
            id = DMF_TRIANGLES;
            file.write((char*)&id, sizeof(id));
            file.write((char*)&model.triangles[0], sizeof(Triangle) * model.triangles.size());

            // The END flag is placed to tell the loader that the file has finished specifying geometry.
            id = DMF_END;
            file.write(&id, sizeof(id));



            // It's time to write the skin, material and texture data, so we place a SKIN_SPEC flag
            id = DMF_SKIN_SPEC;
            file.write(&id, sizeof(id));

            // Writes TEXTURES flag to tell loader there is are textures ahead
            id = DMF_TEXTURES;
            file.write((char*)&id, sizeof(id));
            // Writes all of the model's textures
            for (unsigned int i = 0; (i < model.textures.size()); i++)
            {
                // Writes the texture's ID
                file.write((char*)&model.textures[i].id, sizeof(model.textures[i].id));

                // Gets the filename of the texture
                std::string filename = model.textures[i].filename;

                // Adds a ? at the end to tell the loader that it is the end of the string
                filename.resize(filename.size() + 1, '?');

                // Writes the texture's filename to the file
                file.write((char*)&filename[0], (sizeof(char) * filename.size()));
            }

            // Writes the material flag here
            id = DMF_MATERIALS;
            file.write((char*)&id, sizeof(id));
            // Writes all of the model's materials
            file.write((char*)&model.materials[0], sizeof(DMFMaterial) * model.materials.size());

            // Once again, writes the flag
            id = DMF_SKINS;
            file.write((char*)&id, sizeof(id));
            // Writes of the all of the model's skins
            file.write((char*)&model.skins[0], sizeof(DMFSkin) * model.skins.size());

            // We are finished writing skin, texture and material data
            id = DMF_END;
            file.write(&id, sizeof(id));


            // PUT ANIMATION WRITING CODE HERE!!


            // The data of the model has been written, so we close the file stream
            file.close();

            // Model writing went successful, return true
            return true;
        }
        catch (debug::Exception& ex)
        {
            ex.PrintMessage();

            return false;
        }
        catch (std::exception& ex)
        {
            std::cout << ex.what() << std::endl;

            return false;
        }
        catch (...)
        {
            std::cout << "Unknown error occured when writing model" << filename << "." << std::endl;

            return false;
        }
    }

}

}

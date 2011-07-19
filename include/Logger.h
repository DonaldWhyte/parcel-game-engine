/*
 * File:   Logger.h
 * Author: Donald
 *
 * Created on December 18, 2008, 5:28 PM
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

namespace parcel
{

namespace debug
{

    /* Class that supports basic loggging features. Can create multiple logs at once. */
    class Logger
    {

    private:

        /* Fields. */
        static const int maximumLogs = 10; // The maximum amount of logs allowed open
        std::ofstream logs[maximumLogs]; // Array that holds file stream of each log

    public:

        /* Constructor and Destructor. */
        Logger();
        ~Logger(); // Closes any open streams

        /* This method adds a new ofstream to the array and returns
         * the index inside the array that the ofstream is stored. in*/
        unsigned int StartLog(std::string name);
        /* Closes ofstream at the given index. */
        void EndLog(const int& logIndex);

        /* Writes some text into the log at the given index.
         * Automatically starts a new line after it unless you send
         * false as the newline parameter in the overloaded method. */
         void WriteTextAndNewLine(const int& logIndex, const std::string& text);
         void WriteText(const int&logIndex, const std::string& text);
         // These overloads make the logger support C strings.
         void WriteTextAndNewLine(const int& logIndex, const char* text);
         void WriteText(const int&logIndex, const char* text);
         /* All the different overloads for writing a number in the log. */
         void WriteNumber(const int& logIndex, const int& number);
         void WriteNumber(const int& logIndex, const float& number);
         void WriteNumber(const int& logIndex, const double& number);
         /* Creates a new line in the log. */
         void NewLine(const int& logIndex);

    };

}

}

#endif

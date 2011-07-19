/*
 * File:   Logger.cpp
 * Author: Donald
 *
 * Created on December 18, 2008, 6:30 PM
 */

#include "Logger.h"

namespace parcel
{

namespace debug
{

    Logger::Logger() {}

    Logger::~Logger()
    {
        // Makes sure every stream is closed
        for (int i = 0; (i < maximumLogs); i++)
        {
            if (logs[i].is_open()) logs[i].close();
        }
    }


    unsigned int Logger::StartLog(std::string name)
    {
        // Adds extension onto the given name
        name += ".log";

        // Finds a place to put the new stream
        for (int i = 0; (i < maximumLogs); i++)
        {
            // If the stream is not open, open it with the given filename
            if (!logs[i].is_open())
            {
                logs[i].open(name.c_str());
                return i; // Returns the index for the newly created log
            }
        }

        return 0; // If space wasn't found and log could not be opened, return 0
                       // NOTE: This is a valid index in the log array so this could cause unexpected behaviour
    }

    void Logger::EndLog(const int& logIndex)
    {
        // Closes the log if it is open
        if (logs[logIndex].is_open()) logs[logIndex].close();
    }


    void Logger::WriteTextAndNewLine(const int& logIndex, const std::string& text)
    {
        // If log is open, add the text and a new line
        if (logs[logIndex].is_open()) logs[logIndex] << text << std::endl;
    }
    void Logger::WriteText(const int& logIndex, const std::string& text)
    {
        // If log is open, adds ext to the file stream
        if (logs[logIndex].is_open()) logs[logIndex] << text;
    }
    void Logger::WriteTextAndNewLine(const int& logIndex, const char* text)
    {
        if (logs[logIndex].is_open()) logs[logIndex] << text << std::endl;
    }
    void Logger::WriteText(const int& logIndex, const char* text)
    {
        if (logs[logIndex].is_open()) logs[logIndex] << text;
    }

    void Logger::WriteNumber(const int& logIndex, const int& number)
    {
        // If log is open, add the number to the stream
        if (logs[logIndex].is_open()) logs[logIndex] << number;
    }
    void Logger::WriteNumber(const int& logIndex, const float& number)
    {
        if (logs[logIndex].is_open()) logs[logIndex] << number;
    }
    void Logger::WriteNumber(const int& logIndex, const double& number)
    {
        if (logs[logIndex].is_open()) logs[logIndex] << number;
    }

    void Logger::NewLine(const int& logIndex)
    {
        // If log is open, add a new line
        if (logs[logIndex].is_open()) logs[logIndex] << std::endl;
    }

}

}

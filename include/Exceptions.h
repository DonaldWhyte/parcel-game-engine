/*
 * File:   Exceptions.h
 * Author: Donald
 *
 * Created on December 18, 2008, 7:40 PM
 */

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>
#include <iostream>

namespace parcel
{

namespace debug
{

    /* Generic exception class. All other exceptions derive from this. */
    class Exception
    {

    protected:
        std::string message; // Message displayed/logged when exception is caught

    public:

        //Exception() {} // Default constructor for inheritence
        Exception(const std::string& errorMessage) : message(errorMessage) {} // Constructor

        /* Gets the message the exception holds. */
        const std::string& Message() const { return message; }
        /* Prints the message the exception holds. */
        void PrintMessage() const { std::cout << message << std::endl; }

    };


    /* Exception thrown when null pointer is given instead of object. */
    class NullPointerException : public Exception
    {
    public:
        NullPointerException(const std::string& errorMessage) : Exception(errorMessage) {}

    };


    /* Exception thrown when a memory allocation failed due to lack of memory space. */
    class OutOfMemoryException : public Exception
    {
        public:
            OutOfMemoryException(const std::string& errorMessage) : Exception(errorMessage) {}
    };


    /* Exception thrown when an invalid argument was given for a function. */
    class InvalidArgumentException : public Exception
    {
        public:
            InvalidArgumentException(const std::string& errorMessage) : Exception(errorMessage) {}
    };


    /* Exception thrown when an operation for a class is not supported. */
    class UnsupportedOperationException : public Exception
    {
        public:
            UnsupportedOperationException(const std::string& errorMessage) : Exception(errorMessage) {}
    };

}

}

#endif

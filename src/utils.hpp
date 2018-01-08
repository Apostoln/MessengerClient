#ifndef MESSENGERCLIENT_UTILS_HPP
#define MESSENGERCLIENT_UTILS_HPP

#endif //MESSENGERCLIENT_UTILS_HPP

#include <easylogging++.h>

el::Level getLogLevel(std::string logLevel) {
    /*
     *     Trace
     *     Debug
     *     Fatal
     *     Error
     *     Warning
     *     Info
     *     Verbose
     */
    std::transform(logLevel.begin(), logLevel.end(),logLevel.begin(), ::toupper);

    if ("DEBUG" == logLevel) {
        return el::Level::Debug;
    }
    else if ("FATAL" == logLevel) {
        return el::Level::Fatal;
    }
    else if ("ERROR" == logLevel) {
        return el::Level::Error;
    }
    else if ("WARNING" == logLevel) {
        return el::Level::Warning;
    }
    else if ("INFO" == logLevel) {
        return el::Level::Info;
    }
    else {
        return el::Level::Unknown;
    }
}
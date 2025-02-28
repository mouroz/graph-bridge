#ifndef RESPONSE_STRUCT_H
#define RESPONSE_STRUCT_H

/**
 * @brief A structure to encapsulate a response with a value and status
 *
 * The `Response` structure is used to store both the result (a value of type T) 
 * and the status (an integer). By default, the status is set to `0` (indicating success),
 * and the value holds the response data. If the status is set to `-1`, it indicates an error 
 * or failure condition. This structure is helpful for functions that may return both 
 * a result and a status code (success or failure).
 *
 * @tparam T The type of the value stored in the response
 */

namespace ResponseStatus {
    const int OK = 0;
    const int ERROR = -1;
};

template <typename T>
struct Response {
    std::string message = ""; // String empty signals
     
    T value; // Must have default initialization. 
             // Assign value on initialization, otherwise a default copy is created first
            

    bool isOk() { return message == ""; }

    Response(std::string message, T value) : message(message), value(value) {}
    Response(T value) : message(""), value(value) {}
    
};


template <>
struct Response<void> {
    std::string message = ""; // String empty signals there is no error
            

    bool isOk() { return message == ""; }

    Response(std::string message) : message(message) {}
    Response() : message("") {}
};





template <typename T>
struct TimedResponse {
    std::string error = "";
    long duration = 0;
    T value;


    bool isOk() { return error == ""; }
    
    TimedResponse(std::string message, long duration, T value) : error(message), duration(duration), value(value) {}
    TimedResponse(long duration, T value) : error(""), duration(duration), value(value) {}
};

template <>
struct TimedResponse<void> {
    std::string error = "";
    long duration = 0;

    bool isOk() { return error == ""; }

    TimedResponse(std::string message, long duration) : error(message), duration(duration) {}
    TimedResponse(long duration) : error(""), duration(duration) {}
};

#endif
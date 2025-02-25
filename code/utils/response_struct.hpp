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
template <typename T>
struct Response {
    int status = 0; //default value is ok
    T val;

    bool is_ok() { return status == 0; }

    // Standart "not ok" value without any meaning
    void set_not_ok() { status = -1; }
  
};

#endif
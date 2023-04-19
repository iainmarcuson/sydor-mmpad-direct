//******************************************************************
/// @file stutil_error.h
/// @brief Utility functions to deal with error codes
/// @author      W.R. Lear
/// @date        08/09/2019
/// @copyright   Copyright (c) 2019 Sydor Technologies, 
///              All rights reserved.
///
//******************************************************************
#ifndef STUTIL_ERROR_H
#define STUTIL_ERROR_H

#include <stdint.h>
#include <stdbool.h>
#include <string>
#include "st_errors.h"

namespace STUTIL
{

//******************************************************************
// Error Function Definitions
//******************************************************************

//------------------------------------------------------------------
/// Get string representation of an error code
///
/// @param[in] errorcode           error code to convert to string
///
/// @returns string containing description of error code
///
std::string getErrorStr(int32_t errorcode);

//------------------------------------------------------------------
/// Convert an operating system error number to an error code
///
/// @param[in] errorNumber           errno to convert
///
/// @returns a negative error number equivalent to errorNumber
///
int32_t convertErrno(int32_t errorNumber);

//------------------------------------------------------------------
/// return true if a code is a status code
inline bool isStatusCode(int32_t code)
{
    return ((code < ST_STATUS_BASE) && (code >= ST_STATUS_END));
}

//------------------------------------------------------------------
/// return true if a code is an error code
inline bool isErrorCode(int32_t code)
{
    return (((code < 0) && (code > ST_ERR_END)) ||
        (code <= ST_ERR_SYSTEM));
}

//------------------------------------------------------------------
/// return true if a code is a native error code
inline bool isNativeErrorCode(int32_t code)
{
    return ((code < ST_ERR_BASE) && (code >= ST_ERR_END));
}

} //namespace STUTIL

//******************************************************************
// End of file
//******************************************************************
#endif // STUTIL_ERROR_H

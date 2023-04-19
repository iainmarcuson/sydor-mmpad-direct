//******************************************************************
/// @file stutil_misc.h
/// @brief Miscellaneous utility Functions
/// @author      W.R. Lear
/// @date        10/11/2021
/// @copyright   Copyright (c) 2021 Sydor Technologies, 
///              All rights reserved.
///
//******************************************************************
#ifndef STUTIL_MISC_H
#define STUTIL_MISC_H

#include <stdint.h>

namespace STUTIL
{

//******************************************************************
// Definitions and Constants
//******************************************************************

//******************************************************************
// String Function Definitions
//******************************************************************

//------------------------------------------------------------------
/// Round  value up to a multiple of N
/// @param[in] value        value to round
/// @param[in] multiple     desired multiple
///
/// @return rounded value
///
//------------------------------------------------------------------
// Round a value up to a multiple of N
template<class T>
T roundUp(const T& value, uint32_t multiple)
{
    if(multiple <= 1)
    {
        return value;
    }
    
    return static_cast<T>( ((value + multiple - 1) / multiple) * multiple);
 
}

//------------------------------------------------------------------
// Round a value down to a multiple of N
template<class T>
T roundDown(const T& value, uint32_t multiple)
{
    if(multiple <= 1)
    {
        return value;
    }
    
    return (value / multiple) * multiple;
 
}

} // namespace STUTIL

//******************************************************************
// End of file
//******************************************************************
#endif // STUTIL_MISC_H

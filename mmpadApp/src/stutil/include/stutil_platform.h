//******************************************************************
/// @file stutil_platform.h
/// @brief Platform Specific definitions
/// @author      W.R. Lear
/// @date        04/20/2019
/// @copyright   Copyright (c) 2019 Sydor Technologies, 
///              All rights reserved.
///
//******************************************************************
#ifndef STUTIL_PLATFORM_H
#define STUTIL_PLATFORM_H

#ifdef _WIN32
    // Win10 or later
    #undef  _WIN32_WINNT
    #define _WIN32_WINNT 0x0A00
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
    #include <direct.h>
    #include <stdint.h>

    #define STRERROR_S(buf, buflen, errno) strerror_s(buf, buflen, errno)

    #ifndef S_ISDIR
        #define S_ISDIR(mode)  (((mode) & S_IFMT) == S_IFDIR)
    #endif

    #ifndef S_ISREG
        #define S_ISREG(mode)  (((mode) & S_IFMT) == S_IFREG)
    #endif

    #define SLEEP_MSEC(msec) Sleep(msec) 

    #define STRICMP(a,b) ::_stricmp(a,b)

#else
    #include <unistd.h>
    #include <sys/time.h>
    //-=-= FIXME: This does not work with the GNU version of strerror_r
    #define STRERROR_S(buf, buflen, errno) (void)strerror_r(errno, buf, buflen)

    #define SLEEP_MSEC(msec) usleep((msec)*1000)

    #define STRICMP(a,b) ::stricmp(a,b)
#endif

//******************************************************************
// End of file
//******************************************************************
#endif // STUTIL_PLATFORM_H

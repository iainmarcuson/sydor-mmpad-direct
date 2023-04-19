//******************************************************************
/// @file stutil_system.h
/// @brief System level utilities
/// @author      W.R. Lear
/// @date        04/20/2019
/// @copyright   Copyright (c) 2019 Sydor Technologies, 
///              All rights reserved.
///
//******************************************************************
#ifndef STUTIL_SYSTEM_H
#define STUTIL_SYSTEM_H

#include <stdint.h>
#include <string>
#include "stutil_platform.h"

#ifdef _WIN32
#else
#include <sys/time.h>
#endif

namespace STUTIL
{

//******************************************************************
// Definitions and Constants
//******************************************************************

//******************************************************************
// Data structures, enumerations and type definitions
//******************************************************************

typedef struct
{
    std::string CompanyName;
    std::string ComputerName;
    std::string Architecture;
    std::string Cores;
    std::string UserName;
    std::string HomeDir;
    std::string DocDir;
    std::string DataDir;
    std::string LogDir;
    std::string CfgDir;
    std::string TempDir;
    std::string WorkingDir;
    std::string OperatingSystem;
    std::string ProgramPath;
    std::string ProgramFileName;
    std::string ProgramBaseName;
    std::string ProgramDir;
    std::string BaseName;
    std::string OutDir;
} SystemInfo;

//******************************************************************
// System information
//******************************************************************

//------------------------------------------------------------------
/// Get Computer name
std::string getComputerName(void);

//------------------------------------------------------------------
/// Get User name
std::string getUserName(void);

//------------------------------------------------------------------
/// Get Operating System name
std::string getOperatingSystem(void);

//------------------------------------------------------------------
/// Get the current program path
std::string getProgramPath(void);
 
//------------------------------------------------------------------
/// Get system information
/// @param[in] basename     friendly program name for use in dir and file names
/// @param[in] companyname  optional company name for use in dir names
///
/// @returns SystemInfo struct  containing available system information
///
SystemInfo getSystemInfo(const std::string& basename = "", const std::string& companyname = "");

} // namespace STUTIL

//******************************************************************
// End of file
//******************************************************************
#endif // STUTIL_SYSTEM_H

//******************************************************************
/// @file stutil_hexdump.h
/// @brief Create a tabulated hex display of an array of data
/// @author      W.R. Lear
/// @date        12/21/2018
/// @copyright   2018 Sydor Technologies, All rights reserved.
///
/// This file contains a function to display an array of data
/// in tabulated hexadecimal format.
///
//******************************************************************
#ifndef STUTIL_HEXDUMP_H
#define STUTIL_HEXDUMP_H

#include <string>
#include <vector>

namespace STUTIL
{
    
//------------------------------------------------------------------
/// Get a string containing a hexadecimal listing of an array of values
///
/// @param addr         Address of first byte in data buffer
/// @param data         data buffer containing data to display
/// @param nbytes       number of bytes to display
/// @param size         size of fundamental data units in bytes (1,2,4 or 8). (default=4)
/// @param bytesperline Number of bytes to display in each row
/// @param showAscii    show ascii characters if true. (default=true)
/// @param showAddr     show address column if true (default = true)
///
/// @return string containing formatted data
///
std::string getHexDump(uint64_t addr, const void *data, uint32_t nbytes, uint32_t size = 4, 
                       uint32_t bytesperline = 16, bool showAscii = true,
                       bool showAddr = true);
                               
//------------------------------------------------------------------
/// Get a vector<string> containing a hexadecimal listing of an array of values
///
/// @param outv         vector<string> to receive formatted data
/// @param addr         Address of first byte in data buffer
/// @param data         data buffer containing data to display
/// @param nbytes       number of bytes to display
/// @param size         size of fundamental data units in bytes (1,2,4 or 8). (default=4)
/// @param bytesperline Number of bytes to display in each row
/// @param showAscii    show ascii characters if true. (default=true)
/// @param showAddr     show address column if true (default = true)
///
/// @return string containing formatted data
///

void getHexDump(std::vector<std::string>& outv,
                uint64_t addr, const void *data, uint32_t nbytes, uint32_t size = 4,
                uint32_t bytesperline = 16, bool showAscii = true, bool showAddr = true);


//------------------------------------------------------------------
/// Output a hexadecimal listing of an array of values to an output stream
///
/// @param out          Output stream to receive formatted data
/// @param addr         Address of first byte in data buffer
/// @param data         data buffer containing data to display
/// @param nbytes       number of bytes to display
/// @param size         size of fundamental data units in bytes (1,2,4 or 8). (default=4)
/// @param bytesperline Number of bytes to display in each row
/// @param showAscii    show ascii characters if true. (default=true)
/// @param showAddr     show address column if true (default = true)
///
void getHexDump(std::ostream& out,
                uint64_t addr, const void *data, uint32_t nbytes, uint32_t size = 4,
                uint32_t bytesperline = 16, bool showAscii = true, bool showAddr = true);
                               
} // namespace STUTIL
//******************************************************************
// End of file
//******************************************************************
#endif // STUTIL_HEXDUMP_H

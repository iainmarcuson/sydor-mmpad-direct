//******************************************************************
/// @file stutil_string.h
/// @brief String Utility Function library
/// @author      W.R. Lear
/// @date        06/10/2020
/// @copyright   Copyright (c) 2019,2020 Sydor Technologies, 
///              All rights reserved.
///
/// This file contains string related utility functions
///
//******************************************************************
#ifndef STUTIL_STRING_H
#define STUTIL_STRING_H

#include <string>
#include <vector>

namespace STUTIL
{

//******************************************************************
// Definitions and Constants
//******************************************************************
const int32_t STUTIL_FMT_STRING_MAX_LEN = 1024;      ///< Max formatted string length

//******************************************************************
// String Function Definitions
//******************************************************************

//------------------------------------------------------------------
/// Trim whitespace from start of a string
std::string ltrim(const std::string& s);

//------------------------------------------------------------------
/// Trim whitespace from end of a string
std::string rtrim(const std::string& s);

//------------------------------------------------------------------
/// Trim whitespace from end of a string
std::string trim(const std::string& s);

//------------------------------------------------------------------
/// Convert a string to lower case
/// @param[in] str              Input string
/// @returns lower case version of str
std::string toLower(const std::string& str);

//------------------------------------------------------------------
/// Return true if character is space or tab
bool isWhite(char c);

//------------------------------------------------------------------
/// return true if string starts with specified start string
/// @param[in] str              Input string
/// @param[in] start            Starting substring to check for
/// @returns true if str ends with end
///
bool startsWith(const std::string& str, const std::string& start);

//------------------------------------------------------------------
/// return true if string ends with specified ending
/// @param[in] str              Input string
/// @param[in] end              Ending substring to check for
/// @returns true if str ends with end
///
bool endsWith(const std::string& str, const std::string& end);

//------------------------------------------------------------------
/// Replace all occurrences of a substring with another string
/// @param[in] str             Input string
/// @param[in] oldStr          substring to replace
/// @param[in] newStr          new substring
///
/// @returns modified string
///
std::string strReplaceAll(const std::string& str,
    const std::string& oldStr,
    const std::string& newStr = "");

//-----------------------------------------------------------------
/// Get (up to) the last N characters of a string
///
/// @param[in] str      Input string
/// @param[in] n        Max number of characters to return
///
/// @returns requested substring
///
std::string rclip(const std::string& str, uint32_t n);

//-----------------------------------------------------------------
/// Get (up to) the first Ncharacters of a string
///
/// @param[in] str      Input string
/// @param[in] n        Max number of characters to return
///
/// @returns requested substring
///
std::string lclip(const std::string& str, uint32_t n);

//------------------------------------------------------------------
/// Replace first occurrence of a substring with another string
/// @param[in] str             Input string
/// @param[in] oldStr          substring to replace
/// @param[in] newStr          new substring
///
/// @returns modified string
///
std::string strReplaceFirst(const std::string& str,
    const std::string& oldStr,
    const std::string& newStr = "");

//------------------------------------------------------------------
/// Replace last occurrence of a substring with another string
/// @param[in] str             Input string
/// @param[in] oldStr          substring to replace
/// @param[in] newStr          new substring
///
/// @returns modified string
///
std::string strReplaceLast(const std::string& str,
    const std::string& oldStr,
    const std::string& newStr = "");

//------------------------------------------------------------------
/// Split a delimited string into tokens
/// @param[in] str              Input string
/// @param[out] tokens          vector to receive split tokens
/// @param[in] delim            delimiter character
///
/// @return number of tokens found
int32_t splitString(const std::string& str, 
                    std::vector<std::string>& tokens, 
                    char delim);


//------------------------------------------------------------------
// Format a string - sprintf for std::string
std::string formatString(const char* fmt, ...);

//------------------------------------------------------------------
// Format a string - vsprintf for std::string
std::string formatStringV(const char* fmt, va_list ap);

//------------------------------------------------------------------
/// get a range limited integer
///
/// @param[in] val     Input value
/// @param[in] hi      maximum allowable value
/// @param[in] lo      minimum allowable value (default=0)
///
/// @returns val, hi, or lo
///
int32_t getInt(int32_t val, int32_t hi, int32_t lo = 0);

//------------------------------------------------------------------
/// get a range limited unsigned integer
///
/// @param[in] val     Input value
/// @param[in] hi      maximum allowable value
/// @param[in] lo      minimum allowable value (default=0)
///
/// @returns val, hi, or lo
///
uint32_t getUInt(uint32_t val, uint32_t hi, uint32_t lo = 0);

//------------------------------------------------------------------
/// get a non-null string
///
/// @param[in] str     Input string
/// @param[in] def     Default str is null or empty
///
/// @returns str or def
///
std::string getStr(const char* str, const char* def = "");

//------------------------------------------------------------------
/// Get string representation of version number
///
/// @param[in] version         version number MMmmbb
///
/// @returns string containing formatted version (MMmmbb)
///
std::string getVersionStr(uint32_t version);

//------------------------------------------------------------------
/// Get a decimal string representation of an integer value
///
/// @param[in] val              value to convert to a string
///
std::string toDecString(int64_t val);

//------------------------------------------------------------------
/// Get a decimal string representation of a double with specified precision
///
/// @param[in] val              value to convert to a string
/// @param[in] decimalplaces    number of decimal places to show
///
std::string toDecString(double val, int32_t decimalplaces);

//------------------------------------------------------------------
/// Get a decimal string representation of an unsigned integer value with leading zeros
///
/// @param[in] val              value to convert to a string
/// @param[in] ndigits          number of digits to show
///
std::string toUDecString(uint64_t val, int32_t ndigits);

//------------------------------------------------------------------
/// Get a decimal string representation of an unsigned integer value
///
/// @param[in] val              value to convert to a string
///
std::string toUDecString(uint64_t val);

//------------------------------------------------------------------
/// Convert unsigned value to a decimal string with K,M,G suffix
///
/// @param[in] val              value to convert to a string
///
std::string toUDecKMGString(uint64_t val);

//------------------------------------------------------------------
/// Get a decimal string representation of a boolean value
///
/// @param[in] val              value to convert to a string
///
std::string toBoolString(bool val);

//------------------------------------------------------------------
/// Get a hexadecimal string representation of an unsigned integer value
///
/// @param[in] val              value to convert to a string
/// @param[in] nbytes           number of hex bytes to show
/// @param[in] prefix           prefix to add to start of hex value (e.g. '0x')
///
std::string toHexString(uint64_t val, int32_t nbytes, const char* prefix="");

//------------------------------------------------------------------
/// Convert a string to an integer.
///  Converts a string to a 64-bit signed integer.
///  Radix can be explicitly defined in the string:<br>
///  'x' or '0x'= hex, 'n'=decimal, '_'=binary, 'o'=octal.<br>
///  if radix is not explicitly defined, the default radix is used.
///  If the string is not a valid integer for the specified radix,
///  or an invalid radix is specified, the default value is returned.
///
/// @param[in] str         String to convert
/// @param[in] def         Default value (default=0)
/// @param[in] defRadix    Default radix (default=10)
/// @param[out] pRadix     Optional Pointer to receive actual radix
///                        (radix is 0 for any conversion error)
///
/// @returns converted value or default
///
int64_t toInt64(const std::string& str, int64_t def=0,
                int32_t defRadix = 10, int32_t* pRadix=nullptr);

uint64_t toUInt64(const std::string& str, uint64_t def,
                  int32_t defRadix = 10, int32_t* pRadix=nullptr);

int32_t toInt32(const std::string& str, int32_t def=0,
                int32_t defRadix = 10, int32_t* pRadix=nullptr);

uint32_t toUInt32(const std::string& str, uint32_t def=0,
                  int32_t defRadix = 10, int32_t* pRadix=nullptr);

int16_t toInt16(const std::string& str, int16_t def = 0,
                int32_t defRadix = 10, int32_t* pRadix = nullptr);

uint16_t toUInt16(const std::string& str, uint16_t def=0,
                  int32_t defRadix = 10, int32_t* pRadix=nullptr);

uint8_t toUInt8(const std::string& str, uint8_t def=0,
                int32_t defRadix = 10, int32_t* pRadix=nullptr);


//------------------------------------------------------------------
/// Convert a string to a 64-bit floating point value
/// @param[in] str         String to convert
/// @param[in] def         Default value (default=0)
/// @param[in] pOk         Optional pointer to receive conversion status
///                        (true = ok)
///
/// @returns converted value or default
///
double toDouble(const std::string& str, double def = 0.0,
                 bool* pOk = nullptr);

//------------------------------------------------------------------
///  Convert a string to a boolean value.
///
///  true values:
///  "1", "true", "yes", "enable", "on"
///  false values:
///  "0", "false", "no", "disable", "off"
///
/// All other values return the default value,
///  and sets the ok parameter to false (if supplied).
///
/// @param[in] str         String to convert
/// @param[in] def         Default value (default=0)
/// @param[in] pOk         Optional pointer to receive conversion status
///                        (true = ok)
///
/// @returns converted value or default
///
bool toBoolean(const std::string& str, bool def=false, bool* pOk=nullptr);

//------------------------------------------------------------------
/// Convert an element of a string vector to an boolean.
///
/// @param[in] strVect     vector of strings
/// @param[in] index       index of string to convert
/// @param[in] def         Default value (default=0)
/// @param[in] pOk         Optional pointer to receive conversion status
///                        (true = ok)
///
/// @returns converted value or default
///
bool toBoolean(const std::vector<std::string>& strVect, uint32_t index, bool def=false, bool* pOk=nullptr);

//------------------------------------------------------------------
/// Convert a dotted version or hex string to a 32-bit unsigned integer
/// 
/// String can be "", "M", "M.m", "M.m.b", or "M.m.b.p" or 0xMMmmbbpp
/// 
/// Each component must be in the range 0-255.
/// M will be placed in byte 3 of the returned value, m in byte 2, b in byte 1, p in byte 0.
/// Missing components will be set to 0.
///
/// @param[in] str         String to convert
/// @param[in] def         Default value if error (default=0)
/// @param[in] pOk         Optional pointer to receive conversion status
///                        (true = ok)
///
/// @returns converted value or default
///
uint32_t toVersion(const std::string& str, uint32_t def = 0, bool* pOk = nullptr);


//------------------------------------------------------------------
/// Return the escaped representation of an unescaped character
/// Returns 0 if character does not need to be escaped
char escapeChar(char c);

//------------------------------------------------------------------
/// Return the unescaped version of an escaped character
char unescapeChar(char c);

//----------------------------------------------
/// Escape special characters in a string
///
/// @param str           String contaning unescaped characters
///
/// @returns             String with special characters replaced by escape sequences
///
std::string escapeString(const std::string& str);

//----------------------------------------------
/// Unescape special characters in a string
///
/// @param str           String contaning escaped character sequences
///
/// @returns             String with escape sequences replaced by unescaped characters
///
std::string unescapeString(const std::string& str);

//----------------------------------------------
/// Get a string representation of a mSec time interval
///
/// @param[in] mSec     number of milliseconds
/// 
/// @return string equivalent as HH:MM:SS.mmm
/// 
std::string mSecToString(uint64_t mSec);


}   // namespace STUTIL

//******************************************************************
// End of file
//******************************************************************
#endif // STUTIL_STRING_H

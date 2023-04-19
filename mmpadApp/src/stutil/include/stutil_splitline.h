//******************************************************************
/// @file stutil_splitline.h
/// @brief Split a delimited line into fields
/// @author      W.R. Lear
/// @date        06/10/2020
/// @copyright   Copyright (c) 2020 Sydor Technologies, 
///              All rights reserved.
///
/// Utility function to split a delimeted line into
/// a list of field strings
///
/// The field delimiter can be specified to be a comma (','),
/// or white space (' ', '\t').
///
/// Fields may be quoted with double quotation marks ('"'). Quotation
/// marks are removed from the return field.
///
/// Any characters may be escaped by preceding the character with a
/// backslash ('\'). 
///
/// Leading and trailing whitespace is trimmed from each returned field.
///
//******************************************************************
#ifndef STUTIL_SPLIT_LINE_H
#define STUTIL_SPLIT_LINE_H

#include <string>
#include <vector>

namespace STUTIL
{

//******************************************************************
// Definitions and Constants
//******************************************************************

//******************************************************************
// Data structures, enumerations and type definitions
//******************************************************************

//******************************************************************
// Function declarations
//******************************************************************

//------------------------------------------------------------------
/// Split a comma or whitespace separated line string into a list of 
/// field strings.
///
/// @param[in] line         string to be parsed and split into fields
/// @param[out] fields      vector to hold returned field strings
/// @param[in] commaIsSep   true if separator is ','. false for whitespace
/// @param[out] pErrorChar  optional variable to return character index 
///                         where an error was detected.
///
/// @return 0 if ok, negative error code on any error 
///
int32_t splitLine(const std::string& line, 
                        std::vector<std::string>& fields, 
                        bool commaIsSep,
                        uint32_t* pErrorChar);

//------------------------------------------------------------------
/// Split a file containing comma or whitespace separated lines
/// into a list containing a list of field strings for each line
///
/// @param[in]  filePath    relative or absolute file path to be read
/// @param[out] lineFields  vector to hold returned field strings
/// @param[in]  commaIsSep  true if separator is ','. false for whitespace
/// @param[out] errorLine   optional variable to return line number
///                         in which an error occurred,
/// @param[out] errorChar   optional variable to return character index
///                         where an error was detected
///
/// @return 0 if ok, negative error code on any error 
///
int32_t splitFile(const std::string& filePath,
    std::vector <std::vector<std::string>>& lineFields,
    bool commaIsSep,
    uint32_t* pErrorLine,
    uint32_t* pErrorChar);

}  // STUTIL
//******************************************************************
// End of file
//******************************************************************
#endif //STUTIL_SPLIT_LINE_H

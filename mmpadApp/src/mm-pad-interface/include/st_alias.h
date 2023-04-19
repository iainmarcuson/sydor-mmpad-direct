//*******************************************************************
/// @file st_alias.h
/// Sydor Data Store alias class, common to client and server 
///
/// @author      W.R. Lear
/// @date        03/06/2020
/// @copyright   Copyright (c) 2019, 2020 Sydor Technologies, all rights reserved.
///
/// This C++ class represents one alias definition in the the Sydor
/// Data Store.
///
//*******************************************************************
#ifndef STALIAS_H
#define STALIAS_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <map>
#include <mutex>
#include "nlohmann_json_fwd.hpp"
#include "st_if_defs.h"
#include "st_errors.h"
#include "stutil_logger.h"

//******************************************************************
// Forward Declarations
//
namespace STUTIL
{
    class Logger;
}

namespace ST_INTERFACE
{
    
//******************************************************************
/// @name Definitions and Constants
///@{


//----------------------------------------------
/// Alias Property Key enumeration
typedef enum
{
    AL_ID = 0,          ///< Unique ID
    AL_PROTECTED,       ///< True if this is alias cannot be deleted by client
    AL_DEFINITION       ///< Alias definition string
} AliasKey_t;


///@} end of definitions and typedefs

//******************************************************************
/// @name Parameter Property Key Nams
///@{

// Parameter  Property Key Names
#define AL_KEY_ID           "Id"            ///< Unique ID
#define AL_KEY_PROTECTED    "Protected"     ///< True if this alias cannot be deleted
#define AL_KEY_DEFINITION   "Definition"    ///< Alias definition string

///@} Alias Property Key Names


//******************************************************************
/// @name Class definitions
///@{

//------------------------------------------------------------------
/// Parameter Definition
class StAlias
{
private:
    static const std::map<std::string, AliasKey_t> sPropertyKeys; /// key name to enum map

    STUTIL::Logger *pLog;               ///< Logger instance

    // Common properties
    std::string     mId;                ///< Alias Id - unique
    std::string     mDefinition;        ///< Alias definition string
    bool            mProtected;         ///< true if alias cannot be deleted

public:
         
    //----------------------------------------------
    /// Default Constructor
    StAlias(const std::string& id = "", nlohmann::json *pJson = nullptr);
   
    //----------------------------------------------
    /// Convenience Constructor
    StAlias(const std::string& id, 
            const std::string& definition, 
            bool isProtected = false);
    
    //----------------------------------------------
    /// Destructor
    ~StAlias(void);

    //----------------------------------------------
    /// Return true if param is readable
    bool isProtected(void) const { return mProtected; }


    //----------------------------------------------
    /// get the number of Access enumeration values
    const std::string getDefinition(void) const {return mDefinition; }
 

    //----------------------------------------------
    /// De-serialize from a JSON object
    int32_t fromJson(const nlohmann::json& j);
    
    //----------------------------------------------
    /// Serialize to a JSON object
    /// 
    /// @param[out] j            JSON object to receive result
    void toJson(nlohmann::json& j) const;

    //----------------------------------------------
    /// Serialize to a JSON string
    /// 
    /// @param[out] jsonStr      string to receive result
    void toJson(std::string& jsonStr) const;

};

///@} end of class definitions

} // namespace ST_INTERFACE

//******************************************************************
// End of file
//******************************************************************

#endif //STALIAS_H

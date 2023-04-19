//******************************************************************
/// @file st_clientlist.h
/// @brief Class containing information about all clients connected to an XPAD server
/// @author      W.R. Lear
/// @date        2022/04/26
/// @copyright   2022 Sydor Technologies, All rights reserved.
///
/// This file defines the StClientList class which holds information
/// about all clients currently connected to a Sydor X-PAD Server
///
//******************************************************************
#ifndef ST_CLIENTLIST_H
#define ST_CLIENTLIST_H

#include <stdint.h>
#include <string>
#include "st_if_defs.h"
#include "stutil_string.h"
#include "stutil_logger.h"
#include "nlohmann_json.hpp"

namespace ST_INTERFACE
{


//******************************************************************
/// StClientDef class
///
/// This class contains all information for a single capture run
/// in the Data Index
///
class StClientDef
{
protected:
    STUTIL::Logger* pLog;
    uint32_t mClientId;
    std::string mUserName;
    std::string mComputerName;
    std::string mOperatingSystem;
    uint64_t    mLastMessageTime;

public:

    //----------------------------------------------
    /// Constructor
    StClientDef(void);
    
    //----------------------------------------------
    /// Destructor 
    ~StClientDef();

    //----------------------------------------------
    /// get client id
    uint32_t getClientId(void) const { return mClientId; }

    //----------------------------------------------
    /// set client id
    void setClientId(uint32_t id) { mClientId = id; }

    //----------------------------------------------
    /// get user name
    std::string getUserName(void) const { return mUserName; }

    //----------------------------------------------
    /// set user name
    void setUserName(const std::string& name) { mUserName = name; }

    //----------------------------------------------
    /// get computer name
    std::string getComputerName(void) const { return mComputerName; }

    //----------------------------------------------
    /// set computer name
    void setComputerName(const std::string& name) { mComputerName = name; }

    //----------------------------------------------
    /// get operating system name
    std::string getOperatingSystem(void) const { return mOperatingSystem; }

    //----------------------------------------------
    /// set operating system name
    void setOperatingSystem(const std::string& name) { mOperatingSystem = name; }

    //----------------------------------------------
    /// get last message time
    uint64_t getLastMessageTime(void) const { return mLastMessageTime; }

    //----------------------------------------------
    /// set last message time
    void setLastMessageTime(uint64_t value) { mLastMessageTime = value; }
   
    //----------------------------------------------
    /// De-serialize from a JSON string
    int32_t fromJson(const std::string& jsonStr);

    //----------------------------------------------
    /// De-serialize from a JSON object
    int32_t fromJson(const nlohmann::json& j);
    
    //----------------------------------------------
    /// Serialize to a JSON object
    void toJson(nlohmann::json& j) const;

    //----------------------------------------------
    /// Serialize to a JSON string
    void toJson(std::string jsonStr, int32_t indent = 4) const;

}; // Class StClientDef

  
//******************************************************************
/// StClientList class
class StClientList
{
protected:
    STUTIL::Logger* pLog;
    uint64_t mTimeStamp;
    std::vector<StClientDef> mClients;
  
public:
    //----------------------------------------------
    /// Constructor
    StClientList(void);
    
    //----------------------------------------------
    /// Destructor 
    ~StClientList();

    //----------------------------------------------
    // Clear the content
    void clear(void);

    //----------------------------------------------
    /// get set count
    uint32_t getClientCount(void) const { return static_cast<uint32_t>(mClients.size()); }

    //----------------------------------------------
    /// add a set definition to set
    void addClient(const StClientDef& clientdef);

    //----------------------------------------------
    /// get the collection of all sets
    std::vector<StClientDef> getClients(void) const { return mClients; }

    //----------------------------------------------
    /// get the specified client
    StClientDef getClient(uint32_t index) const;
 
    //----------------------------------------------
    /// De-serialize from a JSON object
    ///
    /// @param[in] j     JSON object
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    int32_t fromJson(const nlohmann::json& j);
    
    //----------------------------------------------
    /// De-serialize from a JSON string
    int32_t fromJson(const std::string& jsonStr);

    //----------------------------------------------
    /// De-serialize from a JSON string
    int32_t fromJson(const char* jsonStr) { return fromJson(std::string(jsonStr)); }

    //----------------------------------------------
    /// Serialize to a JSON object
    void toJson(nlohmann::json& j) const;  

    //----------------------------------------------
    /// Serialize to a JSON string
    void toJson(std::string& jsonStr, int32_t indent = 4) const;

private:

};  // class StClientList

} // namespace ST_INTERFACE
//******************************************************************
// End of file
//******************************************************************
#endif // ST_CLIENTLIST_H

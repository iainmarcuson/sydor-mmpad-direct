//*******************************************************************
/// @file st_servers.h
/// @brief Collection of Sydor Server interface objects
/// @author      W.R. Lear
/// @date        04/19/2019
/// @copyright   2019 Sydor Technologies, All rights reserved.
///
/// This file defines a container class holding a collection of
/// StClientInterface instances
///
//*******************************************************************
#ifndef ST_SERVERS_H
#define ST_SERVERS_H

#include <string>
#include <map>
#include <vector>
#include <mutex>
#include "st_if_defs.h"

//******************************************************************
// Forward Declarations
//******************************************************************
namespace ST_INTERFACE
{
    class StClientInterface;
}

namespace STUTIL
{
    class Logger;
}

//******************************************************************
// Definitions and Constants
//******************************************************************

namespace ST_INTERFACE
{
    //----------------------------------------------
    // map of handles to server interface instances
    typedef std::map<int32_t, StClientInterface*> ServerMap;
    typedef std::map<int32_t, StClientInterface*>::iterator ServerMapIterator;

}

//******************************************************************
// Class Definition
//******************************************************************
namespace ST_INTERFACE
{

class StServers
{
protected:
    //-=-= FIXME!!: These server info definitions need to be replaced with a config file
    static const STServerInfo sLocalHostInfo;
    static const STServerInfo sServer1Info;
    static const STServerInfo sServer2Info;
    static const STServerInfo sServer3Info;
    STUTIL::Logger *pLog;
    uint32_t mOptionFlags;
    std::vector<STServerInfo> mServerList;
    ServerMap mServers;
    int32_t mNextHandle;
    std::string mServerListFilePath;

public:

    //----------------------------------------------
    /// Mutex to prevent closing interface while in use
    ///
    /// @note: this is intended to be an interim solution
    ///        until getServer() is modified to return a
    ///        pointer wrapper class that automatically
    ///        acquires/releases the lock
    ///
    static std::recursive_mutex sLockCS;

    //----------------------------------------------
    /// Constructor
    StServers( uint32_t optionflags = 0);
    
    //----------------------------------------------
    /// Destructor
    ~StServers();

    //----------------------------------------------
    /// Initialize everything
    ///
    /// @param[in] options      Option flags to pass to each interface
    ///
    void init(uint32_t options = 0);

    //----------------------------------------------
    /// Set the optional Server list file path
    void setServerListFilePath(const std::string& path) { mServerListFilePath = path; }

    //----------------------------------------------
    /// Return a server info struct useable as a template for locateServers()
    ///
    /// @param[in] name             // string to match in name
    /// @param[in] type             // string to match in type
    /// @param[in] location         // string to match in location
    /// @param[in] description      // string to match in description
    static STServerInfo getServerTemplate( const std::string& name = "",
                                    const std::string& type = "",
                                    const std::string& location = "",
                                    const std::string& description = "");

    //----------------------------------------------
    /// Query network for servers matching specified characteristics
    ///
    /// @param[in] serverTemplate   desired server characteristics
    /// 
    /// @return 0 on success or negative error code
    ///
    int32_t locateServers(const STServerInfo& serverTemplate);

    //----------------------------------------------
    /// Get the number of servers found
    ///
    uint32_t getFoundServerCount(void) { return static_cast<uint32_t>(mServerList.size()); }

    //----------------------------------------------
    /// Retrieve the list of servers responding to locateServers()
    ///
    /// @param[out] servers          collection of server info matching template
    /// 
    /// @return number of matching servers found
    ///
    uint32_t getServerList(std::vector<STServerInfo>& servers);

    //------------------------------------------------------------------
    /// Open a server and add it to the collection of active servers
    ///
    /// @param[in] serverInfo    reference to info of server to open
    ///
    /// @return handle for added servo, or negative error code
    ///
    int32_t openServer(const STServerInfo& serverInfo);

    //------------------------------------------------------------------
    /// Close a server and remove from the collection
    ///
    /// @param[in] handle    handle of server to delete
    ///
    /// @return 0 if ok, or negative error code
    ///
    int32_t closeServer(int32_t handle);

    //------------------------------------------------------------------
    /// Close a server and remove it from the collection of active servers
    ///
    /// @param[in] serverIndex    index to server info list
    ///
    /// @return handle for added servo, or negative error code
    ///
    int32_t closeServer(uint32_t serverIndex);

    //------------------------------------------------------------------
    /// Close a server and remove from the collection
    ///
    /// @param[in] serverInfo    reference to info of server to open
    ///
    /// @return 0 if ok, or negative error code
    ///
    int32_t closeServer(const STServerInfo& serverInfo);

    //------------------------------------------------------------------
    /// Close a server and remove from the collection
    ///
    /// @param[in] pServer   Pointer to the open client interface
    ///
    /// @return 0 if ok, or negative error code
    ///
    int32_t closeServer(StClientInterface* pServer);

    //------------------------------------------------------------------
    /// Close all open servers
    ///
    /// @return 0 if ok, or negative error code
    ///
    int32_t closeAll(void);

    //------------------------------------------------------------------
    /// Get a server interface from the collection
    ///
    /// @param[in] handle    handle of server to retrieve
    ///
    /// @return pointer to interface, or nullptr
    ///
    StClientInterface* getServer(int32_t handle);

    //------------------------------------------------------------------
    /// Get the handle of the server with the specified host name or ip address
    /// 
    /// @param[in] hostStr    Host name or ip address string
    /// 
    /// @return handle or negative error code if not found
    ///
    int32_t getHandle(const char* hostStr);

    //------------------------------------------------------------------
    /// Get the handle of the specified server interface instance
    /// 
    /// @param[in] pServer    pointer to server interface
    /// 
    /// @return handle or negative error code if not found
    ///
    int32_t getHandle(const StClientInterface* pServer);

protected:
    //------------------------------------------------------------------
    /// Add a server interface to the collection
    ///
    /// @param[in] serverInfo    reference to info of server to add
    ///
    /// @return handle for added servo, or negative error code
    ///
    int32_t addServer(const STServerInfo& serverInfo);

    //------------------------------------------------------------------
    /// Delete a server interface from the collection
    ///
    /// @param[in] handle    handle of server to delete
    ///
    /// @return 0 if ok, or negative error code
    ///
    int32_t deleteServer(int32_t handle);

    //------------------------------------------------------------------
    /// Delete a server interface from the collection of open servers
    /// @param[in] pServer    pointer to client interface to be deleted
    ///
    /// @return 0 if ok, or negative error code
    ///
    int32_t deleteServer(StClientInterface* pServer);

};

} //namespace ST_SERVER_IF

//******************************************************************
// End of file
//******************************************************************
#endif //ST_SERVERS_H

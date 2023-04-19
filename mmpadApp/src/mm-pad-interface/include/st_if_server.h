//*******************************************************************
/// @file st_if_server.h
/// Sydor Client Interface Server-side API Library.
///
/// @author      W.R. Lear
/// @date        07/14/2019
/// @copyright   Copyright (c) 2019 Sydor Technologies, all rights reserved.
///
/// This header file defines a Linux compatible library of C++ classes,
/// data structures, and definitions that implements the server interface 
/// to the Sydor Network link.
///
//*******************************************************************
#ifndef ST_IF_SERVER_H
#define ST_IF_SERVER_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <thread>
#include <mutex>
#include <string>
#include <sstream>
#include <ostream>
#include "st_if_defs.h"
#include "st_response_handler.h"
#include "st_client_info.h"
#include "st_message.h"
#include "stutil_system.h"
#include "st_datastore.h"
#include "st_doublebuf.h"
#include "st_framebuffer.h"

//*******************************************************************
// Doxygen Main Page
//*******************************************************************
/// @mainpage Sydor Server Interface Library
/// @copyright Copyright (c) 2019 Sydor Technologies,
///            All rights reserved.
///
/// @section if_srvlib_intro_sect Introduction
/// The Sydor Server Interface Library
/// implements a C++ application programming interface (API) that allows 
/// Server resident application(s) to interact with TCP/IP network connected 
/// client applications via the Client Interface protocols.
///
/// @section if_srvlib_implementation_sect Implementation
/// The Sydor Server Interface library consists of 
/// a collection of C++11 classes, data structures and definitions
/// 
/// This library is a static library for 64-bit Linux platforms. 
///
/// @section if_srvlib_overview_sect API Function Overview
/// The classes exported by the Server Interface Library can be loosely grouped 
/// into seven categories:
/// - Discovery:  participation in client discovery of accessible Servers
/// - Connection: managing connections to Clients
/// - Coordination: managing access permissions of multiple clients
/// - Configuration: preparing for a capture run by defining ettings
/// - Control/Status: runtime interaction with connected clients during a run
/// - Image Data Transfer: efficiently transferring frame data to a client following
///   a capture run
/// - Utility Functions: convenience functions of use in typical API use cases
///
/// @section if_srvlib_defs_sect Library Definitions
/// Many of the Server Interface library methods use API specific data structures, 
/// enumerations, definitions, and typedefs. These can be grouped into five categories:
/// - Typedefs, Structures and enumerations : Used to pass data to/from API methods.
/// - Return Status Codes : API functions return consistent status codes if possible
/// - Manifest constants : Definitions of various limits and data ranges
/// - Log Message Levels : Definitions of debug log message severity levels.
///


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
/// @name Server Interface Version
/// @{
#define ST_SERVER_IF_MAJOR  (3)     ///< Library major version
#define ST_SERVER_IF_MINOR  (10)     ///< Library minor version
#define ST_SERVER_IF_BUILD  (0)     ///< Library build
#define ST_SERVER_IF_PATCH  (0)     ///< Library patch

/// Full Version MMmmBBPP
#define ST_SERVER_IF_VERSION   ((ST_SERVER_IF_MAJOR<<24) | \
                                (ST_SERVER_IF_MINOR<<16) | \
                                (ST_SERVER_IF_BUILD<<8 ) | \
                                (ST_SERVER_IF_PATCH))
///@} end of version definition

//******************************************************************
/// @name Definitions and Constants
///@{

#define SERDES_ALIGNMENT_TIMEOUT_MSEC 200
const uint32_t COMM_THREAD_START_MSEC  = 3000;  ///< Default max time to wait for thread to start
const uint32_t COMM_THREAD_STOP_MSEC   = 3000;  ///< Default max time to wait for thread to stop

// Arbitrary "addresses" must match JSON param file
// Per-client
#define ST_ADDR_FRAME_OPTIONS            0x0000     ///< Frame correction option flags register
#define ST_ADDR_FRAME_SKIP               0x0001     ///< Background frames to skip count register
#define ST_ADDR_DEBOUNCE_METHOD          0x0002     ///< Debounce method register
#define ST_ADDR_BATCH_CORRECT_BUSY       0x0003     ///< Batch correct busy flag
    
// Per-server
#define ST_ADDR_SYSTEM_TYPE              0x0100     ///< System type
#define ST_ADDR_SUBFRAME_COUNT           0x0101     ///< Subframe (aka quartus) count
#define ST_ADDR_SENSORS_PER_SUBFRAME     0x0102     ///< Number of sensor ASICs per subframe
#define ST_ADDR_SENSOR_COUNT             0x0103     ///< Total number of sensor ASICs
#define ST_ADDR_TELEM_PARAM_COUNT        0x0104     ///< Number of telemetry parameter values per sensor
#define ST_ADDR_LOG_LEVEL                0x0105     ///< Server debug log level
#define ST_ADDR_CONSOLE_LOG_LEVEL        0x0106     ///< Server console log level

///@} end of definitions and constants

//******************************************************************
/// @name Typedefs, Structures and Enums
/// Data types used to pass information to/from library functions
///@{

//----------------------------------------------
/// Map to get client info from id
typedef std::map < int32_t, StClientInfo> MMClientMap;

///@} end of typedefs

//******************************************************************
/// @name Class definitions
///@{
    
//------------------------------------------------------------------
/// ServerInterface class
///
/// This is a singleton class that allows the Sydor server
/// software to communicate with network connected client
/// applications.
///
class ServerInterface
{
private:
    // Class wide 
    STUTIL::Logger *pLog;                       ///< Debug logger instance
    ResponseHandler* mPResponseHandler;         ///< Pointer to response handler class
    StDataStore mDataStore;                     ///< Data Dictionary and parameter cache
    std::string mDataDictionaryPath;            ///< path to the data dictionary file

    DoubleBuf<std::vector<uint16_t>> mTelemetry;///< Current telemetry data
    uint64_t mTelemetryTimeStamp;               ///< Timestamp of current telemetry
    DoubleBuf<StFrameBuffer> mSampleFrame;      ///< Current sample frame

    uint64_t mSampleFrameTimeStamp;             ///< Timestamp of current sample frame
    bool mSampleFrameReceived;                  ///< True if a sample frame has been received for the current run
    STRunStatus mCurRunStatus;                  ///< Information about current (or most recent capture run)
    STSystemType mSystemType;                   ///< System type
    bool mIsSimulator;                          ///< True if the response handler is a simulator
    uint32_t mQuartusCount;                     ///< Number of Quartus channels in this system
    std::string mCurSetName;                    ///< Currently active set name
    uint32_t mServerVersion;                    ///< Realtime supervisor version

    std::map<uint32_t, uint32_t> mLocalParams;  ///< Local parameters
    
    // Client Connections
    int32_t mNextHandle;                        ///< next available client handle
    int32_t mCurClientHandle;                   ///< Current client handle
    MMClientMap mClientMap;                     ///< collection of connected clients
    int32_t mWriteClient;                       ///< handle of client with write privilege (-1 if none)
    int32_t mDeleteClient;                      ///< handle of client with delete privilege (-1 if none)
    StClientInfo *mPCurClientInfo;              ///< Current client info
	uint32_t mDefaultFrameOptions;              ///< Default frame correction settings
    uint32_t mDefaultBgSkipFrames;              ///< Default frames to skip
    uint32_t mDefaultDebounceMethod;            ///< Default debouncing method
    uint32_t mBDefaultBatchCorrectBusy;          ///< Default status of Batch Correct Busy flag

    // Message handling
    StMessage mCurMessage;                      ///< Current client message
    std::recursive_mutex mMsgCS;                ///< Mutex to serialize message access (temporary)

    // Communication Thread
    std::thread* mPCommThread;                  ///< Communication thread
    bool mCommThreadRunning;                    ///< true if communication thread is running
    bool mCommThreadStopRequested;              ///< true if commmunication thread stop request is pending
    int32_t mCommThreadRtn;                     ///< Communication thread return value.
    void* mCommContext;                         ///< Client communications context
    void* mCommSocket;                          ///< Client communications socket

    //----------------------------------------------
    /// Delete a client context
    ///
    /// @param[in] handle       Handle of client to delete
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    int32_t deleteClient(int32_t handle);

public:

    //----------------------------------------------
    /// Constructor 
    ServerInterface(); 

    //----------------------------------------------
    /// Destructor
    ~ServerInterface();

    //----------------------------------------------
    /// Get the library version
    uint32_t getLibVersion(void) { return ST_SERVER_IF_VERSION; }

    //----------------------------------------------
    /// Get the Realtime supervisor version
    uint32_t getServerVersion(void) { return mServerVersion; }

    //----------------------------------------------
    /// Get  pointer to the singleton class instance
    ///
    /// @param[in] pHandler      Pointer to ResponseHandler class instance
    /// @param[in] version       Server version (0xMMmmbbpp)
    /// @param[in] systemType    System type
    /// @param[in] isSimulator   true if the response handler class is a simulator
    /// 
    /// @return pointer to singleton instance, or nullptr on error
    ///
    int32_t setResponseHandler(ResponseHandler* pHandler, 
                               uint32_t version, 
                               STSystemType systemType,
                               bool isSimulator);

    //----------------------------------------------
    /// Close all client connections
    ///
    void closeAllClients(void);

    //----------------------------------------------
    /// Enable/Disable the server interface
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    int32_t enable(bool enable = true);

    //----------------------------------------------
    /// Set the next available sample frame
    ///
    /// @param frame     reference to the sample frame
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    int32_t setSampleFrame(ST_INTERFACE::StFrameBuffer &frame);

    //----------------------------------------------
    /// Set the next available sensor telemetry data
    ///
    /// @param telData     reference to a vector of telemetry data
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    int32_t setTelemetry(const std::vector<uint16_t> &telemetryData);

    //----------------------------------------------
    /// Get a pointer to the current set of sensor telemetry data
    ///
    /// @return pointer to the telemetry data
    ///
    std::vector<uint16_t> *getTelemetryPtr(void) {return mTelemetry.getOutputPtr();};

    //----------------------------------------------
    /// Read one or more values from an array parameter
    int32_t readParamArray(const std::string& paramId,
        std::vector<double>& values, uint32_t index, uint32_t count,
        uint32_t padIndex, int32_t rtnIn);


    //----------------------------------------------
    /// Convenience method to read a double parameter
    int32_t readParam(const std::string& paramId,
                      double& scaledValue, 
                      uint32_t index = 0, 
                      uint32_t padIndex = DEF_SUBFRAME_COUNT,
                      int32_t rtnIn = 0);

    //----------------------------------------------
    /// Convenience method to read an unsigned integer parameter
    int32_t readParam(const std::string& paramId,
                      uint32_t& uintValue,
                      uint32_t index = 0,
                      uint32_t padIndex = DEF_SUBFRAME_COUNT,
                      int32_t rtnIn = 0);

    //----------------------------------------------
    /// Convenience method to read a boolean parameter
    int32_t readParam(const std::string& paramId,
                      bool& boolValue,
                      uint32_t index = 0,
                      uint32_t padIndex = DEF_SUBFRAME_COUNT,
                      int32_t rtnIn = 0);

    //----------------------------------------------
    /// Convenience method to write a double parameter
    int32_t writeParam(StParameter* pParam,
                       double scaledValue,
                       uint32_t index = 0,
                       uint32_t padIndex = DEF_SUBFRAME_COUNT,
                       int32_t rtnIn = 0);

    //----------------------------------------------
    /// Convenience method to write a double parameter
    int32_t writeParam(const std::string& paramId,
                       double scaledValue,
                       uint32_t index = 0,
                       uint32_t padIndex = DEF_SUBFRAME_COUNT,
                       int32_t rtnIn = 0);

    //----------------------------------------------
    /// Convenience method to write an unsigned integer parameter
    int32_t writeParam(const std::string& paramId,
                       uint32_t uintValue,
                       uint32_t index = 0,
                       uint32_t padIndex = DEF_SUBFRAME_COUNT,
                       int32_t rtnIn = 0);

    //----------------------------------------------
    /// Convenience method to write a boolean parameter
    int32_t writeParam(const std::string& paramId,
                       bool boolValue,
                       uint32_t index = 0,
                       uint32_t padIndex = DEF_SUBFRAME_COUNT,
                       int32_t rtnIn = 0);

    //----------------------------------------------
    /// Convenience method to write the default value to a parameter
    int32_t writeParamDefault(const std::string& paramId,
                              uint32_t index = 0,
                              uint32_t padIndex = DEF_SUBFRAME_COUNT,
                              int32_t rtnIn = 0);

private:

    //----------------------------------------------
    /// Start the Communications thread
    ///
    int32_t startCommThread(uint32_t waitMSec = COMM_THREAD_START_MSEC);

    //----------------------------------------------
    /// Stop the Communications thread
    ///
    int32_t stopCommThread(uint32_t waitMSec = COMM_THREAD_STOP_MSEC);

    //----------------------------------------------
    /// Communications Thread body
    ///
    /// The communications thread manages all of the network
    /// level communications between the server and clients
    ///
    void commThread(void);

    //----------------------------------------------
    /// Cleanup Communications thread
    ///
    /// @param[in] rtnIn      Incoming return code
    ///
    /// @return 0 if OK, or negative error code
    ///
    int32_t cleanupCommThread(int32_t rtnIn);

    //----------------------------------------------
    /// Start discovery
    ///
    int32_t startDiscovery(void);

    //----------------------------------------------
    /// Stop discovery
    ///
    int32_t stopDiscovery(void);

    //----------------------------------------------
    /// Look up the client info for the specified client Id
    ///
    /// @return pointer to client info, or nullptr if not found
    ///
    StClientInfo* getClientInfo(int32_t id);

    //----------------------------------------------
    /// Enable write privilege for the specified client
    int32_t enableWrite(int32_t handle, bool force = false);

    //----------------------------------------------
    /// Disable write privilege for the specified client
    void disableWrite(int32_t handle);

    //----------------------------------------------
    /// Enable delete privilege for the specified client
    int32_t enableDelete(int32_t handle, bool force = false);

    //----------------------------------------------
    /// Disable delete privilege for the specified client
    void disableDelete(int32_t handle);

    //----------------------------------------------
    /// Return true if the current client has write permission
    bool canWrite(void);

    //----------------------------------------------
    /// Return true if thecurrent client has delete privilege
    bool canDelete(void);

    //----------------------------------------------
    /// Get parameter info for specified param Id

    //----------------------------------------------
    /// parse and handle an incoming message
    ///
    /// @param[in] msgJsonStr   JSON string received from Client
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    int32_t handleMessage(std::string& msgJsonStr);

    //----------------------------------------------
    /// Reset the Sensor FPGA Readout circuitry
    int32_t resetReadout(int32_t rtnIn = 0);

    //----------------------------------------------
    /// perform the OpenServer command
    int32_t doOpenServer(void);

    //----------------------------------------------
    /// perform the CloseServer command
    int32_t doCloseServer();

    //----------------------------------------------
    /// perform the HeartBeat command
    int32_t doHeartBeat();

    //----------------------------------------------
    /// perform the AcquireToken command
    int32_t doAcquireToken(void);

    //----------------------------------------------
    /// perform the ReleaseToken command
    int32_t doReleaseToken(void);

    //----------------------------------------------
    /// perform the StartCaptureSet command
    int32_t doStartCaptureSet(void);

    //----------------------------------------------
    /// perform the EditCaptureSet command
    int32_t doEditCaptureSet(void);

    //----------------------------------------------
    /// perform the GetParamArray command
    int32_t doGetParamArray(void);

    //----------------------------------------------
    /// perform the GetParam command
    int32_t doGetParam(void);

    //----------------------------------------------
    /// perform the SetParam command
    int32_t doSetParam(void);

    //----------------------------------------------
    /// read a local (Server Interface) software parameter value
    int32_t readLocalRawValue(StParameter& param,
                              uint32_t *pRawValue,
                              uint32_t index = 0,
                              uint32_t padIndex = DEF_SUBFRAME_COUNT,
                              int32_t rtnIn = 0);

    //----------------------------------------------
    /// Write a local (Server Interface) software parameter value
    int32_t writeLocalRawValue(StParameter& param,
                               uint32_t rawValue,
                               uint32_t mask,
                               uint32_t index = 0,
                               uint32_t padIndex = DEF_SUBFRAME_COUNT,
                               int32_t rtnIn = 0);

    //----------------------------------------------
    /// Initialize local parameters to their default values
    int32_t initLocalParams(void);

    //----------------------------------------------
    /// perform the CalcBackground command
    int32_t doCalcBackground(void);

    //----------------------------------------------
    /// perform the CalcFlatfield command
    int32_t doCalcFlatfield(void);

    //----------------------------------------------
    /// reload the corrections files
    int32_t doReloadCorr(void);

    //----------------------------------------------
    /// enable or disable background subtraction
    int32_t doEnableBackground(void);
    
    //----------------------------------------------
    /// Batch correct a whole run
    int32_t doBatchCorrectRun(void);

    //----------------------------------------------
    /// perform the StartCaptureRun command
    int32_t doStartCaptureRun(void);

    //----------------------------------------------
    /// perform the StopCaptureRun command
    int32_t doStopCaptureRun(void);

    //----------------------------------------------
    /// perform the GetRunStatus command
    int32_t doGetRunStatus(void);

    //----------------------------------------------
    /// perform the GetNextFrame command
    int32_t doGetNextFrame(void);

    //----------------------------------------------
    /// perform the GetTelemetry command
    int32_t doGetTelemetry(void);

    //----------------------------------------------
    /// perform the GetServerDataIndex command
    int32_t doGetServerDataIndex(void);

    //----------------------------------------------
    /// perform the  command
    int32_t doGetRunConfigData(void);

    //----------------------------------------------
    /// perform the GetBackground command
    int32_t doGetBackground(void);

    //----------------------------------------------
    /// perform the SetBackground command
    int32_t doSetBackground(void);

    //----------------------------------------------
    /// perform the GetRunFrame command
    int32_t doGetRunFrame(void);

    //----------------------------------------------
    /// perform the RunDMC command
    int32_t doRunDMC(void);

    //----------------------------------------------
    /// get list of connected clients
    int32_t doGetServerClientList(void);

    //----------------------------------------------
    /// Return true if server is busy
    bool isBusy(void);

    //----------------------------------------------
    /// Get a default run name
    ///
    /// @return valid run name
    ///
    std::string getDefaultRunName(void);

    //----------------------------------------------
    /// Set or clear newFrameAvailable flag for all clients
    void setNewFrameAvailable(bool available);

    //----------------------------------------------
    // Validat the cached values for all config parameters
    void validateConfigParams(void);

};

///@} end of class definitions

} // namespace SERVER_INTERFACE

//******************************************************************
// End of file
//******************************************************************

#endif //ST_IF_SERVER_H

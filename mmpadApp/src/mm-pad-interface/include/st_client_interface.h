//*******************************************************************
/// @file st_client_interface.h
/// @brief Sydor Client interface Class
/// @author      W.R. Lear
/// @date        11/12/2019
/// @copyright   2019 Sydor Technologies, All rights reserved.
///
/// This file defines the StClientInterface C++ class used by client
/// applications to communicate with a Sydor Pixel Array Detector
/// (PAD) Server
///
//*******************************************************************
#ifndef ST_CLIENT_INTERFACE_H
#define ST_CLIENT_INTERFACE_H

#include <stdint.h>
#include <string>
#include <vector>
#include <stdarg.h>
#include <exception>
#include "stutil_error.h"
#include "stutil_logger.h"
#include "stutil_system.h"
#include "st_errors.h"
#include "st_if_defs.h"
#include "st_message.h"
#include "st_datastore.h"
#include "st_parameter.h"
#include "st_framebuffer.h"
#include "st_clientlist.h"

namespace ST_INTERFACE
{

// Forward declaration of Data Index class
class StDataIndex;

//******************************************************************
// Definitions and Constants
//******************************************************************
#define ST_CLIENT_IF_MAJOR  (3)     ///< Library major version
#define ST_CLIENT_IF_MINOR  (8)     ///< Library minor version
#define ST_CLIENT_IF_BUILD  (0)     ///< Library build
#define ST_CLIENT_IF_PATCH  (0)     ///< Library patch

/// Full Version MMmmBBPP
#define  ST_CLIENT_IF_VERSION   (( ST_CLIENT_IF_MAJOR<<24) | \
                                 ( ST_CLIENT_IF_MINOR<<16) | \
                                 ( ST_CLIENT_IF_BUILD<<8 ) | \
                                 ( ST_CLIENT_IF_PATCH))

constexpr auto ST_MSG_TIMEOUT_MSEC          = 1500;
constexpr auto ST_MSG_OPEN_TIMEOUT_MSEC     = 5000;
constexpr auto ST_MSG_RUNDMC_TIMEOUT_MSEC   = 5000;
constexpr auto ST_MSG_CALC_BG_TIMEOUT_MSEC  = 5000;
constexpr auto ST_MSG_RELOAD_CORR_TIMEOUT_MSEC = 5000;

//******************************************************************
// Client Interface Class Definition
//******************************************************************
class StClientInterface
{
protected:
    STUTIL::Logger* pLog;               ///< Debug log instance
    STUTIL::SystemInfo mSysInfo;        ///< System information
    STServerInfo mServerInfo;           ///< Server information
    StDataStore mDataStore;             ///< Data Dictionary and parameter cache
    std::string mDictionary;            ///< Data dictionary JSON string
    std::vector<uint16_t> mRawTelemetry;///< Current raw telemetry data
    ST_INTERFACE::StFrameBuffer* mFrameBuffer;  ///< Current frame buffer

    uint32_t mOptionFlags;              ///< Library option flags
    bool mIsConnected;                  ///< true if connected to server
    int32_t mClientHandle;              ///< client handle assigned by the server
    bool mCanWrite;                     ///< true if client has write permission
    bool mCanDelete;                    ///< true if client has delete permission
    int32_t mToken;                     ///< collection of client permission flags
    STSystemType mSystemType;           ///< System Type
    bool mSimulator;                    ///< true if this is a simulated server
    uint32_t mServerVersion;            ///< Server realtime supervisor version
    uint32_t mServerLibVersion;         ///< Server library version

    StMessage mCurMessage;              ///< Reuseable message/response
    std::recursive_mutex mMsgSendCS;    ///< Mutex to serialize message access (temporary)

    void* mCommContext;                 ///< zeroMQ Context
    void* mCommSocket;                  ///< zeroMQ Socket

    std::recursive_mutex mScratchMutex;
    //MXFrame32 mScratchpadRawFrame;

public:
    //----------------------------------------------
    /// Constructor
    StClientInterface(const STServerInfo& info, uint32_t optionflags = 0);
    
    //----------------------------------------------
    /// Destructor
    ~StClientInterface();

    //----------------------------------------------
    /// Get the client interface library version
    uint32_t getLibVersion(void) { return ST_CLIENT_IF_VERSION; }

    //----------------------------------------------
    /// Get the server interface library version
    uint32_t getServerLibVersion(void) { return mServerLibVersion; }

    //----------------------------------------------
    /// Get the server software version
    uint32_t getServerVersion(void) { return mServerVersion; }

    //----------------------------------------------
    /// Get the Data dictionary verion
    uint32_t getDataDictionaryVersion(void);

    //----------------------------------------------
    /// Return true if this is a simulated server
    bool isSimulator(void) { return mSimulator; }

    //----------------------------------------------
    /// Get a reference to the server info
    const STServerInfo& getServoInfo(void) { return mServerInfo; }

    //----------------------------------------------
    /// Get the server host name or ip address string
    const char* getServerHost(void) { return mServerInfo.host; }

    //----------------------------------------------
    /// Get the server name string
    const char* getServerName(void) { return mServerInfo.name; }

    //----------------------------------------------
    /// Get the server description string
    const char* getServerDescription(void) { return mServerInfo.description; }

    //----------------------------------------------
    /// Get the server location string
    const char* getServerLocation(void) { return mServerInfo.location; }

    //----------------------------------------------
    /// Get the server type string
    const char* getServerType(void) { return mServerInfo.type; }

    //----------------------------------------------
    /// Get the server type string
    const STSystemType getServerSystemType(void) { return mSystemType; }

    //----------------------------------------------
    /// Get the client handle
    int32_t getClientHandle(void) { return mClientHandle; }

    //----------------------------------------------
    /// Convenience wrapper - initialize a new message
    ///
    /// @param[in] cmdName      Name of message command
    /// @param[in] rtnIn        Optional chained error code
    ///
    /// @return rtnIn if != 0, 0 if OK, else negative error code
    int32_t initMessage(const char* cmdname, int32_t rtnIn = 0);

    //----------------------------------------------
    /// Get pointer to the client side frame buffer
    //MXFrame32* getFramePtr(void) { return &mFrame; }

    //----------------------------------------------
    /// Return true if server is connected
    bool isServerConnected(void) { return mIsConnected; }

    //----------------------------------------------
    /// Return true if writing parameters is allowed
    bool canWrite(void) { return mCanWrite; }

    //----------------------------------------------
    /// Return true if deleting frame data is allowed
    bool canDelete(void) { return mCanDelete; }

    //----------------------------------------------
    /// open the connection to the server
    int32_t openConnection(void);
    
    //----------------------------------------------
    /// close the connection to the server
    int32_t closeConnection(void);

    //----------------------------------------------
    /// return false and log message if not connected
    bool isServerConnected(const char* name);

    //----------------------------------------------
    /// Acquire a permission token
    int32_t acquireToken(STToken token, bool force) ;

    //----------------------------------------------
    /// Release a permission token
    int32_t releaseToken(STToken token); 

    //----------------------------------------------
    /// Get list of all connected clients
    int32_t getClientList(StClientList& clientList);
      
    //----------------------------------------------
    /// Start a capture set
    int32_t startCaptureSet(const std::string& setName, 
                                    const std::string& description,
                                    const std::string&  tags);
                            
    //----------------------------------------------
    /// Edit the description and/or tags for a capture set
    int32_t editCaptureSet(const std::string& setName, 
                                   const std::string& description,
                                   const std::string&  tags);
    
    //----------------------------------------------
    /// Get a pointer to the current data store for a server
    StDataStore *getDataStore(void) { return &mDataStore; }

    //----------------------------------------------
    /// Get the current data dictionary as a JSON document
    const char *getDataDictionaryJSON(void);

    //----------------------------------------------
    /// get a list of parameters matching the specified state
    ///
    /// @param[out] params          vector to receive params matching specified state
    /// @param[in] stateFlags       desired state flags
    ///
    /// @return number of parameters found, or negative error code
    ///
    int32_t findParameters(std::vector<StParameter*>& params, uint32_t stateFlags)
        { return mDataStore.findParameters(params, stateFlags); }

    //----------------------------------------------
    /// get a list of parameters matching all the specified metadata.
    ///
    /// @param[out] params          vector to receive params matching specified state
    /// @param[in] str              string to compare to id
    /// @param[in] mode             access mode (RW, RO, WO, any)
    /// @param[in] domain           data domain or any
    /// @param[in] type             data type or any
    ///
    /// @return number of parameters found, or negative error code
    ///
    int32_t findParameters(std::vector<StParameter*>& params,
        const std::string& str = "",
        STAccessMode mode = AC_ANY,
        STDataDomain domain = DD_ANY,
        STDataType type = DT_ANY)
    {
        return mDataStore.findParameters(params, str.c_str(), mode, domain, type);
    }

    //----------------------------------------------
    /// get a list of parameters matching all the specified metadata.
    ///
    /// @param[out] params          vector to receive params matching specified state
    /// @param[in] str              string to compare to id
    /// @param[in] mode             access mode (RW, RO, WO, any)
    /// @param[in] domain           data domain or any
    /// @param[in] type             data type or any
    /// @param[in] startsWith       true if param id must start with str
    ///
    /// @return number of parameters found, or negative error code
    ///
    int32_t findParameters(std::vector<StParameter*>& params,
        const char* str = "",
        STAccessMode mode = AC_ANY,
        STDataDomain domain = DD_ANY,
        STDataType type = DT_ANY,
        bool startsWith = false)
    {
        return mDataStore.findParameters(params, str, mode, domain, type, startsWith);
    }

    //----------------------------------------------
    /// get a list of parameters containing or starting with the specified string.
    ///
    /// @param[out] params          vector to receive params matching specified state
    /// @param[in] str              strin to compare to id
    /// @param[in] startsWith       true if param id must start with str
    ///
    /// @return number of parameters found, or negative error code
    ///
    int32_t findParameters(std::vector<StParameter*>& params,
        const char* str, bool startsWith)
    {
        return mDataStore.findParameters(params, str, startsWith);
    }

    //----------------------------------------------
    /// get a list of parameters containing or starting with the specified string.
    ///
    /// @param[out] params          vector to receive params matching specified state
    /// @param[in] str              strin to compare to id
    /// @param[in] startsWith       true if param id must start with str
    ///
    /// @return number of parameters found, or negative error code
    ///
    int32_t findParameters(std::vector<StParameter*>& params,
        const std::string& str, bool startsWith)
    {
        return mDataStore.findParameters(params, str.c_str(), startsWith);
    }

    //----------------------------------------------
    /// Get the metadata for a parameter
    StParameter *getParamInfo(const std::string& id) 
    { 
        return getParamInfo(id.c_str()); 
    }

    //----------------------------------------------
    /// Get the metadata for a parameter
    ///
    /// @param[in] id     Parameter ID
    ///
    /// @return pointer to parameter info or nullptr if not found
    ///
    StParameter *getParamInfo(const char* id);

    //----------------------------------------------
    /// Get the value of a parameter
    ///
    /// @param[in] id           Parameter Id
    /// @param[out] value       destination for retrieved parameter value
    ///                         (unmodified if error)
    /// @param[in] index        Parameter array index (default = 0)
    /// @param[in] padIndex     PAD channel index (default = 0)
    ///
    /// @return 0 if OK, or negative error code
    ///
    template<typename T>
    int32_t getParam(const std::string& id, T& value,
        uint32_t index = 0, uint32_t padIndex = 0)
    {
        if (!isServerConnected("getParam")) return ST_ERR_SVR_NOT_OPEN;

        LOGTRACE("getParam(%s, %u)", id.c_str(), index);

        std::lock_guard<std::recursive_mutex> guard(mMsgSendCS);
        T val;
        int32_t rtn = initMessage(ST_STR_GET_PARAM);
        rtn = mCurMessage.setMessageParam<std::string>(ST_STR_PARAM_ID, id, rtn);
        rtn = mCurMessage.setMessageParam<uint32_t>(ST_STR_PARAM_INDEX, index, rtn);
        rtn = mCurMessage.setMessageParam<uint32_t>(ST_STR_PAD_INDEX, padIndex, rtn);
        rtn = sendMessage(rtn);
        rtn = mCurMessage.getResponseParam<T>(ST_STR_PARAM_VALUE, val, false, rtn);
        if (ST_ERR_OK == rtn)
        {
            value = val;
        }
        else
        {
            LOGERROR("getParam: id %s returned error %s", id.c_str(), STUTIL::getErrorStr(rtn).c_str());
        }
        return rtn;
    }

    //----------------------------------------------
    /// Get value(s) fro an array parameter
    ///
    /// @param[in] id           Parameter Id
    /// @param[out] values      destination for retrieved parameter values
    ///                         (unmodified if error)
    /// @param[in] index        Parameter array start index (default = 0)
    /// @param[in] count        Max number of values to return (default = 1)
    /// @param[in] padIndex     PAD channel index (default = 0)
    ///
    /// @return 0 if OK, or negative error code
    ///
    template<typename T>
    int32_t getParamArray(const std::string& id, std::vector<T>& values,
        uint32_t index = 0, uint32_t count = 1, uint32_t padIndex = 0)
    {
        if (!isServerConnected("getParamArray")) return ST_ERR_SVR_NOT_OPEN;

        LOGTRACE("getParamArray(%s, %u, %u)", id.c_str(), index, count);

        std::lock_guard<std::recursive_mutex> guard(mMsgSendCS);
        int32_t rtn = initMessage(ST_STR_GET_PARAM_ARRAY);
        rtn = mCurMessage.setMessageParam<std::string>(ST_STR_PARAM_ID, id, rtn);
        rtn = mCurMessage.setMessageParam<uint32_t>(ST_STR_PARAM_INDEX, index, rtn);
        rtn = mCurMessage.setMessageParam<uint32_t>(ST_STR_PARAM_COUNT, count, rtn);
        rtn = mCurMessage.setMessageParam<uint32_t>(ST_STR_PAD_INDEX, padIndex, rtn);
        rtn = sendMessage(rtn);
        rtn = mCurMessage.getResponseParamArray<T>(ST_STR_PARAM_ARRAY, values, false, rtn);
        if (ST_ERR_OK != rtn)
        {
            LOGERROR("getParamArray: id %s returned error %s", id.c_str(), STUTIL::getErrorStr(rtn).c_str());
        }
        return rtn;
    }

    //----------------------------------------------
    /// Set the value of a parameter
    ///
    /// @param[in] id           Parameter Id
    /// @param[out] value       value to write to the parameter
    /// @param[in] index        Parameter array index (default = 0)
    /// @param[in] padIndex     PAD channel index (default = 0)
    ///
    /// @return 0 if OK, or negative error code
    ///
    template<typename T>
    int32_t setParam(const std::string& id, const T& value, 
                     uint32_t index = 0, uint32_t padIndex = 0)
    {
        if (!isServerConnected("setParam")) return ST_ERR_SVR_NOT_OPEN;

        LOGTRACE("setParam(%s, %f)", id.c_str(), static_cast<double>(value));

        std::lock_guard<std::recursive_mutex> guard(mMsgSendCS);
        int32_t rtn = initMessage(ST_STR_SET_PARAM);
        rtn = mCurMessage.setMessageParam < std::string > (ST_STR_PARAM_ID, id, rtn);
        rtn = mCurMessage.setMessageParam<T>(ST_STR_PARAM_VALUE, value, rtn);
        rtn = mCurMessage.setMessageParam<uint32_t>(ST_STR_PARAM_INDEX, index, rtn);
        rtn = mCurMessage.setMessageParam<uint32_t>(ST_STR_PAD_INDEX, padIndex, rtn);
        rtn = sendMessage(rtn);
        if (ST_ERR_OK != rtn)
        {
            LOGERROR("setParam: id %s = %f returned error %s",
                id.c_str(), static_cast<double>(value), STUTIL::getErrorStr(rtn).c_str());
        }

        return rtn;
    }

    //----------------------------------------------
    /// Calculate a background image
    ///
    /// @param setName           Name of the set
    /// @param runName           Name of the run
    ///
    /// @return 0 on success, negative error code on any error
    ///
    int32_t calcBackground(const std::string& setName, const std::string& runName);


    //----------------------------------------------
    /// Calculate a flatfield image
    ///
    /// @param setName           Name of the set
    /// @param runName           Name of the run
    ///
    /// @return 0 on success, negative error code on any error
    ///
    int32_t calcFlatfield(const std::string& setName, const std::string& runName);

    //----------------------------------------------
    /// Reload the server corrections files
    ///
    /// @return 0 on success, negative error code on any error
    ///
    int32_t reloadCorr();

    //----------------------------------------------
    /// Start a batch conversion
    ///
    /// @param setName           Name of the set
    /// @param runName           Name of the run
    ///
    /// @return 0 on success, negative error code on any error
    /// 
    int32_t batchCorrect(const std::string& setName, const std::string& runName);

    //----------------------------------------------
    /// Enable background subtraction
    ///
    /// @param bEnable           Boolean to enable
    ///
    /// @return 0 on success, negative error code on any error
    ///
    int32_t enableBackground(const std::string& bEnable); 
    
    //----------------------------------------------
    /// Get the background image for the specified capture set, run
    ///
    /// @param[in] setName         Capture set name
    /// @param[in] runName         Capture run name
    /// @param[out] frameBuffer    Destination for background image
    ///
    /// @return 0 on success, negative error code on any error
    ///
    int32_t getBackground(const std::string setName, 
                          const std::string runName,
                          const std::string capNum,
                          ST_INTERFACE::StFrameBuffer& frameBuffer);

    //----------------------------------------------
    /// Set the background image for the specified capture set, run
    ///
    /// @param[in] setName         Capture set name
    /// @param[in] runName         Capture run name
    /// @param[in] bgSetName       Background set name
    /// @param[in] bgRunName       Background run name
    ///
    /// @return 0 on success, negative error code on any error
    ///
    //-=-= FIXME!!: Need setBackground version to set current default, or update a completed run
    int32_t setBackground(const std::string setName,
        const std::string runName,
        const std::string bgSetName,
        const std::string bgRunName );

    //----------------------------------------------
    /// Start a Frame Capture Run
    ///
    /// @param runName          User-defined name for the run
    /// @param[out] runId       Destination for run id supplied by server
    /// @param flags            option flags
    ///                         ST_RUN_OPT_BG : background run
    ///                         ST_RUN_OPT_NO_ALIGN : only report alignment fail
    ///
    /// @return 0 on success, negative error code on any error
    ///
    int32_t startCaptureRun(const std::string& runName,
                                    std::string& runId,
                                    uint32_t flags = 0);
    
    //----------------------------------------------
    /// Stop the active Frame Capture Run, if any
    ///
    /// @return 0 on success, negative error code on any error
    ///
    int32_t stopCaptureRun(void);
    
    //----------------------------------------------
    /// Get the status of the active or most recent Frame Capture Run
    ///
    /// @param[out] status      struct to receive status
    ///
    /// @return 0 on success, negative error code on any error
    ///
    int32_t getCaptureRunStatus(ST_INTERFACE::STRunStatus& status);

    //----------------------------------------------
    /// Get the next available raw sample frame
    ///
    /// @param[in] onlynew      true if only new frames are to be retrieved
    /// @param[out] frame       struct to receive frame
    ///
    /// @return 0 on success, negative error code on any error
    ///
    int32_t getNextFrame(bool onlynew, ST_INTERFACE::StFrameBuffer& frameBuffer);
    
    //----------------------------------------------
    /// Get the most recent raw telemetry data as a vector of 32-bit integers
    ///
    /// @param[in] rawData         vector to receive raw sensor data
    ///
    /// @return 0 on success, negative error code on any error
    ///
    int32_t getRawTelemetry(std::vector<uint16_t>& rawData);
    
    //----------------------------------------------
    /// Get the most recent scaled telemetry data
    ///
    /// @param[in] scaledData         vector to receive scaled sensor data
    ///
    /// @return 0 on success, negative error code on any error
    ///
    int32_t getScaledTelemetry(std::vector<double>& scaledData);
    
    //------------------------------------------------------------------
    /// Convert raw telemetry data to scaled telemetry data.
    ///
    /// @param[in] rawData      raw sensor data all subframes in the connected system
    /// @param[out] scaledData  scaled sensor data of the same dimension as rawData
    ///
    /// @return 0 on success, negative error code on any error
    ///
    /// @note fields in rawData that are null for the current configuration will
    ///       return 0 in the scaledData vector
    ///
    int32_t scaleTelemetry(const std::vector<uint16_t>& rawData,
                                 std::vector<double>& scaledData);

    //----------------------------------------------
    /// get a list of all capture runs currently available on the server
    ///
    /// @param[out] index      Class instance to receive index
    ///
    /// @return 0 on success, negative error code on any error
    ///
    //-=-= TODO: Pass in frame/run name patterns
    int32_t getDataIndex(StDataIndex& index);
    
    //----------------------------------------------
    /// get the configuration data for the specified capture run
    ///
    /// @param[in] setName      Capture Set name
    /// @param[in] runName      Capture Run Name
    /// @param[out] json        String to receive JSON output
    ///
    /// @return 0 on success, negative error code on any error
    ///
    int32_t getConfigData(const std::string& setName,
                                  const std::string& runName,
                                  std::string& json);

    //----------------------------------------------
    /// Transfer a single raw image frame
    ///
    /// @param[in] setName      Capture Set name
    /// @param[in] runName      Capture Run name
    /// @param[in] frameNumber  Frame number
    /// @param[out] pFrame      pointer to struct to receive frame
    ///
    /// @return 0 on success, negative error code on any error
    ///
    int32_t getRunFrame(const std::string& setName,
                                const std::string& runName,
                                uint32_t frameNumber,
                                ST_INTERFACE::StFrameBuffer& frameBuffer,
                                STDataType dataType = DT_INT32);

    //-=-= TODO: Add RunDMC option to reparse data dictionary and calibration files.
    //-=-=        Only doable when head is connected and valid, or perhaps serial number is required

    //----------------------------------------------
    /// Run the specified Device Maintenance Command
    ///
    /// @param[in] command      Command Id
    ///
    /// @return 0 on success, negative error code on any error
    ///
    int32_t runDMC(const std::string& command);

    //----------------------------------------------
    /// Open Communications channel
    ///
    /// @param[in] rtnIn        Optional chained error code
    ///
    /// @return rtnIn if != 0, 0 if OK, else negative error code
    ///
    int32_t openComm(int32_t rtnIn = 0) ;

    //----------------------------------------------
    /// Close Communications channel
    void closeComm(void);
    
    //----------------------------------------------
    /// Send a message to server, get response
    /// @param[in] rtnIn          Optional chained error code
    /// @param[in] timeoutMSec    Optional timeout in mSec
    ///
    /// @return rtnIn if != 0, 0 if OK, else negative error code
    int32_t sendMessage(int32_t rtnIn = 0, 
                        int32_t timeoutMSec = ST_MSG_TIMEOUT_MSEC);

protected:

}; //class StClientInterface

} //namespace ST_INTERFACE

//******************************************************************
// End of file
//******************************************************************
#endif //ST_CLIENT_INTERFACE_H

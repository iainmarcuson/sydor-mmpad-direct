//*******************************************************************
/// @file st_message.h
/// Sydor Client/Server Interface Message/Response classes
///
/// @author      W.R. Lear
/// @date        09/03/2019
/// @copyright   Copyright (c) 2019 Sydor Technologies, all rights reserved.
///
/// This header file defines the Message and Response classes 
/// used to communicate between Sydor Pixel Array Detector clients and servers.
///
//*******************************************************************
#ifndef ST_MESSAGE_H
#define ST_MESSAGE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string>
#include "st_if_defs.h"
#include "st_errors.h"
#include "stutil_logger.h"
#include "nlohmann_json.hpp"

namespace STUTIL
{
    class Logger;
}

namespace ST_INTERFACE
{

//******************************************************************
/// @name Definitions and Constants
///@{

//------------------------------------------------------------------
// Client message command names
#define ST_STR_INVALID_CMD            ""
#define ST_STR_OPEN_SERVER            "OpenServer"
#define ST_STR_CLOSE_SERVER           "CloseServer"
#define ST_STR_HEART_BEAT             "HeartBeat"
#define ST_STR_ACQUIRE_TOKEN          "AcquireToken"
#define ST_STR_RELEASE_TOKEN          "ReleaseToken"
#define ST_STR_START_CAPTURE_SET      "StartCaptureSet"
#define ST_STR_EDIT_CAPTURE_SET       "EditCaptureSet"
#define ST_STR_GET_DATA_DICTIONARY    "GetDataDictionary"
#define ST_STR_GET_PARAM              "GetParam"
#define ST_STR_SET_PARAM              "SetParam"
#define ST_STR_GET_PARAM_ARRAY        "GetParamArray"
#define ST_STR_CALC_BACKGROUND        "CalcBackground"
#define ST_STR_CALC_FLATFIELD         "CalcFlatfield"
#define ST_STR_RELOAD_CORR            "ReloadCorr"
#define ST_STR_START_CAPTURE_RUN      "StartCaptureRun"
#define ST_STR_STOP_CAPTURE_RUN       "StopCaptureRun"
#define ST_STR_GET_RUN_STATUS         "GetRunStatus"
#define ST_STR_GET_NEXT_FRAME         "GetNextFrame"
#define ST_STR_GET_TELEMETRY          "GetTelemetry"
#define ST_STR_GET_SERVER_DATA_INDEX  "GetServerDataIndex"
#define ST_STR_GET_SERVER_CLIENT_LIST "GetServerClientList"
#define ST_STR_GET_RUN_CONFIG_DATA    "GetRunConfigData"
#define ST_STR_GET_BACKGROUND         "GetBackground"
#define ST_STR_SET_BACKGROUND         "SetBackground"
#define ST_STR_ENABLE_BACKGROUND      "EnableBackground"
#define ST_STR_BATCH_CORRECT          "BatchCorrect"
#define ST_STR_GET_RUN_FRAME          "GetRunFrame"
#define ST_STR_RUN_DMC                "RunDMC"

//------------------------------------------------------------------
// Command and Response Message parameter names
#define ST_STR_BAD_PACKET_COUNT       "BadPacketCount"
#define ST_STR_BG_RUN_NAME            "BgRunName"
#define ST_STR_BG_SET_NAME            "BgSetName"
#define ST_STR_CLIENT                 "Client"
#define ST_STR_COMMAND                "Command"
#define ST_STR_COMPLETION_CODE        "CompletionCode"
#define ST_STR_DATA_BYTES             "DataBytes"
#define ST_STR_DESCRIPTION            "Description"
#define ST_STR_DIGITAL                "Digital"
#define ST_STR_DISK_PERCENT_FULL      "DiskPercentFull"
#define ST_STR_DISK_ERROR             "DiskError"
#define ST_STR_DMA_ERROR              "DmaError"
#define ST_STR_DMC_INITDMA            "InitDma"
#define ST_STR_DMC_FULL_RESET         "FullReset"
#define ST_STR_DMC_FLUSH              "Flush"
#define ST_STR_DMC_NAME               "DMCName"
#define ST_STR_DMC_RESET_CONNECT      "ResetConnection"
#define ST_STR_FRAME_BUFFER_BYTES     "FrameBufferBytes"
#define ST_STR_FRAME_COUNT            "FrameCount"
#define ST_STR_FRAME_NUMBER           "FrameNumber"
#define ST_STR_FRAMES_SAVED           "FramesSaved"
#define ST_STR_FORCE                  "Force"
#define ST_STR_IMAGE_HEIGHT           "ImageHeight"
#define ST_STR_IMAGE_TYPE             "ImageType"
#define ST_STR_IMAGE_WIDTH            "ImageWidth"
#define ST_STR_IS_ARMED               "IsArmed"
#define ST_STR_IS_BACKGROUND          "IsBackground"
#define ST_STR_IS_SIMULATOR           "isSimulator"
#define ST_STR_LIB_VERSION            "LibraryVersion"
#define ST_STR_MAX_FRAMES             "MaxFrames"
#define ST_STR_MAX_RUN_TIME           "MaxRunTime"
#define ST_STR_NAME                   "Name"
#define ST_STR_NO_DISK_SAVE           "NoDiskSave"
#define ST_STR_ONLY_NEW               "OnlyNew"
#define ST_STR_OPTION_FLAGS           "OptionFlags"
#define ST_STR_PAD_TYPE               "PadType"
#define ST_STR_PAD_INDEX              "PadIndex"
#define ST_STR_PARAM_ID               "ParamId"
#define ST_STR_PARAM_INDEX            "ParamIndex"
#define ST_STR_PARAM_MASK             "ParamMask"
#define ST_STR_PARAM_VALUE            "ParamValue"
#define ST_STR_PARAM_ARRAY            "ParamArray"
#define ST_STR_PARAMETERS             "Parameters"
#define ST_STR_RAW_FRAME_BYTES        "RawFrameBytes"
#define ST_STR_RUN_ID                 "RunId"
#define ST_STR_RUN_NAME               "RunName"
#define ST_STR_RUN_TIME               "RunTime"
#define ST_STR_RUNS                   "Runs"
#define ST_STR_SERVER_VERSION         "ServerVersion"
#define ST_STR_SET_DESCR              "SetDescription"
#define ST_STR_SET_NAME               "SetName"
#define ST_STR_SET_TAGS               "SetTags"
#define ST_STR_SETS                   "Sets"
#define ST_STR_START_FRAME            "StartFrame"
#define ST_STR_STATUS                 "Status"
#define ST_STR_SYSTEM_TYPE            "SystemType"
#define ST_STR_TIMESTAMP              "TimeStamp"
#define ST_STR_TOKEN                  "Token"
#define ST_STR_CORR_BG                "BgEnable"
#define ST_STR_CAP_NUM                "CapNum"
#define ST_STR_CAP_COUNT              "CapCount"
#define ST_STR_CAP_SELECT             "CapSelect"
#define ST_STR_PARAM_COUNT            "ParamCount"
#define ST_STR_USER_NAME              "UserName"
#define ST_STR_COMPUTER_NAME          "ComputerName"
#define ST_STR_OPERATING_SYSTEM       "OperatingSystem"
    
///@} end of definitions and constants

//******************************************************************
/// @name Typedefs, Structures and Enums
/// Data types used to pass information to/from library functions
///@{

//----------------------------------------------
/// Client message command enumeration
///
    typedef enum
    {
        MM_MSG_INVALID = 0,
        MM_MSG_OPEN_SERVER,
        MM_MSG_CLOSE_SERVER,
        MM_MSG_HEART_BEAT,
        MM_MSG_ACQUIRE_TOKEN,
        MM_MSG_RELEASE_TOKEN,
        MM_MSG_START_CAPTURE_SET,
        MM_MSG_EDIT_CAPTURE_SET,
        MM_MSG_GET_PARAM,
        MM_MSG_SET_PARAM,
        MM_MSG_CALC_BACKGROUND,
        MM_MSG_CALC_FLATFIELD,
        MM_MSG_RELOAD_CORR,
        MM_MSG_START_CAPTURE_RUN,
        MM_MSG_STOP_CAPTURE_RUN,
        MM_MSG_GET_RUN_STATUS,
        MM_MSG_GET_NEXT_FRAME,
        MM_MSG_GET_TELEMETRY,
        MM_MSG_GET_SERVER_DATA_INDEX,
        MM_MSG_GET_RUN_CONFIG_DATA,
        MM_MSG_GET_BACKGROUND,
        MM_MSG_SET_BACKGROUND,
        MM_MSG_GET_RUN_FRAME,
        MM_MSG_RUN_DMC,
        MM_MSG_ENABLE_BACKGROUND,
        MM_MSG_BATCH_CORRECT,
        MM_MSG_GET_PARAM_ARRAY,
        MM_MSG_GET_SERVER_CLIENT_LIST
    } MMMsgCmd;

//----------------------------------------------
/// Map to get client message enum from name
typedef std::map < std::string, MMMsgCmd> MMMessageMap;

///@} end of typedefs

//******************************************************************
/// @name Message Class definition
///@{
    
class StMessage
{
protected:
    // Common
    STUTIL::Logger* pLog;               ///< Debug Logger instance
    static MMMessageMap sMessageMap;    ///< Map of message names to enums

    // Message
    std::string mMsgStr;                ///< Raw message string
    nlohmann::json mMsgJson;            ///< Parsed JSON message
    std::string mMsgCmdName;            ///< Command string from message
    MMMsgCmd mMsgCmd;                   ///< Message Command enum
    uint8_t* mPMsgData;                 ///< Pointer to optional message  data
    size_t mMsgDataMaxLen;              ///< Maximum length of message data in bytes
    size_t mMsgDataLen;                 ///< Length of message data in bytes
    int32_t mMsgClient;                 ///< Message client handle

    // Response
    std::string mRespStr;               ///< Raw response string
    nlohmann::json mRespJson;           ///< JSON response
    std::string mRespCmdName;           ///< Command string from response
    MMMsgCmd mRespCmd;                  ///< Response command enum
    uint8_t* mPRespData;                ///< Pointer to optional response data
    size_t mRespDataMaxLen;             ///< Maximum length of response data in bytes
    size_t mRespDataLen;                ///< Length of response data in bytes
    int32_t mRespStatus;                ///< Response status code
    int32_t mRespClient;                ///< Response client handle

public:
    //**********************************************
    // Common methods
    
    //----------------------------------------------
    /// Constructor
    StMessage();
    
    //----------------------------------------------
    /// Destructor
    ~StMessage();
    
    //----------------------------------------------
    /// Initialize the message
    ///
    /// @param[in] cmdname        Message command name
    /// @param[in] clientHandle   Connected client handle
    ///
    /// @returns 0 if ok, negative error code on any error
    ///
    int32_t init(const char* cmdname, int32_t clientHandle = -1);

    //----------------------------------------------
    /// Set the message client handle
    ///
    /// @param[in] handle     New client handle value
    ///
    void setMessageClient(int32_t handle) { mMsgClient = handle; }

    //----------------------------------------------
    /// Get the message client handle
    ///
    int32_t getMessageClient(void) { return mMsgClient; }

    //----------------------------------------------
    /// Set the response client handle
    ///
    /// @param[in] handle     New client handle value
    ///
    void setResponseClient(int32_t handle) { mRespClient = handle; }

    //----------------------------------------------
    /// Get the response client handle
    ///
    int32_t getResponseClient(void) { return mRespClient; }

    //----------------------------------------------
    /// Get message as a string
    ///
    /// @param indent       indentation level (default = none)
    ///
    std::string getMessageStr(int32_t indent = -1);

    //----------------------------------------------
    /// Get response as a string
    ///
    /// @param indent       indentation level (default = none)
    ///
    std::string getResponseStr(int32_t indent = -1);

    //----------------------------------------------
    /// Return true if message appears valid
    bool isValid(void) { return (mMsgCmd != MM_MSG_INVALID); }
    
    //----------------------------------------------
    /// Get the Message command name

    std::string getMessageCmdName(void) { return mMsgCmdName; }

    //----------------------------------------------
    /// Get the Message command enum
    MMMsgCmd getMessageCmd(void) { return mMsgCmd; }

    //----------------------------------------------
    /// Get the response
    std::string getResponseCmdName(void) { return mRespCmdName; }

    //----------------------------------------------
    /// Get the Response Command enum
    MMMsgCmd getResponseCmd(void) { return mRespCmd; }

    //----------------------------------------------
    /// Get the Response status code
    int32_t getResponseStatus(void) { return mRespStatus; }

    //----------------------------------------------
    /// Check if a parameter exists in the message
    ///
    /// @param[in] paramname  Parameter name
    ///
    /// @returns true if specified parameter exists
    ///
    bool messageParamExists(const char* paramname);

    //----------------------------------------------
    /// Check if a parameter exists in the response
    ///
    /// @param[in] paramname  Parameter name
    ///
    /// @returns true if specified parameter exists
    ///
    bool responseParamExists(const char* paramname);
    
    //**********************************************
    // Client methods
    
    //----------------------------------------------
    /// Parse an incoming JSON response
    ///
    /// @param[in] respJsonStr      string containing the response
    ///
    /// @return rtn if != 0, 0 if OK, else negative error code
    ///
    int32_t parseResponse(std::string& respJsonStr);

    //----------------------------------------------
    /// Add or modify a parameter in the message JSON
    ///
    /// @param[in] name     Parameter name
    /// @param[in] value    Parameter value
    /// @param[in] rtnIn  Optional chained error code
    ///
    /// @return rtnIn if != 0, 0 if OK, else negative error code
    ///
    template<typename T>
    int32_t setMessageParam(const char* name, const T& value, int32_t rtnIn = 0)
    {
        if (0 != rtnIn)
        {
            return rtnIn;
        }

        try
        {
            mMsgJson[name] = value;
            return ST_ERR_OK;
        }
        catch (const nlohmann::json::exception& e)
        {
            // JSON parsing error
            LOGERROR("StMessage::setMessageParam(): error setting '%s': [%s]", name, e.what());
            return ST_ERR_PARAM;
        }
        catch (const std::exception& e)
        {
            // Any other error
            LOGERROR("ServerInterface::setMessageParam(): error setting '%s' [%s]", name, e.what());
            return ST_ERR_PARAM;
        }
    }

    //----------------------------------------------
    /// Get a parameter from the response
    ///
    /// @param[in] name     name of the response parameter to get
    /// @param[out] value   variable to receive value
    /// @param[in] optional parameter is optional if true
    /// @param[in] rtnIn    optional chained error code
    ///
    /// @return rtnIn if != 0, 0 if ok, else negative error code
    ///
    template<typename T>
    int32_t getResponseParam(const char* name, T& value, bool optional = false, int32_t rtnIn = 0)
    {
        if (0 != rtnIn)
        {
            // short hand for chained calls
            return rtnIn;
        }

        try
        {
            if (mRespJson.end() == mRespJson.find(name))
            {
                if (optional)
                {
                    return ST_ERR_OK;
                }
                else
                {
                    LOGERROR("StMessage::getResponseParam(): '%s': not found", name);
                    return ST_ERR_MSG_NOT_FOUND;
                }
            }
            mRespJson.at(name).get_to(value);
        }
        catch (const nlohmann::json::exception& e)
        {
            // JSON parsing error
            LOGERROR("StMessage::getResponseParam(): error getting '%s': [%s]", name, e.what());
            LOGWRITE(mRespJson.dump(2).c_str());
            return ST_ERR_MSG_FORMAT;
        }
        catch (const std::exception& e)
        {
            // Any other error
            LOGERROR("ServerInterface::getResponseParam(): exception getting '%s' [%s]", name, e.what());
            LOGWRITE(mRespJson.dump(2).c_str());
            return ST_ERR_MSG_FORMAT;
        }
        return ST_ERR_OK;
    }

    //----------------------------------------------
    /// Get an array parameter from the response
    ///
    /// @param[in] name     name of the response parameter to get
    /// @param[out] values  vector to receive values
    /// @param[in] optional parameter is optional if true
    /// @param[in] rtnIn    optional chained error code
    ///
    /// @return rtnIn if != 0, 0 if ok, else negative error code
    ///
    template<typename T>
    int32_t getResponseParamArray(const char* name, std::vector<T>& values, bool optional = false, int32_t rtnIn = 0)
    {
        if (0 != rtnIn)
        {
            // short hand for chained calls
            return rtnIn;
        }

        try
        {
            if (mRespJson.end() == mRespJson.find(name))
            {
                if (optional)
                {
                    return ST_ERR_OK;
                }
                else
                {
                    LOGERROR("StMessage::getResponseParam(): '%s': not found", name);
                    return ST_ERR_MSG_NOT_FOUND;
                }
            }
            values.clear();
            auto paramJson = mRespJson.at(name);
            if (paramJson.is_array())
            {
                for (auto entry = paramJson.begin(); entry != paramJson.end(); entry++)
                {
                    values.push_back(entry->get<T>());
                }
            }
            else
            {
                LOGERROR("%s: '%s' is not an array", __func__, name);
                return ST_ERR_NOT_ARRAY;
            }
        }
        catch (const nlohmann::json::exception& e)
        {
            // JSON parsing error
            LOGERROR("StMessage::getResponseParam(): error getting '%s': [%s]", name, e.what());
            LOGWRITE(mRespJson.dump(2).c_str());
            return ST_ERR_MSG_FORMAT;
        }
        catch (const std::exception& e)
        {
            // Any other error
            LOGERROR("ServerInterface::getResponseParam(): exception getting '%s' [%s]", name, e.what());
            LOGWRITE(mRespJson.dump(2).c_str());
            return ST_ERR_MSG_FORMAT;
        }
        return ST_ERR_OK;
    }

    //----------------------------------------------
    /// Set the message data Buffer pointer
    ///
    /// @param[in] pdata        pointer to data buffer
    /// @param[in] len          actual size of data in buffer
    /// @param[in] maxlen       max size of buffer
    ///
    /// @return actual size in bytes
    ///
    template<typename T>
    size_t setMessageData(T* pdata, size_t len, size_t maxlen)
    {
        mPMsgData = static_cast<uint8_t*>(pdata);
        if (nullptr == mPMsgData)
        {
            // No data
            mMsgDataLen = 0;
            mMsgDataMaxLen = 0;
            return 0;
        }

        len *= sizeof(T);
        maxlen *= sizeof(T);

        if (0 > maxlen)
        {
            // Set new max length
            mMsgDataMaxLen = maxlen;
        }
        
        mMsgDataLen = (len > mMsgDataMaxLen) ? mMsgDataMaxLen : len;
        return mMsgDataLen;
    }

    //----------------------------------------------
    /// Get Message data
    template<typename T>
    T* getMessageData(void) { return static_cast<T*>(mPMsgData); }

    //----------------------------------------------
    /// Get Message data length in bytes
    template<typename T>
    size_t getMessageDataLength() { return mMsgDataLen / sizeof(T); }

    //----------------------------------------------
    /// Set Message data length in bytes
    template<typename T>
    void setMessageDataLength(size_t len) { mMsgDataLen = (len * sizeof(T)); }

    //----------------------------------------------
    /// Get Message data maximum length in bytes
    template<typename T>
    size_t getMessageDataMaxLength() { return mMsgDataMaxLen / sizeof(T); }

    //----------------------------------------------
    /// Get Response data
    template<typename T>
    T* getResponseData(void) { return static_cast<T*>(mPRespData); }

    //----------------------------------------------
    /// Get Response data length in bytes
    template<typename T>
    size_t getResponseDataLength() { return mRespDataLen / sizeof(T); }

    //----------------------------------------------
    /// Set Response data length in bytes
    template<typename T>
    void setResponseDataLength(size_t len) { mRespDataLen = len * sizeof(T); }

    //----------------------------------------------
    /// Get Response data maximum length in bytes
    template<typename T>
    size_t getResponseDataMaxLength() { return mRespDataMaxLen / sizeof(T); }

    //----------------------------------------------
    /// Set the response data Buffer pointer
    ///
    /// @param[in] pdata        pointer to data buffer
    /// @param[in] nBytes       actual size of data in buffer
    /// @param[in] maxBytes     max size of buffer
    ///
    /// @return 0 if ok, else negative error code
    ///
    int32_t setResponseData(void* pdata, size_t nBytes, size_t maxBytes = 0);

    //**********************************************
    // Server methods
    
    //----------------------------------------------
    /// Parse an incoming JSON message
    ///
    /// @param[in] msgJsonStr      string containing the message
    /// @param[in] rtnIn         optional chained error code
    ///
    /// @return rtnIn if != 0, 0 on success, else negative error code
    ///
    int32_t parseMessage(std::string& msgJsonStr, int32_t rtnIn = 0);

    //----------------------------------------------
    /// Get a parameter from the message
    ///
    /// @param[in] name        name of message parameter to get
    /// @param[out] value      variable to receive value
    /// @param[in] optional    if true, no error if param is missing
    /// @param[in] rtnIn       optional chained error code
    ///
    /// @return rtnIn if != 0, 0 if ok, else negative error code
    ///
    template<typename T>
    int32_t getMessageParam(const char* name, T& value, 
                            bool optional = false, int32_t rtnIn = 0)
    {
        if (0 != rtnIn)
        {
            return rtnIn;
        }

        try
        {
            if (mMsgJson.end() == mMsgJson.find(name))
            {
                if (optional)
                {
                    return ST_ERR_OK;
                }
                else
                {
                    LOGERROR("StMessage::getMessageParam(): '%s': not found", name);
                    return ST_ERR_MSG_NOT_FOUND;
                }
            }
            mMsgJson.at(name).get_to(value);
        }
        catch (const nlohmann::json::exception& e)
        {
            // JSON parsing error
            LOGERROR("StMessage::getMessageParam(): error getting '%s': [%s]", name, e.what());
            LOGWRITE(mMsgJson.dump(2).c_str());
            return ST_ERR_MSG_FORMAT;
        }
        catch (const std::exception& e)
        {
            // Any other error
            LOGERROR("ServerInterface::getMessageParam(): exception parameter '%s' [%s]", name, e.what());
            LOGWRITE(mMsgJson.dump(2).c_str());
            return ST_ERR_MSG_FORMAT;
        }
        return ST_ERR_OK;
    }

    //----------------------------------------------
    /// Add or modify a parameter in the response JSON
    ///
    /// @param[in] name      name of message parameter to get
    /// @param[out] value    new value
    /// @param[in] rtnIn       optional chained error code
    ///
    /// @return rtnIn if != 0, 0 if ok, else negative error code
    ///
    template<typename T>
    int32_t setResponseParam(const char* name, const T& value, int32_t rtnIn = 0)
    {
        if (0 != rtnIn)
        {
            return rtnIn;
        }

        try
        {
            mRespJson[name] = value;
            return ST_ERR_OK;
        }
        catch (const nlohmann::json::exception& e)
        {
            // JSON parsing error
            LOGERROR("StMessage::setResponseParam(): error setting '%s': [%s]", name, e.what());
            return ST_ERR_PARAM;
        }
        catch (const std::exception& e)
        {
            // Any other error
            LOGERROR("ServerInterface::setResponseParam(): error setting '%s' [%s]", name, e.what());
            return ST_ERR_PARAM;
        }
    }

    //----------------------------------------------
    /// Add an array parameter in the response JSON
    ///
    /// @param[in] name      name of message parameter to get
    /// @param[out] values   vector of new values
    /// @param[in] rtnIn       optional chained error code
    ///
    /// @return rtnIn if != 0, 0 if ok, else negative error code
    ///
    template<typename T>
    int32_t setResponseParamArray(const char* name, const std::vector<T>& values, int32_t rtnIn = 0)
    {
        if (0 != rtnIn)
        {
            return rtnIn;
        }

        try
        {
            //-=-= FIXME -- must set ARRAY
            mRespJson[name] = nlohmann::json::array();
            auto& cache = mRespJson[name];
            for (uint32_t i = 0; i < values.size(); i++)
            {
                cache[i] = values[i];
            }
            return ST_ERR_OK;
        }
        catch (const nlohmann::json::exception& e)
        {
            // JSON parsing error
            LOGERROR("StMessage::setResponseParam(): error setting '%s': [%s]", name, e.what());
            return ST_ERR_PARAM;
        }
        catch (const std::exception& e)
        {
            // Any other error
            LOGERROR("ServerInterface::setResponseParam(): error setting '%s' [%s]", name, e.what());
            return ST_ERR_PARAM;
        }
    }

};
///@} end of StMessage class

} // namespace ST_INTERFACE

//******************************************************************
// End of file
//******************************************************************

#endif //ST_MESSAGE_H

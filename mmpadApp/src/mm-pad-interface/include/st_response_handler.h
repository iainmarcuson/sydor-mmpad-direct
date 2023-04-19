//*******************************************************************
/// @file st_response_handler.h
/// Sydor Server Interface Response Handler Base Class
///
/// @author      W.R. Lear
/// @date        10/06/2019
/// @copyright   Copyright (c) 2019 Sydor Technologies, all rights reserved.
///
/// This header file defines the C++ pure-virtual abstract base class 
/// for any class that handles responses to client messages received
/// by the server interface.
///
//*******************************************************************
#ifndef ST_RESPONSE_HANDLER_H
#define ST_RESPONSE_HANDLER_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string>
#include "st_if_defs.h"
#include "st_parameter.h"
#include "st_dataindex.h"
#include "st_framebuffer.h"
#include "st_clientlist.h"

namespace ST_INTERFACE
{

//******************************************************************
/// @name Definitions and Constants
///@{

///@} end of definitions and constants

//******************************************************************
/// @name Typedefs, Structures and Enums
/// Data types used to pass information to/from library functions
///@{
                                  

///@} end of typedefs


//******************************************************************
/// @name Class definitions
///@{
    
//------------------------------------------------------------------
/// Response Handler Base class
///
/// This is a pure virtual abstract base class for any class
/// used to handle responses to client messages received via the
/// server interface
///
class ResponseHandler
{
protected:
    //----------------------------------------------
    /// Constructor
    ResponseHandler() {}

public:
    
    //----------------------------------------------
    /// Destructor
    virtual ~ResponseHandler() {}

    //----------------------------------------------
    /// Get the file path to the Server's data dictionary JSON File
    ///
    /// @return string containing full or relative path to DataDictionary file
    ///
    virtual std::string getDataDictionaryPath(void) = 0;

    //----------------------------------------------
    /// Get the path to the Server's calibration file directory
    ///
    /// @return string containing path to the directory containing
    /// hardware specific calibration files
    ///
    virtual std::string getCalibrationDirPath(void) = 0;

    //----------------------------------------------
    /// Read the "raw" data register specified by the supplied data dictionary entry
    ///
    /// inherited from StDataAccess
    ///
    /// @param[in] param     reference to the dictionary entry for the parameter
    /// @param[out] pValue   pointer to variable to receive resultant raw value
    /// @param[in] index     optional array index for array parameters
    ///                      (ignored for non-array parameters)
    /// @param[in] padIndex  optional PAD index (ignored if not needed)
    ///
    /// @note For memory mapped array values, 
    /// the raw address = baseAddress + (index*arrayStride).
    /// 
    /// @note For SPI array values, the baseAddress is constant,
    /// but the raw array index = arrayOffset + (index*(arrayStride/4))
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    virtual int32_t readRawValue(ST_INTERFACE::StParameter& param,
        uint32_t* pValue,
        uint32_t index = 0,
        uint32_t padIndex = 0) = 0;

    //----------------------------------------------
    /// Read one or more "raw" values from an array parameter
    /// specified by the supplied data dictionary entry
    ///
    /// inherited from StDataAccess
    ///
    /// @param[in] param        reference to the dictionary entry for the parameter
    /// @param[out] rawValues   reference to vector to receive resultant raw value(s)
    /// @param[in] index        optional array index for array parameters
    ///                         (ignored for non-array parameters)
    /// @param[in] count        max number of values to read
    /// @param[in] padIndex     optional PAD index (ignored if not needed)
    ///
    /// @note For memory mapped array values, 
    /// the raw address = baseAddress + (index*arrayStride).
    /// 
    /// @note For SPI array values, the baseAddress is constant,
    /// but the raw array index = arrayOffset + (index*(arrayStride/4))
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    virtual int32_t readRawValueArray(ST_INTERFACE::StParameter& param,
        std::vector<uint32_t>& rawValues,
        uint32_t index,
        uint32_t count,
        uint32_t padIndex = 0) = 0;

    //----------------------------------------------
    /// Write the 'raw" data register specified by the suppled data dictionary entry
    ///
    /// inherited from StDataAccess
    ///
    /// @param[in] param     reference to data dictionary entry for the parameter
    /// @param[in] value     raw value to write,
    /// @param[in] mask      optional RMW mask. Ignore if 0
    /// @param[in] index     optional array index for array parameters
    ///                      (ignored for non-array parameters)
    /// @param[in] padIndex  optional PAD index (ignored if not needed)
    ///
    /// @note For memory mapped array values, 
    /// the raw address = baseAddress + (index*arrayStride).
    /// 
    /// @note For SPI array values, the baseAddress is constant,
    /// but the raw array index = arrayOffset + (index*(arrayStride/4))
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    virtual int32_t writeRawValue(ST_INTERFACE::StParameter & param,
                               uint32_t value, uint32_t mask = 0,
                               uint32_t index = 0,
                               uint32_t padIndex = 0) = 0;

    //----------------------------------------------
    /// Start a new active capture set
    ///
    /// @param[in] setName          capture set name
    /// @param[in] description   optional description for the set
    /// @param[in] tags          optional semicolon separated search tags
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    virtual int32_t startCaptureSet(const std::string& setName,
                                    const std::string& description,
                                    const std::string& tags) = 0;

    //----------------------------------------------
    /// Edit an existing capture set
    ///
    /// @param[in] setName          capture set name
    /// @param[in] description   optional new description for the set
    /// @param[in] tags          optional new semicolon separated search tags
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    virtual int32_t editCaptureSet(const std::string& setName,
                                   const std::string& description,
                                   const std::string& tags) = 0;

    //-----------------------------------------------
    /// Enable background subtraction
    ///
    /// @param[in] bEnable   Enable or disable BG sub
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    virtual int32_t enableBackgroundSub(bool bEnable) = 0;

    //----------------------------------------------
    /// Batch correct a run with current geocorrection
    ///
    /// @param[in] setName      Set name
    /// @param[in] runName      Run name to correct
    /// 
    /// @return 0 if successful, otherwise negative error code
    /// 
    virtual int32_t batchCorrectRun(const std::string setName,
        const std::string runName) = 0;

    //----------------------------------------------
    /// Calculate a background image
    ///
    /// @param[in] setName          capture set name
    /// @param[in] runName          capture run name
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    virtual int32_t calcBackground( const std::string& setName,
                                    const std::string& runName) = 0;

    //----------------------------------------------
    /// Calculate a flatfield image
    ///
    /// @param[in] setName          capture set name
    /// @param[in] runName          capture run name
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    virtual int32_t calcFlatfield(const std::string& setName,
        const std::string& runName) = 0;

    //----------------------------------------------
    /// Reload the corrections files
    /// 
    /// @return 0 if ok, negative error code on any error
    virtual int32_t reloadCorr(void) = 0;

    //----------------------------------------------
    /// Return true if background calculation is busy
    virtual bool calcBackgroundIsBusy(void) {return false;}

    //---------------------------------------------
    /// Get run statistics for post-processing
    /// @param[in] setName       the set name
    /// @param[in] runName       the run name
    /// @param[out] capCnt       the number of caps in the run
    /// @param[out] totalFrames  The total number of frames in a run
    /// @param[out] sampleFrame  A retrieved frame for metadata in caller
    /// 
    /// @return 0 if ok, negative error code on error
    ///
    virtual int32_t getRunStats(std::string& setName, std::string& runName, uint32_t& capCnt, uint32_t& totalFrames, ST_INTERFACE::StFrameBuffer& sampleFrame) = 0;

    //----------------------------------------------
    /// Check if a capture run name already exists
    /// 
    /// @param[in] setName      Set name
    /// @param[in] runName      Run name to check
    /// 
    /// @return true if the name exists, else false
    ///
    virtual bool runNameExists(const std::string setName, 
                               const std::string runName) = 0;

    //----------------------------------------------
    /// Check if a capture set name is valid
    /// 
    /// @param[in] setName      Set name to check
    /// 
    /// @return true if the name is valid, else false
    ///
    virtual bool setNameIsValid(const std::string setName) = 0;

    //----------------------------------------------
    /// Check if a capture run name is valid
    /// 
    /// @param[in] runName      Run name to check
    /// 
    /// @return true if the name is valid, else false
    ///
    virtual bool runNameIsValid(const std::string runName) = 0;

    //----------------------------------------------
    /// Start a frame capture run in the currently active set
    ///
    /// @param[in] runInfo          run config data
    /// @param[in] startTime        starting timestamp for run
    /// @param[in] configJson       JSON string containing all config values
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    virtual int32_t startCaptureRun(const STRunStatus& runStatus,
                                    uint64_t startTime,
                                    const std::string& configJson) = 0;

    //----------------------------------------------
    /// Stop the active frame capture run, if any
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    virtual int32_t stopCaptureRun(void) = 0;

    //----------------------------------------------
    /// get status info for the current or most recent capture run
    ///
    /// @param[out] status         struct to receive status
    ///
    /// @note: The getCaptureRunStatus method need only return
    /// the runId, frameCount, and armed elements of the
    /// status struct. The StServerInterface class can provide
    /// the setName and runName.
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    virtual int32_t getCaptureRunStatus(ST_INTERFACE::STRunStatus& status) = 0;

    //----------------------------------------------
    /// get the current index of frame data on the server
    ///
    /// @param[out] index          class instance to receive data index
    /// @param[in]  runBaseName    optional base name to restrict runs
    /// @param[in]  setBaseName    optional base name to restrict sets
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    virtual int32_t getServerDataIndex(
                    ST_INTERFACE::StDataIndex& index,
                    const std::string& runBaseName,
                    const std::string& setBaseName) = 0;

    //----------------------------------------------
    /// get the configuration data for the specified capture run
    ///
    /// @param[in] setName         Capture set name
    /// @param[in] runName         Capture run name
    /// @param[out] json           string to receive JSON document
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    virtual int32_t getRunConfigData(const std::string setName,
                                     const std::string runName,
                                     std::string& json) = 0;

    //----------------------------------------------
    /// get the background image for the specified capture run
    ///
    /// @param[in] setName         Capture set name
    /// @param[in] runName         Capture run name
    /// @param[out] frameBuffer    Class to receive the background frame
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    virtual int32_t getBackground(const std::string setName,
                                  const std::string runName,
                                  const std::string capNum,
                                  StFrameBuffer& frameBuffer) = 0;

    //----------------------------------------------
    /// set the background image for the specified capture run
    ///
    /// @param[in] setName         Capture set name
    /// @param[in] runName         Capture run name
    /// @param[in] bgSetName       Background set name
    /// @param[in] bgRunName       Background run name
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    virtual int32_t setBackground(const std::string setName,
        const std::string runName,
        const std::string bgSetName,
        const std::string bgRunName)
    {
        (void)setName;
        (void)runName;
        (void)bgSetName;
        (void)bgRunName;
        return ST_ERR_NOT_IMPL;
    }

    //----------------------------------------------
    // get one frame from the specified capture run
    ///
    /// @param[in] setName          Capture set name
    /// @param[in] runName          Capture run name
    /// @param[in] frameNumber      Frame number to get
    /// @param[out] frameBuffer     Frame buffer class
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    virtual int32_t getRunFrame(const std::string setName,
                                const std::string runName,
                                uint32_t frameNumber, 
                                StFrameBuffer& frameBuffer) = 0;

    //----------------------------------------------
    /// Check if a capture run is in progress
    ///
    /// @return true if armed for capture, false if idle
    ///
    virtual bool isArmed(void) = 0;

    //----------------------------------------------
    /// Send message to application for display
    ///
    /// Optional method intended to pass messages for
    /// display when running in simulation mode
    ///
    virtual void showMsg(const std::string& msg)
    {
        (void)msg;
    }

    //----------------------------------------------
    /// Run Device Maintenance Commands
    ///
    /// Perform the specified low level maintenance command
    ///
    virtual int32_t runDMC(const std::string& command)
    {
        (void)command;
        return ST_ERR_NOT_IMPL;
    }
}; // class ResponseHandler

///@} end of class definitions

} // namespace SERVER_INTERFACE

//******************************************************************
// End of file
//******************************************************************

#endif //RESPONSE_HANDLER_H

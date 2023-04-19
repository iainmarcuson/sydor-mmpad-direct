//*******************************************************************
/// @file st_datastore.h
/// Sydor Data Store - common to client and server 
///
/// @author      W.R. Lear
/// @date        08/26/2019
/// @copyright   Copyright (c) 2019 Sydor Technologies, all rights reserved.
///
/// The StDataStore class holds the collection of metadata defining
/// all externally accessible data parameters supported by a Sydor Pixel Array Detector
/// (PAD) system.
///
/// Data parameters are primarily abstractions of raw data register values available
/// obtained from hardware registers or low level software. 
/// For example, a parameter might represent a scaled version of the raw voltage value,
/// or a parameter could represent only a field of one or more bits within the raw value.
/// The StDataStore class contains all metadata needed to access the raw data
/// and convert to/from the parameter values.  
///
/// The StDataStore class is used on both the client and server side of distributed
/// system. The client side uses the data store primarily to validate parameter
/// identifiers and values entered by a user. The server side uses the data store
/// to receive parameter read/write requests from a client, access the associated
/// raw data values, and convert between the raw and parameter values.
/// 
/// The class also includes a parameter data cache used by the server interface to
/// efficiently obtain many of the parameter values without performing an actual
/// hardware access. The cache also allows the server to determine which parameter
/// values are in effect for a particular frame capture run, and which parameters
/// were modified relative to the previous run
/// 
/// The metadata for all available data store parameters are defined
/// in a Data Dictionary JSON file which the data store receives from the server
/// real-time supervisor application and parses during initialization. 
///
/// Note: The data store class does not include any methods for actually accessing
/// the raw hardware registers. Rather, the server software uses the 
/// data store metadata to perform the actual hardware data accesses.
///
//*******************************************************************
#ifndef ST_DATASTORE_H
#define ST_DATASTORE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <vector>
#include <mutex>
#include <istream>
#include "nlohmann_json_fwd.hpp"
#include "st_parameter.h"
#include "st_alias.h"

//******************************************************************
/// @name Forward Declarations
/// @{
namespace STUTIL
{
    class Logger;
}

namespace ST_INTERFACE
{
    class StParameter;
}

///@} end of Forward Declarations

namespace ST_INTERFACE
{

//******************************************************************
/// @name Typedefs, Structures and Enums
/// Data types used to pass information to/from library functions
///@{

///@} end of typedefs

//******************************************************************
/// @name Data Store Class definition
///@{

class StDataStore
{
protected:
    STUTIL::Logger *pLog;                       ///< Logger instance
    
    //----------------------------------------------
    // Dictionary Metadata from JSON file
    uint32_t mDictVersion;                          ///< Dictionary version from JSON Metadata
    std::string mDictDate;                          ///< Date from JSON Metadata
    std::string mDictionary;                        ///< Dictionary raw JSON text
    uint32_t mMinHfpgaVersion;                      ///< Minimum compatible Host FPGA Version
    uint32_t mMaxHfpgaVersion;                      ///< Maximum compatible Host FPGA Version
    uint32_t mMinSfpgaVersion;                      ///< Minimum compatible Sensor FPGA Version
    uint32_t mMaxSfpgaVersion;                      ///< Maximum compatible Sensor FPGA Version
    uint32_t mMinRtSupVersion;                      ///< Minimum compatible Realtime Supervisor Version
    uint32_t mMaxRtSupVersion;                      ///< Maximum compatible Realtime Supervisor Version

    //----------------------------------------------
    // Alias  definitions
    std::map<std::string, StAlias> mAliases;        ///< collection of alias definitions

    //----------------------------------------------
    // Parameter definitions
    std::map<std::string, StParameter> mParameters; ///< collection of parameter definitions

    //----------------------------------------------
    // Telemetry definitions

    std::vector<StParameter> mTelemParams;   ///< get parameter definition from Telemetry index

public:

    //----------------------------------------------
    /// Constructor
    StDataStore();
    
    //----------------------------------------------
    /// Destructor
    ~StDataStore();

    //----------------------------------------------
    // initialize everything - empty the data store
    void init(void);

    //----------------------------------------------
    /// get the number of Access enumeration values
    static int32_t getAccessNameCount(void) { return StParameter::getAccessNameCount(); }
 
    //----------------------------------------------
    /// get all access enumeration name strings
    static std::vector<const char*> getAccessNames(void) { return StParameter::getAccessNames(); }
    
    //----------------------------------------------
    /// get the name of a specified access enumeration value
    static const char* getAccessName(AccessMode_t a) { return StParameter::getAccessName(a); }

    //----------------------------------------------
    /// get the number of data domain enumeration values
    static int32_t getDomainNameCount(void) { return StParameter::getDomainNameCount(); }

    //----------------------------------------------
    /// get all data domain enumeration name strings
    static std::vector<const char*> getDomainNames(void) { return StParameter::getDomainNames(); }

    //----------------------------------------------
    /// get name of a specified data domain enumeration value
    static const char* getDomainName(DataDomain_t d) { return StParameter::getDomainName(d); }

    //----------------------------------------------
    /// get the number of data type enumeration values
    static int32_t getDataTypeNameCount(void) { return StParameter::getDataTypeNameCount(); }

    //----------------------------------------------
    /// get all data type enumeration name strings
    static std::vector<const char*> getDataTypeNames(void) { return StParameter::getDataTypeNames(); }

    //----------------------------------------------
    /// get the name of a specified data type enumeration value
    static const char* getDataTypeName(DataType_t t) { return StParameter::getDataTypeName(t); }

    //----------------------------------------------
    /// Get the data dictionary JSON document version
    uint32_t getDictionaryVersion(void) { return mDictVersion; }

    //----------------------------------------------
    /// Get the minimum compatible Host FPGA version
    uint32_t getMinHfpgaVersion(void) { return mMinHfpgaVersion; }

    //----------------------------------------------
    /// Get the maximum compatible Host FPGA version
    uint32_t getMaxHfpgaVersion(void) { return mMaxHfpgaVersion; }

    //----------------------------------------------
    /// Get the minimum compatible Sensor FPGA version
    uint32_t getMinSfpgaVersion(void) { return mMinSfpgaVersion; }

    //----------------------------------------------
    /// Get the maximum compatible Sensor FPGA version
    uint32_t getMaxSfpgaVersion(void) { return mMaxSfpgaVersion; }

    //----------------------------------------------
    /// Get the minimum compatible Realtime Supervisor version
    uint32_t getMinRtSupVersion(void) { return mMinRtSupVersion; }

    //----------------------------------------------
    /// Get the maximum compatible Realtime Supervisor version
    uint32_t getMaxRtSupVersion(void) { return mMaxRtSupVersion; }

    //----------------------------------------------
    /// Get the data dictionary JSON document creation date
    std::string getDictionaryDate(void) {return mDictDate;}

    //----------------------------------------------
    /// Get a copy of the data dictionary JSON document
    const std::string getDictionary(void) { return mDictionary; }

    //----------------------------------------------
    /// Get a pointer to the data dictionary JSON document
    const char* getDictionaryPtr(void) { return mDictionary.c_str(); }

    //----------------------------------------------
    /// Get the size of the data dictionary JSON document
    size_t getDictionarySize(void) { return mDictionary.size(); }

    //----------------------------------------------
    /// Get the number of defined telemetry parameters
    uint32_t getTelemParamCount(void) { return static_cast<uint32_t>(mTelemParams.size()); }

    //----------------------------------------------
    /// Check if this dictionary is compatible with the specified versions
    ///
    /// @param rtsupVer     Current Realtime supervisor Version, or 0 for don't care
    /// @param hfpgaVer     Current HFPGA Version, or 0 for don't care
    /// @param sfpgaVer     Current SFPGA Version, or 0 for don't care
    ///
    /// @return 0 if ok, or negative error code
    ///
    int32_t checkVersions(uint32_t rtsupVer, uint32_t hfpgaVer, uint32_t sfpgaVer);

    //----------------------------------------------
    /// get a list of parameter definitions matching the specified state
    ///
    /// @param[out] params          vector to receive params matching specified state
    /// @param[in] stateflags       desired state flags
    ///
    /// @return number of parameters found, or negative error code
    ///
    int32_t findParameters(std::vector<StParameter*>& params, uint32_t stateflags );

    //----------------------------------------------
    /// get a list of parameter definitions matching all the specified metadata.
    ///
    /// @param[out] params          vector to receive params matching specified state
    /// @param[in] id               string to compare to id
    /// @param[in] access           access mode (RW, RO, WO, any)
    /// @param[in] domain           data domain or any
    /// @param[in] type             data type or any
    /// @param[in] startsWith       if true, param id must start with pattern
    ///
    /// @return number of parameters found, or negative error code
    ///
    int32_t findParameters(std::vector<StParameter*>& params,
                            const char* id         = "", 
                            STAccessMode access    = AC_ANY,
                            STDataDomain domain    = DD_ANY,
                            STDataType type        = DT_ANY,
                            bool startsWith        = false);

    //----------------------------------------------
    /// get a list of parameter definitions matching all the specified metadata.
    ///
    /// @param[out] params          vector to receive params matching specified state
    /// @param[in] id               string to compare to id
    /// @param[in] access           access mode (RW, RO, WO, any)
    /// @param[in] domain           data domain or any
    /// @param[in] type             data type or any
    /// @param[in] startsWith       if true, param id must start with pattern
    ///
    /// @return number of parameters found, or negative error code
    int32_t findParameters(std::vector<StParameter*>& params,
        const std::string& id = "",
        STAccessMode access = AC_ANY,
        STDataDomain domain = DD_ANY,
        STDataType type = DT_ANY,
        bool startsWith = false)
    {
        return findParameters(params, id.c_str(), access, domain, type, startsWith);
    }

    //----------------------------------------------
    /// get a list of parameter definitions containing or starting with the specified string.
    ///
    /// @param[out] params          vector to receive params matching specified state
    /// @param[in] id               string to compare to id
    /// @param[in] startsWith       if true, param id must start with pattern
    ///
    /// @return number of parameters found, or negative error code
    int32_t findParameters(std::vector<StParameter*>& params,
        const std::string& id, bool startsWith)
    {
        return findParameters(params, id.c_str(), AC_ANY, DD_ANY, DT_ANY, startsWith);
    }

    //----------------------------------------------
    /// Find the specified parameter definition
    ///
    /// @param[in] id           Desired parameter id
    ///
    /// Returns a pointer to the specified parameter, or null if not found
    StParameter* findParameter(const std::string& id)
    {
        return findParameter(id.c_str());
    }

    //----------------------------------------------
    /// Find the specified parameter definition
    ///
    /// @param[in] id           Desired parameter id
    ///
    /// Returns a pointer to the specified parameter, or null if not found
    StParameter* findParameter(const char* id);

    //----------------------------------------------
    /// Find the specified parameter definition by name
    ///
    /// @param[in] name           Desired parameter name
    ///
    /// @return a pointer to the first matching parameter definition, or null if not found
    const StParameter* findParameterByName(const std::string& name)
    {
        return findParameterByName(name.c_str());
    }

    //----------------------------------------------
    /// Find the specified parameter definition by name
    ///
    /// @param[in] name           Desired parameter name
    ///
    /// @return a pointer to the first matching parameter, or null if not found
    const StParameter* findParameterByName(const char* name);

    //----------------------------------------------
    /// Get all telemetry parameters sorted by index (some entries my be null)
    ///
    /// @param[out] paramPtrs     Returned array of poniters to Telemetry parameters
    ///
    void getTelemetryParams(std::vector<const StParameter *>& paramPtrs );

    //----------------------------------------------
    /// Get scaled telemetry parameter value
    ///
    /// @param[in] index        Telemetry index
    /// @param[in] rawValue     Raw telemetry value
    /// 
    /// @return scaled telemetry value, or 0.0 if the index is invalid
    ///
    double getTelemetryScaledValue(uint32_t index, uint32_t rawValue);

    //----------------------------------------------
    /// Get all scaled telemetry parameter values
    /// 
    /// Iterates through the telemetry parameter definitions, applying 
    /// the conversion functions to the corresponding rawValues,
    /// cycling through the telemetry parameters as needed until
    /// all rawValues are converted.
    ///
    /// @param[in] rawValues              vector containing raw values
    /// @param[out] scaledValues          vector to receive scaled values
    /// 
    /// @return the number of values returned
    /// 
    int32_t getTelemetryScaledValues(const std::vector<uint16_t>& rawValues,
                                     std::vector<double>& scaledValues);

    //----------------------------------------------
    /// Clear the modified bit on all parameters
    void clearModified(void);

    //----------------------------------------------
    // Clear the valid and modified bits on all parameters
    void clearValid(void);

    //----------------------------------------------
    /// get a list of aliases containing or starting with the specified string.
    /// 
    /// @param[out] aliases         vector to receive matching aliases
    /// @param[in] id               string to compare to alias name
    /// @param[in] startsWith       if true, alias id must start with pattern
    ///
    /// @return number of aliases found, or negative error code
    ///
    int32_t findAliases(std::vector<StAlias*>& aliases,
        const char* id,
        bool startsWith = false);

    //----------------------------------------------
    /// get a list of aliases containing or starting with the specified string.
    /// 
    /// @param[out] aliases         vector to receive matching aliases
    /// @param[in] id               string to compare to alias name
    /// @param[in] startsWith       if true, alias id must start with pattern
    ///
    /// @return number of aliases found, or negative error code
    ///
    int32_t findAliases(std::vector<StAlias*>& aliases,
        const std::string& id,
        bool startsWith = false)
    {
        return findAliases(aliases, id.c_str(), startsWith);
    }

    //----------------------------------------------
    /// find the specified alias definition
    /// @param[in] id              alias id to find
    ///
    /// @return a pointer to the specifed alias definition, or null if not found
    ///
    const StAlias* findAlias(const char* id);

    //----------------------------------------------
    /// find the specified alias definition
    /// @param[in] id              alias id to find
    ///
    /// @return a pointer to the specifed alias definition, or null if not found
    ///
    const StAlias* findAlias(const std::string& id)
    {
        return findAlias(id.c_str());
    }

    //----------------------------------------------
    /// Get the specified Alias definition, if it exiats
    /// @param[in] id              alias to get
    ///
    /// @return the alias definition string or empty string if not found
    ///
    const std::string getAliasDef(const std::string& id)
    {
        return getAliasDef(id.c_str());
    }

    //----------------------------------------------
    /// Get the specified Alias definition, if it exiats
    /// @param[in] id              alias to get
    ///
    /// @return the alias definition string or empty string if not found
    ///
    const std::string getAliasDef(const char* id);

    //----------------------------------------------
    /// Add or replace a local alias definition
    /// @param[in] id              alias definition to add or replace
    /// @param[in] def             definition string for the alias
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    int32_t addAlias(const std::string& id, const std::string& def)
    {
        return addAlias(id.c_str(), def.c_str());
    }

    //----------------------------------------------
    /// Add or replace a local alias definition
    /// @param[in] id              alias definition to add or replace
    /// @param[in] def             definition string for the alias
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    int32_t addAlias(const char* id, const char* def);

    //----------------------------------------------
    /// Delete a local alias definition
    /// @param[in] id              alias definition to add or replace
    ///      
    /// @return 0 if ok, negative error code on any error
    ///
    int32_t deleteAlias(const std::string& id)
    {
        return deleteAlias(id.c_str());
    }

    //----------------------------------------------
    /// Delete a local alias definition
    /// @param[in] id              alias definition to add or replace
    ///      
    /// @return 0 if ok, negative error code on any error
    ///
    int32_t deleteAlias(const char* id);

    //----------------------------------------------
    /// De-serialize the data dictionary from a JSON file
    ///
    /// @param[in] jsonPath     full or relative path to source json file
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    int32_t fromJson(const std::string& jsonPath)
    {
        return fromJson(jsonPath.c_str());
    }

    //----------------------------------------------
    /// De-serialize the data dictionary from a JSON file
    ///
    /// @param[in] jsonPath     full or relative path to source json file
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    int32_t fromJson(const char* jsonPath);

    //----------------------------------------------
    /// De-serialize the data dictionary from a JSON string
    ///
    /// @param[in] jsonString     String containing the JSON definition
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    int32_t fromJsonStr(const std::string& jsonString);

    //----------------------------------------------
    /// De-serialize the data dictionary from a JSON stream
    ///
    /// @param[in] jsonStream      Input stream sourcing json document
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    int32_t fromJson(std::istream& jsonStream);

    //----------------------------------------------
    /// De-serialize the data dictionary from a JSON object
    ///
    /// @param[in] j       reference to source JSON object
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    int32_t  fromJson(const nlohmann::json& j);

    //----------------------------------------------
    /// Serialize to a JSON object
    ///
    /// @param[in] j       reference to destination JSON object
    ///
    void toJson(nlohmann::json& j) const;

    //----------------------------------------------
    /// get a config parameter JSON string
    ///
    /// get a JSON string containing the current values
    /// and other information about all capture run
    /// configuration parameters.
    ///
    int32_t getConfigJson(std::string& cfgDataStr);


};  //class StDataStore

///@} end of class definitions

} // namespace ST_INTERFACE

//******************************************************************
// End of file
//******************************************************************

#endif //ST_DATASTORE_H

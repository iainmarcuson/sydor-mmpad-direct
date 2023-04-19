//*******************************************************************
/// @file st_parameter.h
/// Sydor Data Store parameter class, common to client and server 
///
/// @author      W.R. Lear
/// @date        05/26/2020
/// @copyright   Copyright (c) 2019,2020 Sydor Technologies, all rights reserved.
///
/// This C++ class represents one parameter in the the Sydor
/// Data Store.
///
//*******************************************************************
#ifndef STPARAMETER_H
#define STPARAMETER_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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
/// Access Mode enum must be in this namespace for JSON (de)serialization
/// @note This MUST be consistent with STAccessMode
typedef enum
{
    ACT_RW  = STAccessMode::AC_RW,  ///< Read Write (default)
    ACT_RO  = STAccessMode::AC_RO,  ///< Read Only
    ACT_WO  = STAccessMode::AC_WO,  ///< Write Only
    ACT_ANY = STAccessMode::AC_ANY  ///< Any mode
} AccessMode_t;

//----------------------------------------------
/// Data Domain enum must be in this namespace for JSON (de)serialization
/// @note This MUST be consistent with STDataDomain
typedef enum
{
    DDT_BACK_CHANNEL    = STDataDomain::DD_BACK_CHANNEL,    ///< ACPP Back Channel registers (default)
    DDT_SENSOR_FPGA     = STDataDomain::DD_SENSOR_FPGA,     ///< Sensor FPGA registers
    DDT_HOST_FPGA       = STDataDomain::DD_HOST_FPGA,       ///< Host Interface FPGA registers
    DDT_RT_SUP          = STDataDomain::DD_RT_SUP,          ///< Real-time Supervisor software variable
    DDT_SERVER_IF       = STDataDomain::DD_SERVER_IF,       ///< Server interface software variable
    DDT_DATAPATH_FPGA   = STDataDomain::DD_DATAPATH_FPGA,   ///< Datapath FPGA SPI registers
    DDT_SIMULATOR       = STDataDomain::DD_SIMULATOR         ///< Simulator value (ignored by real server)
} DataDomain_t;

// # MacOS
// nope. #include <TargetConditionals.h>
// try
// todo: #ifdef __APPLE__

#ifdef Q_OS_MACOS
    // Why not work --??
   #undef DST_NONE
#endif

#if defined(TARGET_OS_OSX)
   #undef DST_NONE
#endif
//----------------------------------------------
/// Data Sub-domain enum must be in this namespace for JSON (de)serialization
/// @note This MUST be consistent with STDataSubDomain
typedef enum
{
    DST_NONE    = STDataSubDomain::DS_NONE,     ///< No subdomain (default)
    DST_CLIENT  = STDataSubDomain::DS_CLIENT,   ///< Per-client register
    DST_ACP_SPI = STDataSubDomain::DS_ACP_SPI,  ///< ACP Back Channel SPI register
    DST_ACP_I2C = STDataSubDomain::DS_ACP_I2C,  ///< ACP Back Channel I2C register
    DST_ARM     = STDataSubDomain::DS_ARM,      ///< Special Arm register handling
} DataSubDomain_t;

//----------------------------------------------
/// Data type enum must be in this namespace for JSON (de)serialization
/// @note This MUST be consistent with STDataType
typedef enum
{
    DTT_UINT32 = STDataType::DT_UINT32,  ///< 32-bit unsigned integer (default)
    DTT_INT32  = STDataType::DT_INT32,   ///< 32-bit signed integer
    DTT_UINT16 = STDataType::DT_UINT16,  ///< 16-bit unsigned integer
    DTT_INT16  = STDataType::DT_INT16,   ///< 16-bit signed integer
    DTT_UINT8  = STDataType::DT_UINT8,   ///<  8-bit unsigned integer
    DTT_INT8   = STDataType::DT_INT8,    ///<  8-bit signed integer
    DTT_UINT64 = STDataType::DT_UINT64,  ///< 64-bit unsigned integer
    DTT_INT64  = STDataType::DT_INT64,   ///< 64-bit signed integer
    DTT_FLOAT  = STDataType::DT_FLOAT,   ///< 64-bit floating point number
    DTT_DOUBLE = STDataType::DT_DOUBLE,  ///< 64-bit floating point number
    DTT_BOOL   = STDataType::DT_BOOL,    ///< Boolean value
    DTT_ENUM   = STDataType::DT_ENUM     ///< Enumerated value
} DataType_t;

//----------------------------------------------
/// Parameter Property Key enumeration
typedef enum
{
    PR_ID = 0,          ///< Unique ID
    PR_NAME,            ///< Name
    PR_DESCRIPTION,     ///< Description
    PR_DISABLE,         ///< True if disabled/ignored
    PR_ACCESS,          ///< Access mode
    PR_REQUIRED,        ///< True if param MUST be set before a run
    PR_CONFIG,          ///< True if a config param
    PR_COMMON,          ///< True if common to all blocks in MegaPAD
    PR_RAWREGISTER,     ///< True if param is a full raw register
    PR_DIMENSION,       ///< Dimension 
    PR_DOMAIN,          ///< Raw value data domain
    PR_SUBDOMAIN,       ///< Raw value data subdomain
    PR_ADDRESS,         ///< Raw value address
    PR_NBYTES,          ///< Raw value size in bytes
    PR_ARRAYSTRIDE,     ///< Raw value array index stride in bytes
    PR_ARRAYOFFSET,     ///< Array index offset (for SPI arrays)
    PR_STARTBIT,        ///< Start bit in raw value
    PR_NBITS,           ///< Width in bits
    PR_VOLATILE,        ///< True if volatile (no cache)
    PR_DATATYPE,        ///< Data type
    PR_MINIMUM,         ///< Minimum scaled value
    PR_MAXIMUM,         ///< Maximum scaled value
    PR_SCALE,           ///< Scale (multiplier) value
    PR_OFFSET,          ///< Offset value
    PR_DEFAULTVALUE,    ///< Default value
    PR_UNITS,           ///< Measurement units
    PR_ENUMVALUES,      ///< Enumeration values
    PR_FORMAT,          ///< Format string
    PR_CONVERSION,      ///< Special conversion function
    PR_COMMENT,         ///< Comment string
    PR_TELEM_NAME,      ///< Telemetry parameter short name
    PR_TELEM_INDEX,     ///< Telemetry parameter index
    PR_TELEM_DIMENSION, ///< Telemetry dimension
    PR_TELEM_ARRAY_STRIDE ///< Telemetry array stride
} PropertyKey_t;

//----------------------------------------------
/// Telem ASIC Index enumeration
typedef enum
{
    TELEM_ASIC_EVEN = 0,         ///< Even indexed telemetry ASIC
    TELEM_ASIC_ODD = 1,          ///< Odd indexed telemetry ASIC
    TELEM_ASIC_ANY = 2           ///< (Default) Even and odd indexed telemetry ASICs
} TelemAsicIndex_t;

//----------------------------------------------
/// Cache value entry
typedef struct CachedValue_Struct
{
    double value;
    uint32_t index;
    uint32_t rawValue;
    bool isValid;
    bool isModified;

    void toJson(nlohmann::json& j) const;
    void fromJson(const nlohmann::json& j);
} CachedValue_t;

///@} end of definitions and typedefs

//******************************************************************
/// @name Parameter Property Key Nams
///@{

// Parameter  Property Key Names
#define KEY_ID               "Id"              ///< Unique ID
#define KEY_NAME             "Name"            ///< Name
#define KEY_DESCRIPTION      "Description"     ///< Optional description string
#define KEY_DISABLE          "Disable"         ///< True if parameter is disabled/ignored
#define KEY_ACCESS           "Access"          ///< Access mode
#define KEY_CONFIG           "Config"          ///< True if this is a capture config parameter
#define KEY_COMMON           "Common"          ///< True if common to all blocks in MegaPAD
#define KEY_RAWREGISTER      "RawRegister"     ///< True if this parameter is a full raw register
#define KEY_REQUIRED         "Required"        ///< True if this parameter MUST be set before a capture run
#define KEY_DIMENSION        "Dimension"       ///< dimension (<=1: scalar, >1: Array
#define KEY_DOMAIN           "Domain"          ///< Raw value data domain
#define KEY_SUBDOMAIN        "SubDomain"       ///< Raw value data subdomain
#define KEY_ADDRESS          "Address"         ///< Raw value address
#define KEY_NBYTES           "NBytes"          ///< Raw value slze in bytes (default=4)
#define KEY_ARRAYSTRIDE      "ArrayStride"     ///< Raw value array stride in bytes (default=0)
#define KEY_ARRAYOFFSET      "ArrayOffset"     ///< Raw value array index offset for SPI arrays (default=0)
#define KEY_STARTBIT         "StartBit"        ///< Start bit in raw value (default=0)
#define KEY_NBITS            "NBits"           ///< width in bits (default = full word)
#define KEY_VOLATILE         "Volatile"        ///< True if is volatile (cannot be cached)
#define KEY_DATATYPE         "DataType"        ///< Data type
#define KEY_MINIMUM          "Minimum"         ///< minimum scaled value
#define KEY_MAXIMUM          "Maximum"         ///< maximum scaled value
#define KEY_SCALE            "Scale"           ///< Scale value
#define KEY_OFFSET           "Offset"          ///< Offset value
#define KEY_DEFAULTVALUE     "DefaultValue"    ///< Default value
#define KEY_UNITS            "Units"           ///< Measurement units string
#define KEY_ENUMVALUES       "EnumValues"      ///< Enumeration values
#define KEY_FORMAT           "Format"          ///< Format string
#define KEY_CONVERSION       "Conversion"      ///< Special conversion function
#define KEY_COMMENT          "Comment"         ///< Comment string
#define KEY_TELEM_NAME       "TelemName"       ///< Telemetry parameter short name
#define KEY_TELEM_INDEX      "TelemIndex"      ///< Telemetry parameter index
#define KEY_TELEM_DIMENSION  "TelemDimension"  ///< Telemetry parameter array dimension
#define KEY_TELEM_ARRAY_STRIDE "TelemArrayStride" ///< Telemetry parameter array stride
#define KEY_ARRAYOFFSET       "ArrayOffset"    ///< Array index offset (for SPI arrays)

#define KEY_ARRAY         "Array"           ///< Cached data
#define KEY_INDEX         "Index"           ///< Cached value index
#define KEY_VALUE         "Value"           ///< Cached scaled value
#define KEY_RAWVALUE      "RawValue"        ///< Cached raw value
#define KEY_MODIFIED      "Modified"        ///< Cached value modified
#define KEY_VALID         "Valid"           ///< Cached value valid

#define CONV_FPGA_VOLTAGE "FPGASupplyVolts"   ///< FPGA Internal supply voltage (volts)
#define CONV_VOLTAGE      "SensorVolts"       ///< Sensor ADC voltage (volts)
#define CONV_VOLTAGE_2    "SensorVolts2"      ///< Sensor ADC voltage x 2 (volts)
#define CONV_HV_VOLTAGE   "SensorHVVoltage"   ///< Sensor GBL HV voltage (volts)
#define CONV_BIAS_CURRENT "SensorBiasCurrent" ///< Sensor ADC voltage x 2 (volts)
#define CONV_HV_CURRENT   "SensorHVCurrent"   ///< Sensor GBL HV current (uA)
#define CONV_CURRENT      "SensorCurrent"     ///< Sensor ADC current (mA)
#define CONV_TEMP         "SensorTemp"        ///< Sensor ADC temperature (deg C)
#define CONV_DIE_TEMP     "DieTemp"           ///< FPGA Die temperature (deg C)
#define CONV_CAP_COUNT    "CapCount"          ///< Keck Capacitor count
#define CONV_8_NIBBLES    "8Nibbles"          ///< map value to/from eight nibbles

///@} Parameter Property Key Names

//******************************************************************
/// @name Parameter State flags
///@{
#define PS_ALL          (0x0000)     ///< Special case - all params
#define PS_REQUIRED     (0x0001)     ///< Only Required params
#define PS_CONFIG       (0x0002)     ///< Only Config params

#define PS_USE_VALID    (0x0010)     ///< ignore validity flg if 0
#define PS_INVALID      (PS_USE_VALID | (0x0000)) ///< Invalid params
#define PS_VALID        (PS_USE_VALID | (0x0020)) ///< valid params

#define PS_USE_MODIFIED (0x0100)     ///< ignore modified flag if 0
#define PS_UNMODIFIED   (PS_USE_MODIFIED | (0x0000)) ///< Unmodified since last clear
#define PS_MODIFIED     (PS_USE_MODIFIED | (0x0200)) ///< modified since last clear

#define PS_USE_VOLATILE (0x1000)     ///< ignore volatile flag if o
#define PS_NONVOLATILE  (PS_USE_VOLATILE | (0x0000)) ///< params not marked volatile
#define PS_VOLATILE     (PS_USE_VOLATILE | (0x2000)) ///< params marked volatile

#define PS_CONFIG_MODIFIED  (PS_CONFIG | PS_MODIFIED)
#define PS_CONFIG_INVALID   (PS_CONFIG | PS_INVALID)

///@} end of Parameter state flags

//******************************************************************
/// @name Class definitions
///@{

//------------------------------------------------------------------
/// Parameter Definition
class StParameter
{
private:
    // Parameter enumeration string values
    static const std::vector<const char*> sAccessStr;
    static const std::vector<const char*> sDomainStr;
    static const std::vector<const char*> sSubDomainStr;
    static const std::vector<const char*> sDataTypeStr;
    static const std::map<std::string, PropertyKey_t> sPropertyKeys;

    STUTIL::Logger *pLog;               ///< Logger instance

    // Common properties
    std::string     mId;                ///< Parameter Id - unique, immutable
    bool            mDisable;           ///< true if parameter is not in use
    std::string     mName;              ///< User friendly name
    std::string     mDescription;       ///< User friendly description
    AccessMode_t    mAccess;            ///< Access mode
    bool            mCommon;            ///< true if this is common to all blocks in MegaPAD
    bool            mRawRegister;       ///< true if this is is a full raw register
    bool            mRequired;          ///< true if user MUST set param before a run
    bool            mConfig;            ///< true if this is a Run configuration param
    uint32_t        mDimension;         ///< dimension if this is an array

    // Data Domain properties
    DataDomain_t    mDomain;            ///< Raw value data domain
    DataSubDomain_t mSubDomain;         ///< Raw value data subdomain
    uint32_t        mAddress;           ///< Raw value address
    uint32_t        mNBytes;            ///< Raw value size in bytes (default=4)
    uint32_t        mArrayStride;       ///< Array stride (default=0)
    uint32_t        mArrayOffset;       ///< Array index offset for SPI arrays (default = 0)
    uint32_t        mStartBit;          ///< Param start bit in raw value (default=0)
    uint32_t        mNBits;             ///< Param width in bits (default=full value)
    bool            mVolatile;          ///< True if param is volatile, non-cacheable

    // Telemetry properties
    std::string     mTelemName;         ///< Telemetry parameter short name
    uint32_t        mTelemIndex;        ///< Telemetry parameter index
    uint32_t        mTelemDimension;    ///< Telemetry array dimension
    uint32_t        mTelemArrayStride;  ///< Telemetry array stride

    // Data Type properties
    DataType_t      mDataType;          ///< Data type enum
    double          mMinimum;           ///< Minimum scaled value
    double          mMaximum;           ///< Maximum scaled value
    double          mScale;             ///< Scale multiplier
    double          mOffset;            ///< Scale offset (SV = RV*mScale + mOffset)
    double          mDefaultValue;      ///< Default value
    std::string     mUnits;             ///< Measurement units
    std::vector<std::string> mEnumValues; ///< Enumeration values
    std::string     mFormat;            ///< Format string
    std::string     mConversion;        ///< Custom conversion function
    // comment is not saved

    // Dynamic values
    std::vector<CachedValue_t>  mCachedValue;  ///< Current cached value(s)
    
    //---------------------------------------------
    /// Special conversion - FPGA internal supply voltages
    double convertFPGASupplyVoltage(uint32_t raw) const;
        
    //---------------------------------------------
    /// Special conversion - Sensor ADC Voltage
    double convertSensorVoltage(uint32_t raw) const;

    //---------------------------------------------
    /// Special conversion - Sensor ADC VDDA,VDDD Voltage
    double convertSensorVoltage2(uint32_t raw) const;

    //---------------------------------------------
    /// Global HV current
    double convertSensorHVVoltage(uint32_t raw) const;

    //---------------------------------------------
    /// Keck submodule bias currents
    double convertSensorBiasCurrent(uint32_t raw) const;

    //---------------------------------------------
    /// Global HV currect
    double convertSensorHVCurrent(uint32_t raw) const;

    //---------------------------------------------
    /// Special conversion - Sensor ADC Current
    double convertSensorCurrent(uint32_t raw) const;

    //---------------------------------------------
    /// Special conversion - Sensor ADC Temperature
    double convertSensorTemperature(uint32_t raw) const;

    //----------------------------------------------
    /// Special conversion - FPGA DieTemperature
    double convertDieTemperature(uint32_t raw) const;

    //----------------------------------------------
    /// Special conversion - convert Capacitor select to Capacitor count
    double convertCapCount(uint32_t raw) const;

    //----------------------------------------------
    /// Special Read conversion - replicate raw value into all 8 nibbles
    double convertFrom8Nibbles(uint32_t raw) const;

    //----------------------------------------------
    /// Special Write conversion - replicate raw value into all 8 nibbles
    uint32_t convertTo8Nibbles(uint32_t rawIn);


public:
    //----------------------------------------------
    /// Extract array index from parameter Id, if it exists
    ///
    /// @param[in]  fullId      Param Id and optional array reference, e.g. [0], [0,1]
    /// @param[out] paramId     returned parameter Id (with brackets stripped, if any)
    /// @param[out] index1      returned 1st index (0 if scalar)
    /// @param[out] index2      returned 2nd index (0 if scalar or 1d)
    ///
    /// @return the number of array dimensions (0,1, or 2), or negative error code
    ///
    static int32_t splitArrayIndex(const std::string fullId, std::string& paramId,
                                   uint32_t& index, uint32_t& padIndex );

    //----------------------------------------------
    /// Convert an FPGA timestamp value to a string
    ///
    /// @param[in] timestamp    Timestamp from the FPGA register
    ///
    /// @return the formatted string
    ///
    static std::string toString(StFpgaTimestamp timestamp);
         
    //----------------------------------------------
    /// Default Constructor
    StParameter(const std::string& id = "", nlohmann::json *pJson = nullptr);
   
    //----------------------------------------------
    /// Destructor
    ~StParameter(void);

    //----------------------------------------------
    /// Return true if param is readable
    bool isReadable(void) const { return (mAccess != ACT_WO); }

    //----------------------------------------------
    /// Return true if param is writable
    bool isWritable(void) const { return (mAccess != ACT_RO); }

    //----------------------------------------------
    /// Return true if param is writable
    bool isRawRegister(void) const { return mRawRegister; }

    //----------------------------------------------
    /// get the number of Access enumeration values
    static int32_t getAccessNameCount(void);
 
    //----------------------------------------------
    /// get all access enumeration name strings
    static std::vector<const char*> getAccessNames(void);

    //----------------------------------------------
    /// get the name of a specified access enumeration value
    static const char* getAccessName(AccessMode_t a);

    //----------------------------------------------
    /// get the number of data domain enumeration values
    static int32_t getDomainNameCount(void);

    //----------------------------------------------
    /// get all data domain enumeration name strings
    static std::vector<const char*> getDomainNames(void);

    //----------------------------------------------
    /// get name of a specified data domain enumeration value
    static const char* getDomainName(DataDomain_t d);

    //----------------------------------------------
    /// get name of a specified data subdomain enumeration value
    static const char* getSubDomainName(DataSubDomain_t d);

    //----------------------------------------------
    /// get the number of data type enumeration values
    static int32_t getDataTypeNameCount(void);

    //----------------------------------------------
    /// get all data type enumeration name strings
    static std::vector<const char*> getDataTypeNames(void);

    //----------------------------------------------
    /// get the name of a specified data type enumeration value
    static const char* getDataTypeName(DataType_t t);

    // Common
    std::string getId(void) {return mId;}
    std::string getName(void) {return mName;}
    std::string getDescription(void) {return mDescription;}
    bool        isRequired(void) {return mRequired;}
    bool        isConfig(void) { return mConfig; }
    bool        isCommon(void) { return mCommon; }
    uint32_t    getDimension(void) {return mDimension;}
    STAccessMode getAccess(void) { return static_cast<STAccessMode>(mAccess); }

    // Data Domain
    STDataDomain getDomain(void) { return static_cast<STDataDomain>(mDomain); }
    STDataSubDomain getSubDomain(void) { return static_cast<STDataSubDomain>(mSubDomain); }
    uint32_t    getAddress(void) {return mAddress;}
    uint32_t    getNBytes(void) {return mNBytes;}
    uint32_t    getArrayStride(void) { return mArrayStride; }
    uint32_t    getArrayOffset(void) { return mArrayOffset; }
    uint32_t    getStartBit(void) {return mStartBit;}
    uint32_t    getNBits(void) {return mNBits;}
    bool        isVolatile(void) {return mVolatile;}

    // Telemetry
    const std::string      getTelemetryName(void) const { return const_cast<const std::string&>(mTelemName); }
    const uint32_t         getTelemetryIndex(void) const { return const_cast<const std::uint32_t&>(mTelemIndex); }
    const uint32_t         getTelemetryDimension(void) const { return const_cast<const std::uint32_t&>(mTelemDimension); }
    const uint32_t         getTelemetryArrayStride(void) const { return const_cast<const std::uint32_t&>(mTelemArrayStride); }

    // Data Type
    STDataType  getDataType(void) { return static_cast<STDataType>(mDataType); }
    double      getMinimum(void) const {return mMinimum;}
    double      getMaximum(void) const {return mMaximum;}
    double      getScale(void) {return mScale;}
    double      getOffset(void) {return mOffset;}
    double      getDefaultValue(void) {return mDefaultValue;}
    std::string getUnits(void) const {return mUnits;}
    std::vector<std::string> getEnumValues(void) {return mEnumValues;}
    std::string getFormat(void) {return mFormat;}
    std::string getConversion(void) { return mConversion; }

    //----------------------------------------------
    // return true if parameter is an array
    bool isArray(void) { return (mDimension > 1); }

    //----------------------------------------------
    // return true if parameter is a telemetry value
    bool isTelemetry(void) { return (!mTelemName.empty()); }

    //----------------------------------------------
    /// get cached value
    ///
    /// @param[in] index     array index
    /// @param[in] padIndex  Quartus (aka PAD) index
    /// @param[out] value    destination for cached value
    ///
    /// @return 0 if OK, negative error code on any error
    template<typename T>
    int32_t getCachedValue(uint32_t index, uint32_t padIndex, T& value )
    {
        (void)padIndex; // Currently ignored

        if (index >= mDimension)
        {
            return ST_ERR_INDEX;
        }
        value = static_cast<T>(mCachedValue[index].value);
        return ST_ERR_OK;
    }

    //----------------------------------------------
    /// set cached value
    ///
    /// @param[in] index    array index
    /// @param[in] padIndex  Quartus (aka PAD) index
    /// @param[in] value    new scaled value
    /// @param[in] rawValue new raw value
    ///
    /// @return 0 if OK, negative error code on any error
    template<typename T>
    int32_t setCachedValue(uint32_t index, uint32_t padIndex, const T& value, uint32_t rawValue)
    {
        (void)padIndex; // Currently ignored

        if (index >= mDimension)
        {
            return ST_ERR_INDEX;
        }

        CachedValue_t& cached = mCachedValue[index];
        double prevValue = cached.value;
        cached.value = static_cast<double>(value);
        cached.rawValue = rawValue;
        if (cached.isValid && (cached.value != prevValue))
        {
            cached.isModified = true;
        }
        cached.isValid = true;

        return ST_ERR_OK;
    }

    //----------------------------------------------
    /// Compute scaled value from raw value
    ///
    /// @param[in] raw      raw value
    ///
    /// @returns scaled value
    double getScaledFromRaw(uint32_t raw) const;

    //----------------------------------------------
    /// Compute raw value from scaled value
    ///
    /// @param[in] val       scaled value
    /// @param[out] mask     mask for RMW operations (0 if not used)
    ///
    /// @returns raw value
    ///
    uint32_t getRawFromScaled(double val, uint32_t& mask);

    //----------------------------------------------
    /// return true if specified index was modified since last time it was reset
    bool isModified(uint32_t index);

    //----------------------------------------------
    /// return true if any index was modified since last time it was reset
    bool isModified(void);

    //----------------------------------------------
    /// clear the modified bit for the specified cached index
    void clearModified(uint32_t index);

    //----------------------------------------------
    // clear/set all modified flags
    void clearModified(void);

    //----------------------------------------------
    /// return true if specified cached index is valid
    bool isValid(uint32_t index);

    //----------------------------------------------
    /// return true if all cached indices are valid
    bool isValid(void);

    //----------------------------------------------
    /// clear the valid (and modified bits) for the specified index
    void clearValid(uint32_t index);

    //----------------------------------------------
    /// clear all valid (and modified) bits
    void clearValid(void);

    //----------------------------------------------
    /// Return true if value is within high & low limits
    bool isInLimits(double val);

    //----------------------------------------------
    /// Get a formatted string representation of a parameter value
    ///
    /// @param[in] value           value to convert
    /// @param[in] numericOnly     true = show enums as numeric only
    ///
    /// @return formatted string
    ///
    std::string toString(double value, bool numericOnly = false);

    //----------------------------------------------
    /// Get a parameter value from a string representation
    ///
    /// @param[in] str       input string representation
    /// @param[out] value    variable to receive converted value
    ///
    /// @return 0 if ok or negative error code 
    ///
    int32_t fromString(const std::string& str, double& value);

    //----------------------------------------------
    /// Get a string summarizing the parameter information
    ///
    /// @param[in] verbose    verbose output if true (default = false)
    ///
    std::string getSummary(bool verbose = false);

    //----------------------------------------------
    /// De-serialize from a JSON object
    int32_t fromJson(const nlohmann::json& j);
    
    //----------------------------------------------
    /// Serialize to a JSON object
    /// 
    /// @param[out] j            JSON object to receive result
    /// @param[in]  getValue     value only if true, definition if false
    void toJson(nlohmann::json& j, bool getValue = false) const;

    //----------------------------------------------
    /// Serialize to a JSON string
    /// 
    /// @param[out] jsonStr      string to receive result
    /// @param[in]  getValue     value onlyif true, definition if false
    void toJson(std::string& jsonStr, bool getValue = false) const;

}; // class StParameter

///@} end of class definitions

} // namespace ST_INTERFACE

//******************************************************************
// End of file
//******************************************************************

#endif //STPARAMETER_H

//*******************************************************************
/// @file st_if_defs.h
/// Sydor Interface definitions common to client and server 
///
/// @author      W.R. Lear
/// @date        3/10/2022
/// @copyright   Copyright (c) 2019-2022 Sydor Technologies, all rights reserved.
///
/// This file defines data structures and definitions common to
/// both the client and server sides of the Sydor Pixel Array Detector
/// (PAD) system interface.
///
//*******************************************************************
#ifndef ST_IF_DEFS_H
#define ST_IF_DEFS_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string>

#if defined (_WIN32) && defined(_MSC_VER)
// Disable VS2019 warning about 'enum' vs 'enum class'
#pragma warning(disable : 26812)    
#endif

//******************************************************************
/// @name Common Definitions and Constants
///@{

#define ST_COMPANY_NAME             "Sydor"     ///< Default company name for directories
#define  DEFAULT_SET_NAME           "default"   ///< Capture set name if none is defined

//------------------------------------------------------------------
// Limits
#define ST_MAX_HOST_LEN             64          ///< Max server hostname or ip address length
#define ST_MAX_NAME_LEN             64          ///< Max server Name string length
#define ST_MAX_TYPE_LEN             64          ///< Max server Type string length
#define ST_MAX_DESC_LEN             256         ///< Max server Description string length
#define ST_MAX_LOC_LEN              64          ///< Max server Location string length
#define ST_MAX_PORT_LEN             16          ///< Max TCP/IP port number string length
#define ST_MAX_SET_NAME_LEN         64          ///< Max capture set name length
#define ST_MAX_RUN_NAME_LEN         64          ///< Max capture run name length
#define ST_MAX_SERNUM_LEN           32          ///< Max PAD Serial number string length

#define ST_MAX_SUBFRAME_COUNT       4           ///< Max number of PAD heads in an X-PAD system.
#define MM_SUBFRAME_COUNT           1           ///< Number of PAD heads in a MM-PAD system
#define MG_SUBFRAME_COUNT           4           ///< Number of PAD heads in a Mega-PAD system.
#define KK_SUBFRAME_COUNT           1           ///< Number of PAD heads in a Keck-PAD system
#define DEF_SUBFRAME_COUNT          (0xff)      ///< Use default subframe Index
#define ST_SUBFRAME_SENSOR_COUNT    16          ///< Number of sensor asics per PAD head
#define KK_MAX_IMAGE_COUNT          536870911   ///< Max KeckPAD Image count (to avoid overflow)

// FrameBuffer Telemetry
#define ST_SENSOR_TELEM_VALUES      (12)        ///< Number of telemetry values per sensor asic
#define ST_TELEM_TEMP_INDEX         (9)         ///< Index of the telemetry temperature value
#define ST_SENSOR_TELEM_BYTES       (ST_SENSOR_TELEM_VALUES * sizeof(uint16_t))         ///< telem bytes per sensor asic
#define ST_SUBFRAME_TELEM_VALUES    (ST_SUBFRAME_SENSOR_COUNT * ST_SENSOR_TELEM_VALUES) ///< telem values per subframe
#define ST_SUBFRAME_TELEM_BYTES     (ST_SUBFRAME_SENSOR_COUNT * ST_SENSOR_TELEM_BYTES)  ///< telem bytes per subframe
#define ST_MAX_TOTAL_TELEM_VALUES   (ST_MAX_SUBFRAME_COUNT * ST_SUBFRAME_TELEM_VALUES)  ///< max telem values per system
#define ST_MAX_TOTAL_TELEM_BYTES    (ST_MAX_SUBFRAME_COUNT * ST_SUBFRAME_TELEM_BYTES)   ///< max telem bytes per system

//------------------------------------------------------------------
/// X-PAD System type
typedef enum
{
    ST_SYS_NONE,            ///< None, unknown, or any
    ST_SYS_MMPAD,           ///< MM-PAD
    ST_SYS_MEGAPAD,         ///< Mega-PAD
    ST_SYS_KECKPAD          ///< Keck-PAD
} STSystemType;

//------------------------------------------------------------------
// Invalid handle
#define ST_INVALID_HANDLE     -1            ///< Invalid handle value

//------------------------------------------------------------------
// Limits that are not defined in <limits.h>, but should be
const double ST_DBL_MAX_INT = (1LL << 53);
const double ST_DBL_MIN_INT = -ST_DBL_MAX_INT;

#if defined(DBL_MAX)
    const double ST_DBL_MAX = DBL_MAX;
#elif defined (__DBL_MAX__)
    const double ST_DBL_MAX = __DBL_MAX__;
#else
    const double ST_DBL_MAX = 1.7976931348623158e+308;
#endif

#if defined(DBL_MIN)
    const double ST_DBL_MIN = DBL_MIN;
#elif defined (__DBL_MIN__)
    const double ST_DBL_MIN = __DBL_MIN__;
#else
    const double ST_DBL_MIN = 2.2250738585072014e-308;
#endif

///@} End of common definitions


//******************************************************************
/// @name X-PAD Client/Server interface enums and structs
///@{
//------------------------------------------------------------------
// Feature control flags
#define ST_ENABLE_PERMISSION_TOKENS 0       ///< enable permission tokens if 1
#define ST_ENABLE_CLOSE_TIMEOUT     0       ///< enable client closure timeout if 1
#define ST_ENABLE_RELEASE_TIMEOUT   0       ///< enable client token release timeout if 1

//------------------------------------------------------------------
// Client<->Server interface Port number
#define ST_INTERFACE_PROTOCOL "tcp"         ///< Network interface protocol
#define ST_INTERFACE_PORT     "5555"        ///< Network interface port number
#define ST_SERVER_SOCKET  ST_INTERFACE_PROTOCOL "://*:" ST_INTERFACE_PORT  ///< Server msg socket 

//------------------------------------------------------------------
// Message Limits and Timeouts
#define ST_MAX_MSG_NBYTES            4096   ///< Max message JSON body size in bytes
#define ST_MAX_RESP_NBYTES           4096   ///< Max response JSON body size in bytes
#define ST_CLIENT_MSG_HEARTBEAT_MSEC 1000   ///< Max time before sending heartbeat msg
#define ST_CLIENT_MSG_RELEASE_MSEC  10000   ///< Timeout to release client tokens
#define ST_CLIENT_MSG_TIMEOUT_MSEC  60000   ///< Timeout to close client connection
#define ST_SERVER_MSG_TIMEOUT_MSEC   5000   ///< Timeout for server responses
#define ST_SERVER_POLL_MSEC           100   ///< Server message polling period
#define ST_CLIENT_POLL_MSEC          2000   ///< Client response polling timeout

// Server Interface timeouts
#define ST_MAX_POWER_ON_MSEC          500   ///< Max time for sensor ASIC power to go ready after enable
#define ST_MAX_READOUT_RESET_MSEC     100   ///< Max time for SERDES channels to align after reset

// Large Message Payload limits
#define ST_MAX_DICTIONARY_NBYTES  1000000   ///< Max data dictionary length in bytes
#define ST_MAX_DATA_INDEX_NBYTES  5000000   ///< Max data index length in bytes
#define ST_MAX_CONFIG_DATA_NBYTES  100000   ///< Max config data length in bytes
#define ST_MAX_FRAMEBUFFER_NBYTES 5000000   ///< Max framebuffer length in bytes 
#define ST_MAX_CLIENT_LIST_NBYTES  100000   ///< Max client list length in bytes

// StartCaptureRun option flags
#define ST_RUN_OPT_BG                   1   ///< This is a background run
#define ST_RUN_OPT_NO_ALIGN             2   ///< report but do not fail on alignment error
#define ST_RUN_OPT_NO_DISK_SAVE         4   ///< do NOT save this run to disk
#define ST_RUN_OPT_INDEX_NAME           8   ///< add index to run name to make unique

// Disconnected Temperature Sensor value
#define ST_NO_TEMP_SENSOR_VALUE        68   ///< Value from a disconnected temp sensor

// Number of data FPGAs in each head
#define ST_DFPGA_COUNT                  8   ///< Each MBP contains 8 data FPGAs

//------------------------------------------------------------------
/// Server Information struct
/// Collection of attributes defining a specific server location
typedef struct
{
    char host[ST_MAX_HOST_LEN + 1];         ///< Server hostname or ip address
    char name[ST_MAX_NAME_LEN + 1];         ///< Server name
    char type[ST_MAX_TYPE_LEN + 1];         ///< Server type
    char location[ST_MAX_LOC_LEN + 1];      ///< Server location
    char description[ST_MAX_DESC_LEN + 1];  ///< Server description
    char port[ST_MAX_PORT_LEN + 1];         ///< TCP/IP port number (defauilt 5555)
} STServerInfo;

//------------------------------------------------------------------
/// Permission Tokens
/// Set of all available client permission tokens
typedef enum
{
    ST_TOK_WRITE  = 0x01,       ///< Able to write parameters
    ST_TOK_DELETE = 0x02,       ///< Able to delete captured frame data
} STToken;

///@} End of X-PAD Client/Server interface enums and structs


//******************************************************************
/// @name Data Store Parameter Property enums and structs
///@{

//----------------------------------------------
/// Key parameter IDs
/// These parameter IDs are accessed programmatically by either the client
/// or server software. These IDs MUST match the corresponding entries
/// in the data dictionary.
#define HOST_DRIVER_VERSION_PARAM   "Host_Driver_Version"
#define HFPGA_VERSION_PARAM         "HFPGA_Version"
#define HFPGA_TIMESTAMP_PARAM       "HFPGA_Timestamp"
#define SFPGA_VERSION_PARAM         "SFPGA_Version"
#define SFPGA_TIMESTAMP_PARAM       "SFPGA_Timestamp"
#define FRAME_COUNT_PARAM           "Frame_Count"
#define IMAGE_COUNT_PARAM           "Image_Count"
#define GOOD_PKT_COUNT_LOW_PARAM    "HFPGA_Good_Packet_Count_Low"
#define GOOD_PKT_COUNT_HIGH_PARAM   "HFPGA_Good_Packet_Count_High"
#define CAP_SELECT_PARAM            "Cap_Select"
#define MAX_FRAME_COUNT_PARAM       "Max_Frame_Count"
#define TRIGGER_COUNT_PARAM         "Trigger_Count"
#define TRIGGER_MODE_PARAM          "Trigger_Mode"
#define CONNECTED_HEAD_PARAM        "Connected_Head"
#define SERDES_ALIGNMENT_PARAM      "SERDES_Alignment_Status"
#define READOUT_SW_RESET_PARAM      "Readout_SW_Reset"
#define HOST_REF_TAG_LOW_PARAM      "Host_Ref_Tag_Low"
#define HOST_REF_TAG_HIGH_PARAM     "Host_Ref_Tag_High"
#define ARM_CAPTURE_PARAM           "Arm_Capture"
#define POWER_ENABLE_PARAM          "Power_Enable"
#define POWER_READY_PARAM           "Power_Sequence_Done"
#define RAW_PWR_CTRL_PARAM          "Raw_Pwr_Ctrl_Reg"
#define POWER_ON_DELAY1_PARAM       "Power_On_Delay_1"
#define POWER_ON_DELAY2_PARAM       "Power_On_Delay_2"
#define POWER_ON_DELAY3_PARAM       "Power_On_Delay_3"
#define POWER_ON_DELAY4_PARAM       "Power_On_Delay_4"
#define POWER_ON_DELAY5_PARAM       "Power_On_Delay_5"
#define POWER_OFF_DELAY_PARAM       "Power_Off_Delay"
#define INTEGRATION_USEC_PARAM      "Integration_USec"
#define INTERFRAME_USEC_PARAM       "Interframe_USec"
#define HV_CONTROL_PARAM            "HV_Control"
const uint32_t PWR_CTRL_ENABLE      = 0x0001;
const uint32_t PWR_CTRL_DISABLE     = 0x0000;
const uint32_t PWR_CTRL_ON          = 0x0002;
const uint32_t HV_CTRL_ENABLE       = 0x0003;
const uint32_t HV_CTRL_DISABLE      = 0x0000;

#define CUR_IMAGE_COUNT_PARAM       "Cur_Image_Count"
#define CUR_FRAME_COUNT_PARAM       "Cur_Frame_Count"
#define RUN_DEBUG_STATUS_PARAM      "Run_Debug_Status"

// The following definitions apply to the AD5391 DAC on the KECK data path boards
#define DAC_PCR_CTRL_PARAM          "DFPGA_DAC_PCR_CTRL"
const uint32_t DAC_PCR_CTRL_ENABLE  = 0x00FF;
const uint32_t DAC_PCR_CTRL_RESET   = (DAC_PCR_CTRL_ENABLE | 0x0200);
const uint32_t DAC_PCR_CTRL_LDAC    = (DAC_PCR_CTRL_ENABLE | 0x0400);

#define DAC_SFR_CTRL_PARAM          "DFPGA_DAC_SFR_CTRL"
const uint32_t DAC_SFR_CTL_HI_Z     = 0x0800;
const uint32_t DAC_SFR_CTL_BOOST    = 0x0200;
const uint32_t DAC_SFR_CTL_MON      = 0x0080;
const uint32_t DAC_SFC_CTL_ENABLE   = (DAC_SFR_CTL_HI_Z | DAC_SFR_CTL_BOOST | DAC_SFR_CTL_MON);

#define DAC_MON_CHAN_PARAM          "DFPGA_DAC_SFR_MON_CHAN"
const uint32_t DAC_DEF_MON_CHAN     = 0;

#define DAC_OUT_VGUARD_PARAM        "DFPGA_DAC_OUT_VGUARD"
#define DAC_OUT_VINJ_PARAM          "DFPGA_DAC_OUT_VINJ"
#define DAC_OUT_VREF_BUF_PARAM      "DFPGA_DAC_OUT_VREF_BUF"
#define DAC_OUT_VREF_BP_PARAM       "DFPGA_DAC_OUT_VREF_BP"
#define DAC_OUT_VREF_PARAM          "DFPGA_DAC_OUT_VREF"
#define DAC_OUT_V_ISS_BUF_PIX_PARAM "DFPGA_DAC_OUT_V_ISS_BUF_PIX"
#define DAC_OUT_V_ISS_AB_PARAM      "DFPGA_DAC_OUT_V_ISS_AB"
#define DAC_OUT_V_ISS_BUF_PARAM     "DFPGA_DAC_OUT_V_ISS_BUF"

//----------------------------------------------
/// Parameter Access modes
typedef enum
{
    AC_RW  = 0,                 ///< Read Write (default)
    AC_RO  = 1,                 ///< Read Only
    AC_WO  = 2,                 ///< Write Only
    AC_ANY = 99                 ///< Any mode (for find only)
} STAccessMode;

#define ACCESS_RW_STR   "rw"    ///< Read/Write : default
#define ACCESS_RO_STR   "ro"    ///< Read Only
#define ACCESS_WO_STR   "wo"    ///< Write Only
#define ACCESS_ANY_STR  "any"   ///< Any mode (for find only)

//----------------------------------------------
/// Data Store Parameter Data Domains
typedef enum
{
    DD_BACK_CHANNEL = 0,        ///< ACP Back Channel registers (default)
    DD_SENSOR_FPGA,             ///< Sensor FPGA registers
    DD_HOST_FPGA,               ///< Host Interface FPGA registers
    DD_RT_SUP,                  ///< Real-time Supervisor software variable
    DD_SERVER_IF,               ///< Server Interface software variable
    DD_DATAPATH_FPGA,           ///< Datapath FPGA SPI registers
    DD_SIMULATOR,               ///< Simulator value (ignored by real server)
    DD_ANY = 99                 ///< Any domain (for find only)
} STDataDomain;

#define DOMAIN_BACK_CHANNEL_STR     "BackChannel"   ///< ACP Back Channel Register - default
#define DOMAIN_SENSOR_FPGA_STR      "SensorFPGA"    ///< Sensor FPGA PCIe or Back Channel Register
#define DOMAIN_HOST_FPGA_STR        "HostFPGA"      ///< Host FPGA PCIe Register
#define DOMAIN_RT_SUP_STR           "RtSup"         ///< Real-time Supervisor Software value
#define DOMAIN_SERVER_IF_STR        "ServerIF"      ///< Server Interface Software value
#define DOMAIN_SIMULATOR_STR        "Simulator"     ///< Simulator value (ignored on real server)
#define DOMAIN_DATAPATH_FPGA_STR    "DatapathFPGA" ///< Datapath FPGA SPI Register
#define DOMAIN_ANY_STR              "any"           ///< Any domain (For find only)

//----------------------------------------------
/// Data Store Parameter Data SubDomains
typedef enum
{
    DS_NONE = 0,                ///< No subdomain (default)
    DS_CLIENT,                  ///< Per-client value
    DS_ACP_SPI,                 ///< ACP Back Channel SPI register
    DS_ACP_I2C,                 ///< ACP Back Channel I2C register
    DS_ARM                      ///< Special ARM register handling
} STDataSubDomain;

#define DOMAIN_SUB_NONE_STR     "None"          ///< No subdomain (default)
#define DOMAIN_SUB_CLIENT_STR   "PerClient"     ///< Per-client value
#define DOMAIN_SUB_ACP_SPI_STR  "ACPSPI"        ///< ACP Back Channel SPI register
#define DOMAIN_SUB_ACP_I2C_STR  "ACPI2C"        ///< ACP Back Channel I2C register
#define DOMAIN_SUB_ARM_STR      "ArmReg"        ///< Per-client value

//----------------------------------------------
/// Data Store Parameter Data Types
typedef enum
{
    DT_UINT32 = 0,              ///< 32-bit unsigned integer
    DT_INT32,                   ///< 32-bit signed integer
    DT_UINT16,                  ///< 16-bit unsigned integer
    DT_INT16,                   ///< 16-bit signed integer
    DT_UINT8,                   ///< 8-bit  unsigned integer
    DT_INT8,                    ///< 8-bit  signed integer
    DT_UINT64,                  ///< 64-bit unsigned integer
    DT_INT64,                   ///< 64-bit signed integer
    DT_FLOAT,                   ///< 32-bit floating point number
    DT_DOUBLE,                  ///< 64-bit floating point number
    DT_BOOL,                    ///< Boolean value
    DT_ENUM,                    ///< Enumerated value
    DT_ANY = 99                 ///< Any type (for find only)
} STDataType;

#define DATATYPE_UINT32_STR     "uint32"        ///< 32-bit unsigned integer
#define DATATYPE_INT32_STR      "int32"         ///< 32-bit signed integer
#define DATATYPE_UINT16_STR     "uint16"        ///< 16-bit unsigned integer
#define DATATYPE_INT16_STR      "int16"         ///< 16-bit signed integer
#define DATATYPE_UINT8_STR      "uint8"         ///< 8-bit unsigned integer
#define DATATYPE_INT8_STR       "int8"          ///< 8-bit signed integer
#define DATATYPE_UINT64_STR     "uint64"        ///< 64-bit unsigned integer
#define DATATYPE_INT64_STR      "int64"         ///< 64-bit signed integer
#define DATATYPE_FLOAT_STR      "float"         ///< 32-bit floating point value
#define DATATYPE_DOUBLE_STR     "double"        ///< 64-bit floating point value
#define DATATYPE_BOOL_STR       "bool"          ///< Boolean  0 = false, <>0 = true
#define DATATYPE_ENUM_STR       "enum"          ///< Enumeration
#define DATATYPE_ANY_STR        "any"           ///< Any  type (for find only)

///@} end of Data Store Parameter Property enums and structs


//******************************************************************
/// @name X-PAD Common Frame Related Definitions
///@{

//------------------------------------------------------------------
/// Manifest constants
#define ST_FRAME_ID             (0x5354)        ///< StFrameBuffer ID "Magic Number"
#define ST_FRAME_MAJOR_VERSION  (0x01)          ///< StFrameBuffer Major Version
#define ST_FRAME_MINOR_VERSION  (0x02)          ///< StFrameBuffer Minor Version
#define ST_FRAME_VERSION        ((ST_FRAME_MAJOR_VERSION<<8) + ST_FRAME_MINOR_VERSION)
#define ST_FRAME_MAX_SECTIONS   (4)             ///< Maximum number of optional frame data sections
#define ST_FRAME_HEADER_BYTES   (256)           ///< total size of frame header in bytes
#define ST_FRAME_META_BYTES     (80)            ///< total reserved header space for frame metadata
#define ST_FRAME_FOOTER_BYTES   (8)             ///< size of frame footer in bytes
#define ST_FRAME_FOOTER         (0xE4F1E3F2E2F3E1F4) ///< Serialized frame footer (end of frame)
#define ST_GET_MAJOR_VERSION(v) ((v)>>8)        ///< Get the major version number

//------------------------------------------------------------------
/// Capture Run Trigger Mode
//-=-= FIXME!!: must add Keck trigger modes
typedef enum
{
    MT_SW_TRIGGER_EDGE = 0,
    MT_SW_UNUSED       = 1,
    MT_HW_TRIGGER_EDGE = 2,
    MT_HW_RING_CLOCK   = 3,
} STTriggerMode;

//-=-= FIXME: must add Keck trigger modes
#define IS_ST_TRIGGER_SW(mode)    ( ( MT_SW_TRIGGER_EDGE  == (mode) ) || \
                                    ( MT_SW_UNUSED        == (mode) ) )

#define IS_ST_TRIGGER_HW(mode)    ( (MT_HW_TRIGGER_EDGE   == (mode) ) || \
                                    (MT_HW_RING_CLOCK     == (mode) ) )
   
//-------------------------------------------------------------------
/// Frame Status flags
#define ST_FRAME_STAT_RAW           (0x00000000)    ///< Raw frame - no corrections
#define ST_FRAME_STAT_BG_SUBTRACT   (0x00000001)    ///< Background subtracted
#define ST_FRAME_STAT_DEBOUNCE      (0x00000002)    ///< Debounce correction applied
#define ST_FRAME_STAT_FLATFIELD     (0x00000004)    ///< flatfield corrected
#define ST_FRAME_STAT_BAD_PIXEL_MAP (0x00000008)    ///< Bad pixel map corrected
#define ST_FRAME_STAT_GEOMETRIC     (0x00000010)    ///< Geometric correction applied

#define ST_FRAME_STAT_DEFAULT    ST_FRAME_STAT_RAW  ///< Default frame status

//------------------------------------------------------------------
// X-PAD Raw frame footer marker values
const uint64_t ST_FRAME_MARKER1 = 0x5555555555555555;       ///< First marker value
const uint64_t ST_FRAME_MARKER2 = 0xEEEEEEEEEEEEEEEE;       ///< End marker value for all but last frame
const uint64_t ST_FRAME_MARKER2_LAST = 0xFFFFFFFFFFFFFFFF;  ///< End marker value for last frame in run

//------------------------------------------------------------------
/// X-PAD FPGA Timestamp structure
#pragma pack(push, 1)
typedef union
{
    uint32_t raw;
    struct _ts
    {
        unsigned second     : 6;
        unsigned minute     : 6;
        unsigned hour       : 5;
        unsigned year       : 6;
        unsigned month      : 4;
        unsigned dayOfMonth : 5;
    } t;
}StFpgaTimestamp;
#pragma pack(pop)

//-=-= FIXME!!: Update the exposure field definitions for KECK & MM/MEGA
//------------------------------------------------------------------
/// X-PAD Raw Frame metadata struct
/// 
/// Exposure field definitions:
/// 2:0  : Trigger mode
/// 3    : 1 = DeInterleaved
/// 4    : 1 = CDS On
/// 5    : 1 = sWIN On
/// 7:6  : 0 (reserved)
/// 11:8 : quartus index 0-3
/// 13:12: 0 (reserved)
/// 14   : 1 = HTrig On
/// 15   : 1 = Shutter On
/// 31:16: 0 (reserved)
///
/// "Detectors" field definitions
/// 15:0 : 1 = detector enabled (deprecated)
/// 31-16: 0 (reserved)
#pragma pack(push, 1)
typedef struct
{
    uint64_t hostReference;         ///< timestamp of capture run
    uint32_t runFrameNumber;        ///< frame number reset when capture is armed
    uint32_t triggerFrameNumber;    ///< frame number reset on leading edge of each trigger
    uint64_t timeStamp;             ///< timestamp of frame relative to ARM
    uint32_t integrationTime;       ///< Integration time (uSec)
    uint32_t interFrameTime;        ///< Inter-frame time (uSec)
    uint32_t exposureMode;          ///< See field definitons
    uint32_t detectors;             ///< bit mask identifying active detectors
} STFrameMetadata;
#pragma pack(pop)

//------------------------------------------------------------------
/// X-PAD raw telemetry for one subframe
#pragma pack(push, 1)
typedef struct
{
    uint16_t data[ST_SUBFRAME_TELEM_VALUES];
} STRawTelemetry;
#pragma pack(pop)

///@} End of X-PAD Common Frame Related Definitions

//******************************************************************
/// @name MM-PAD/Mega-PAD Common frame related definitions
///@{

//------------------------------------------------------------------
/// MM-PAD/Mega-PAD Single raw image pixel
const STDataType MX_RAW_PIXEL_TYPE = DT_INT32;  ///< Raw pixel type
typedef int32_t MXRawPixel;
const uint32_t MX_RAW_PIXEL_BYTES = (sizeof(MXRawPixel));

//------------------------------------------------------------------
// MM-PAD/Mega-PAD raw frame definitions
const uint32_t   MX_RAW_IMAGE_WIDTH  = 512;     ///< pixels per row
const uint32_t   MX_RAW_IMAGE_HEIGHT = 512;     ///< rows per image
const uint32_t   MX_RAW_IMAGE_PIXELS = (MX_RAW_IMAGE_HEIGHT * MX_RAW_IMAGE_WIDTH);
const uint32_t   MX_RAW_IMAGE_BYTES  = (MX_RAW_IMAGE_PIXELS * MX_RAW_PIXEL_BYTES);

//------------------------------------------------------------------
/// MM-PAD/Mega-PAD raw image
/// Raw image as received from one PAD channel
/// 
#pragma pack(push, 1)
typedef MXRawPixel MXRawImage[MX_RAW_IMAGE_HEIGHT][MX_RAW_IMAGE_WIDTH];
#pragma pack(pop)

//------------------------------------------------------------------
// MM-PAD/Mega-PAD Raw frame footer sizes
const uint32_t MX_RAW_FRAME_FOOTER_BYTES    = MX_RAW_IMAGE_WIDTH * MX_RAW_PIXEL_BYTES; ///< Raw footer size in bytes
const uint32_t MX_RAW_FRAME_META_BYTES      = sizeof(STFrameMetadata);      ///< Metadata size in bytes
const uint32_t MX_RAW_FRAME_TELEMETRY_BYTES = sizeof(STRawTelemetry);       ///< Telemetry size in bytes
const uint32_t MX_RAW_FRAME_MARKER_BYTES    = 16;                           ///< Size of markers in bytes
const uint32_t MX_RAW_FRAME_PAD_BYTES = MX_RAW_FRAME_FOOTER_BYTES -
                                        MX_RAW_FRAME_META_BYTES -
                                        MX_RAW_FRAME_TELEMETRY_BYTES -
                                        MX_RAW_FRAME_MARKER_BYTES;
const uint32_t MX_RAW_FRAME_PAD_WORDS = MX_RAW_FRAME_PAD_BYTES / sizeof(uint32_t);

//------------------------------------------------------------------
/// MM-PAD/Mega-PAD raw frame data struct
#pragma pack(push, 1)
typedef struct
{
    MXRawImage image;                           ///< actual image
    STFrameMetadata metadata;                   ///< Image metadata
    STRawTelemetry telemetry;                   ///< Sensor telemetry data
    uint64_t marker1;                           ///< ST_FRAME_MARKER1
    uint32_t padding[MX_RAW_FRAME_PAD_WORDS];   ///< Padding to a full scanline
    uint64_t marker2;                           ///< ST_FRAME_MARKER2 or ST_FRAME_MARKER2_LAST
} MXRawFrame;
#pragma pack(pop)

const uint32_t MX_RAW_FRAME_BYTES = sizeof(MXRawFrame);
const uint32_t MX_RAW_FRAME_WORDS = MX_RAW_FRAME_BYTES / sizeof(int32_t);

///@} End of MM-PAD/Mega-PAD Common frame related definitions

//******************************************************************
/// @name MM-PAD Specific frame related definitions
/// @{
const uint32_t MM_SENSOR_COUNT      = MM_SUBFRAME_COUNT * ST_SUBFRAME_SENSOR_COUNT;

const uint32_t MM_RAW_IMAGE_WIDTH   = MX_RAW_IMAGE_WIDTH;  ///< pixels per row
const uint32_t MM_RAW_IMAGE_HEIGHT  = MX_RAW_IMAGE_HEIGHT; ///< rows per image
const uint32_t MM_RAW_IMAGE_PIXELS  = MM_RAW_IMAGE_HEIGHT * MM_RAW_IMAGE_WIDTH;
const uint32_t MM_RAW_IMAGE_BYTES   = MM_RAW_IMAGE_PIXELS * MX_RAW_PIXEL_BYTES;

/// @} end of MM-PAD specific definitions

//******************************************************************
/// @name MM-PAD Geocorrected frame related definitions
///@{

//------------------------------------------------------------------
/// MM-PAD Geocorrected Single raw pixel

const STDataType MM_GC_RAW_PIXEL_TYPE = DT_DOUBLE;  ///< Default pixel type
typedef double MMGCRawPixel;
const uint32_t MMGC_RAW_PIXEL_BYTES = (sizeof(MMGCRawPixel));

//------------------------------------------------------------------
// MM-PAD Geocorrected frame definitions
const uint32_t   MM_GC_RAW_IMAGE_WIDTH  = 532;        ///< pixels per row
const uint32_t   MM_GC_RAW_IMAGE_HEIGHT = 612;        ///< rows per image
const uint32_t   MM_GC_RAW_IMAGE_PIXELS = (MM_GC_RAW_IMAGE_HEIGHT * MM_GC_RAW_IMAGE_WIDTH);
const uint32_t   MM_GC_RAW_IMAGE_BYTES  = (MM_GC_RAW_IMAGE_PIXELS * MMGC_RAW_PIXEL_BYTES);

//------------------------------------------------------------------
/// MM-PAD Geocorrected raw image
/// Raw image as received from output of geocorrection procedure
///
#pragma pack(push, 1)
typedef MMGCRawPixel MMGCRawImage[MM_GC_RAW_IMAGE_HEIGHT][MM_GC_RAW_IMAGE_WIDTH];
#pragma pack(pop)

//------------------------------------------------------------------
// MM-PAD Geocorrected frame footer sizes
const uint32_t MM_GC_RAW_FRAME_FOOTER_BYTES    = MM_GC_RAW_IMAGE_WIDTH * MMGC_RAW_PIXEL_BYTES; ///< Raw footer size in bytes
const uint32_t MM_GC_RAW_FRAME_META_BYTES     = sizeof(STFrameMetadata); ///< Metadata size in bytes
const uint32_t MM_GC_RAW_FRAME_TELEMETRY_BYTES = sizeof(STRawTelemetry); ///< Telemetry size in  bytes
const uint32_t MM_GC_RAW_FRAME_MARKER_BYTES    = 16;                     ///< Size of markers in bytes
const uint32_t MM_GC_RAW_FRAME_PAD_BYTES = MM_GC_RAW_FRAME_FOOTER_BYTES -
				           MM_GC_RAW_FRAME_META_BYTES -
				           MM_GC_RAW_FRAME_TELEMETRY_BYTES -
				           MM_GC_RAW_FRAME_MARKER_BYTES;
const uint32_t MM_GC_RAW_FRAME_PAD_WORDS = MM_GC_RAW_FRAME_PAD_BYTES / sizeof(uint32_t);

//------------------------------------------------------------------
/// MM-PAD Raw Geocorrected frame data struct
#pragma pack(push, 1)
typedef struct
{
  MMGCRawImage image;                          ///< actual image
  STFrameMetadata metadata;                    ///< Image metadata
  STRawTelemetry telemetry;                    ///< Sensor telemetry data
  uint64_t marker1;                            ///< ST_FRAME_MARKER1
  uint32_t padding[MM_GC_RAW_FRAME_PAD_WORDS]; ///< Padding to a full scanline
  uint64_t marker2;                            ///< ST_FRAME_MARKER2 or ST_FRAME_MARKER2_LAST
} MMGCRawFrame;
#pragma pack(pop)

const uint32_t MM_GC_RAW_FRAME_BYTES = sizeof(MMGCRawFrame);
const uint32_t MM_GC_RAW_FRAME_WORDS = MM_GC_RAW_FRAME_BYTES / sizeof(int32_t);

///@} End of MM-PAD Geocorrected common frame related definitions


//******************************************************************
/// @name MegaPAD Specific frame related definitions
///@{

const uint32_t MG_SENSOR_COUNT      = MG_SUBFRAME_COUNT * ST_SUBFRAME_SENSOR_COUNT;

const uint32_t MG_RAW_IMAGE_WIDTH   = MX_RAW_IMAGE_WIDTH * 2;   ///< pixels per row
const uint32_t MG_RAW_IMAGE_HEIGHT  = MX_RAW_IMAGE_HEIGHT * 2;  ///< rows per image
const uint32_t MG_RAW_IMAGE_PIXELS  = MG_RAW_IMAGE_HEIGHT * MG_RAW_IMAGE_WIDTH;
const uint32_t MG_RAW_IMAGE_BYTES   = MG_RAW_IMAGE_PIXELS * MX_RAW_PIXEL_BYTES;

///@} end of MegaPAD specific definitions

//******************************************************************
/// @name KeckPAD specific frame related definitions
///@{

const uint32_t KK_SENSOR_COUNT         = KK_SUBFRAME_COUNT * ST_SUBFRAME_SENSOR_COUNT;
const uint32_t KK_MAX_CAPACITOR_COUNT  = 8;     ///< max number of frames per KeckPAD "image"

typedef uint16_t KKRawPixel;
const STDataType KK_RAW_PIXEL_TYPE  = DT_INT16;  ///< Raw pixel type
const uint32_t   KK_RAW_PIXEL_BYTES = (sizeof(KKRawPixel));

const uint32_t KK_RAW_IMAGE_WIDTH   = (512);    ///< pixels per row
const uint32_t KK_RAW_IMAGE_HEIGHT  = (512);    ///< rows per frame
const uint32_t KK_RAW_IMAGE_PIXELS  = KK_RAW_IMAGE_HEIGHT * KK_RAW_IMAGE_WIDTH;
const uint32_t KK_RAW_IMAGE_BYTES   = KK_RAW_IMAGE_PIXELS * KK_RAW_PIXEL_BYTES;

#pragma pack(push, 1)
typedef KKRawPixel KKRawImage[KK_RAW_IMAGE_HEIGHT][KK_RAW_IMAGE_WIDTH];
#pragma pack(pop)

//------------------------------------------------------------------
// KECK-PAD Raw frame footer sizes
const uint32_t KK_RAW_FRAME_FOOTER_BYTES    = KK_RAW_IMAGE_WIDTH * KK_RAW_PIXEL_BYTES; ///< Raw footer size in bytes
const uint32_t KK_RAW_FRAME_META_BYTES      = sizeof(STFrameMetadata);  ///< Metadata size in bytes
const uint32_t KK_RAW_FRAME_TELEMETRY_BYTES = sizeof(STRawTelemetry);   ///< Telemetry size in bytes
const uint32_t KK_RAW_FRAME_MARKER_BYTES    = 16;                       ///< Size of frame markers in bytes
const uint32_t KK_RAW_FRAME_PAD_BYTES = KK_RAW_FRAME_FOOTER_BYTES -
                                        KK_RAW_FRAME_META_BYTES -
                                        KK_RAW_FRAME_TELEMETRY_BYTES -
                                        KK_RAW_FRAME_MARKER_BYTES;
const uint32_t KK_RAW_FRAME_PAD_WORDS = KK_RAW_FRAME_PAD_BYTES / sizeof(uint32_t);

//------------------------------------------------------------------
/// KECK-PAD raw frame data struct
#pragma pack(push, 1)
typedef struct
{
    KKRawImage image;                           ///< actual image
    STFrameMetadata metadata;                   ///< Image metadata
    STRawTelemetry telemetry;                   ///< Sensor telemetry data
    uint64_t marker1;                           ///< 0x5555555555555555
    uint32_t padding[KK_RAW_FRAME_PAD_WORDS];   ///< Padding to a full scanline
    uint64_t marker2;                           ///< 0xEEEEEEEEEEEEEEEE or 0xFFFFFFFFFFFFFFFF
} KKRawFrame;
#pragma pack(pop)

const uint32_t KK_RAW_FRAME_BYTES = sizeof(KKRawFrame);
const uint32_t KK_RAW_FRAME_WORDS = KK_RAW_FRAME_BYTES / sizeof(int32_t);

///@} end of KeckPAD Specific definitions

namespace ST_INTERFACE
{

//******************************************************************
/// @name JSON attribute keys
///@{

#define ST_JSON_STR_SET_NAME            "SetName"
#define ST_JSON_STR_RUN_NAME            "RunName"
#define ST_JSON_STR_NAME                "Name"
#define ST_JSON_STR_TIMESTAMP           "TimeStamp"
#define ST_JSON_STR_DESCRIPTION         "Description"
#define ST_JSON_STR_SETS                "Sets"
#define ST_JSON_STR_RUNS                "Runs"
#define ST_JSON_STR_FRAME_COUNT         "FrameCount"
#define ST_JSON_STR_DMA_ERROR           "DmaError"
#define ST_JSON_STR_DISK_ERROR          "DiskError"
#define ST_JSON_STR_COMPLETION_CODE     "CompletionCode"
#define ST_JSON_STR_FRAME_BUFFER_BYTES  "FrameBufferBytes"
#define ST_JSON_STR_RAW_FRAME_BYTES     "RawFrameBytes"
#define ST_JSON_STR_RUN_TIME            "RunTime"
#define ST_JSON_STR_FRAMES_SAVED        "FramesSaved"
#define ST_JSON_STR_MAX_FRAMES          "MaxFrames"
#define ST_JSON_STR_MAX_RUN_TIME        "MaxRunTime"
#define ST_JSON_STR_CAP_COUNT           "CapCount"
#define ST_JSON_STR_CAP_SELECT          "CapSelect"

#define ST_JSON_STR_CLIENTS             "Clients"
#define ST_JSON_STR_CLIENT_ID           "ClientId"
#define ST_JSON_STR_USER_NAME           "UserName"
#define ST_JSON_STR_COMPUTER_NAME       "ComputerName"
#define ST_JSON_STR_OPERATING_SYSTEM    "OperatingSystem"
#define ST_JSON_STR_LAST_MESSAGE_TIME   "LastMessageTime"



///@)

//******************************************************************
/// @name Typedefs, Structures and Enums
/// Data types used to pass information to/from library functions
///@{
    
//---------------------------------------------
/// C++ Struct to get Capture Run status
///
typedef struct STRunStatus_struct
{
    std::string setName;         ///< User defined set Name
    std::string runName;         ///< User defined run name
    uint32_t    frameCount;      ///< Number of frames captured (so far, if still armed)
    uint32_t    maxFrames;       ///< Max number of frames configured for run (0 if no limit)
    bool        armed;           ///< true if run is currently armed
    uint32_t    badPacketCount;  ///< non-resetting, incrementing count of aurora link bad packets
    uint32_t    diskPercentFull; ///< 10x frame store percentage full
    uint32_t    driverBufsFull;  ///< # of unread buffers in the driver full of received data
    int32_t     dmaErr;          ///< current rtsup DMA, driver, and rtsup error
    int32_t     diskErr;         ///< current rtsup disk errors for most recent run
    int32_t     completionCode;  ///< code identifying reason for run completion
    uint32_t    framesSaved;     ///< number of frames saved to disk
    uint32_t    maxRunTime;      ///< max number of mSec to capture frames (0 if no limit)
    uint32_t    runTime;         ///< number of mSec since run started
    uint32_t    totalFrames;     ///< identical to maxFrames - for legacy compatibility
    uint32_t    capCount;        ///< capacitor count
    uint32_t    capSelect;       ///< capacitor select flags
    bool        noDiskSave;      ///< true if frames are NOT being saved to disk
    bool        isBackground;    ///< true if this is a background run

    STRunStatus_struct()
    {
        clear();
    }

    /// Clear the values
    void clear(void)
    {
        setName = "";
        runName = "";
        frameCount = 0;
        maxFrames = 0;
        armed = false;
        badPacketCount = 0;
        diskPercentFull = 0;
        driverBufsFull = 0;
        dmaErr = 0;
        diskErr = 0;
        completionCode = 0;
        framesSaved = 0;
        maxRunTime = 0;
        runTime = 0;
        totalFrames = 0;
        capCount = 0;
        capSelect = 0;
        noDiskSave = false;
        isBackground = false;
    }

} STRunStatus;

///@} end of typedefs

} // namespace ST_INTERFACE


//******************************************************************
// End of file
//******************************************************************

#endif //ST_IF_DEFS_H

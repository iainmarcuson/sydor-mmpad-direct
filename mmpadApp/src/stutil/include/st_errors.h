//******************************************************************
/// @file st_errors.h
/// @brief Common error status codes
/// @author      W.R. Lear
/// @date        04/20/2019
/// @copyright   Copyright (c) 2019 Sydor Technologies, 
///              All rights reserved.
///
/// This file defines return status codes common to
/// multiple projects.
/// All definitions in this file are compatible with both C and C++ 
///
//******************************************************************
#ifndef ST_ERRORS_H
#define ST_ERRORS_H

#include <cstdint>

//******************************************************************
/// Common error code definitions
///
/// These error codes are used by methods that return a 32-bit signed
/// integer error status.
///
/// Unless stated otherwise for a specific method, a negative value
/// indicates an error while a non-negative value indicates success.
/// The interpretation of success values is method specific
/// (e.g. a count or handle), but error values will be one of the codes
/// defined here.
///
/// Error codes lower than ST_ERR_BASE represent the negative of 
/// operating system errors obtained from 'errno'
///
//******************************************************************
/// @{

/// Values from -1 to ST_ERR_BASE represent system errors (-errno)
const int32_t ST_MAX_SYS_ERR     = 9999;    ///< Highest  recognized system error
const int32_t ST_ERR_BASE        = -10000;  ///< start of error codes
const int32_t ST_ERR_END         = -19999;  ///< end of error codes
const int32_t ST_STATUS_BASE     = -20000;  ///< start of status codes (not errors)
const int32_t ST_STATUS_END      = -29999;  ///< end of status codes

// Base value for converted OS error codes
const int32_t ST_ERR_SYSTEM      = -100000;  ///< add -errno to system error base

const int32_t ST_ERR_INVALID     = -999999;  ///< Invalid error code

// Generic errors
const int32_t ST_ERR_OK                 =               0;      ///< No error
const int32_t ST_ERR_UNKNOWN            = ST_ERR_BASE - 0;      ///< Unknown system error
const int32_t ST_ERR_FAIL               = ST_ERR_BASE - 1;      ///< Generic Failure
const int32_t ST_ERR_TIMEOUT            = ST_ERR_BASE - 2;      ///< Timeout
const int32_t ST_ERR_BUSY               = ST_ERR_BASE - 3;      ///< Resource is busy
const int32_t ST_ERR_NULL_PTR           = ST_ERR_BASE - 4;      ///< Null Pointer
const int32_t ST_ERR_OPTION             = ST_ERR_BASE - 12;     ///< Invalid Option
const int32_t ST_ERR_STATE              = ST_ERR_BASE - 13;     ///< Invalid state
const int32_t ST_ERR_COMPARE            = ST_ERR_BASE - 15;     ///< Data miscompare
const int32_t ST_ERR_INITIALIZE         = ST_ERR_BASE - 17;     ///< Channel group not initialized
const int32_t ST_ERR_NOT_IMPL           = ST_ERR_BASE - 20;     ///< Function not implemented
const int32_t ST_ERR_ALLOC              = ST_ERR_BASE - 50;     ///< Memory allocation failed

const int32_t ST_ERR_THREAD_CREATE      = ST_ERR_BASE - 60;     ///< Unable to create thread
const int32_t ST_ERR_THREAD_START       = ST_ERR_BASE - 61;     ///< Unable to start thread
const int32_t ST_ERR_THREAD_STOP        = ST_ERR_BASE - 62;     ///< Unable to stop thread
const int32_t ST_ERR_THREAD_STOPPING    = ST_ERR_BASE - 63;     ///< Thread stop is pending
const int32_t ST_ERR_THREAD_STOPPED     = ST_ERR_BASE - 64;     ///< Thread is not running
const int32_t ST_ERR_THREAD_LOOP_START  = ST_ERR_BASE - 65;     ///< Thread loop did not start

// Client/Server interface errors
const int32_t ST_ERR_LIB_NOT_OPEN       = ST_ERR_BASE - 100;    ///< Library is not open
const int32_t ST_ERR_LIB_OPEN           = ST_ERR_BASE - 101;    ///< Library is already open
const int32_t ST_ERR_HANDLE             = ST_ERR_BASE - 102;    ///< Invalid handle
const int32_t ST_ERR_SVR_OPEN_FAIL      = ST_ERR_BASE - 103;    ///< Unable to open connection to server
const int32_t ST_ERR_SVR_NOT_OPEN       = ST_ERR_BASE - 104;    ///< Server connection is not open
const int32_t ST_ERR_SVR_OPEN           = ST_ERR_BASE - 105;    ///< Server connection already open
const int32_t ST_ERR_MSG_CMD            = ST_ERR_BASE - 106;    ///< Unknown client message command
const int32_t ST_ERR_MSG_FORMAT         = ST_ERR_BASE - 107;    ///< Invalid message format
const int32_t ST_ERR_MSG_RESP_OK        = ST_ERR_BASE - 108;    ///< Return the generated response
const int32_t ST_ERR_NOT_AVAILABLE      = ST_ERR_BASE - 109;    ///< Requested data is not available
const int32_t ST_ERR_MSG_NOT_FOUND      = ST_ERR_BASE - 110;    ///< Requested message parameter not found
const int32_t ST_ERR_MSG_PRIVILEGE      = ST_ERR_BASE - 111;    ///< Client does not hold required privilege
const int32_t ST_ERR_RESP_FORMAT        = ST_ERR_BASE - 112;    ///< incorrect message response format
const int32_t ST_ERR_RESP_CMD           = ST_ERR_BASE - 113;    ///< incorrect message response command
const int32_t ST_ERR_MSG_INVALID        = ST_ERR_BASE - 114;    ///< Message is not valid
const int32_t ST_ERR_MSG_LENGTH         = ST_ERR_BASE - 115;    ///< Message exceeds max length
const int32_t ST_ERR_NO_SERVER          = ST_ERR_BASE - 116;    ///< No server found
const int32_t ST_ERR_ARMED              = ST_ERR_BASE - 117;    ///< Capture run is armed
const int32_t ST_ERR_NOT_ARMED          = ST_ERR_BASE - 118;    ///< Capture is not armed
const int32_t ST_ERR_RESP_CLIENT        = ST_ERR_BASE - 119;    ///< incorrect message response client handle
const int32_t ST_ERR_MSG_CLIENT         = ST_ERR_BASE - 120;    ///< message client handle not found
const int32_t ST_ERR_COMM_TIMEOUT       = ST_ERR_BASE - 121;    ///< Timeout waiting for server response

// Parameter errors
const int32_t ST_ERR_PARAM              = ST_ERR_BASE - 200;    ///< Invalid parameter
const int32_t ST_ERR_PARAM1             = ST_ERR_BASE - 201;    ///< Invalid parameter #1
const int32_t ST_ERR_PARAM2             = ST_ERR_BASE - 202;    ///< Invalid parameter #2
const int32_t ST_ERR_PARAM3             = ST_ERR_BASE - 203;    ///< Invalid parameter #3
const int32_t ST_ERR_PARAM4             = ST_ERR_BASE - 204;    ///< Invalid parameter #4
const int32_t ST_ERR_PARAM5             = ST_ERR_BASE - 205;    ///< Invalid parameter #5
const int32_t ST_ERR_PARAM6             = ST_ERR_BASE - 206;    ///< Invalid parameter #6
const int32_t ST_ERR_PARAM7             = ST_ERR_BASE - 207;    ///< Invalid parameter #7
const int32_t ST_ERR_PARAM8             = ST_ERR_BASE - 208;    ///< Invalid parameter #8
const int32_t ST_ERR_PARAM9             = ST_ERR_BASE - 209;    ///< Invalid parameter #9
const int32_t ST_ERR_PARAM_COUNT        = ST_ERR_BASE - 210;    ///< Invalid parameter count
const int32_t ST_ERR_ADDRESS            = ST_ERR_BASE - 211;    ///< Invalid address
const int32_t ST_ERR_INDEX              = ST_ERR_BASE - 212;    ///< Invalid index
const int32_t ST_ERR_LENGTH             = ST_ERR_BASE - 213;    ///< Invalid data length
const int32_t ST_ERR_DATA               = ST_ERR_BASE - 214;    ///< Invalid data value
const int32_t ST_ERR_RADIX              = ST_ERR_BASE - 215;    ///< Invalid radix
const int32_t ST_ERR_ITERATIONS         = ST_ERR_BASE - 216;    ///< Invalid iteration count
const int32_t ST_ERR_COUNT              = ST_ERR_BASE - 217;    ///< Invalid count
const int32_t ST_ERR_FRAME              = ST_ERR_BASE - 218;    ///< Invalid frame number
const int32_t ST_ERR_FRAME_COUNT        = ST_ERR_BASE - 219;    ///< Invalid frame count
const int32_t ST_ERR_FRAME_RATE         = ST_ERR_BASE - 220;    ///< Invalid sample frame rate
const int32_t ST_ERR_FRAMES_PER_FILE    = ST_ERR_BASE - 221;    ///< Invalid number of frames per files
const int32_t ST_ERR_FRAME_TYPE         = ST_ERR_BASE - 222;    ///< Invalid frame type
const int32_t ST_ERR_DATA_TYPE          = ST_ERR_BASE - 223;    ///< Invalid data type
const int32_t ST_ERR_NOT_ARRAY          = ST_ERR_BASE - 224;    ///< Parameter is not an array
const int32_t ST_ERR_DIMENSION          = ST_ERR_BASE - 225;    ///< Invalid dimension

// File system errors
const int32_t ST_ERR_NO_DISK            = ST_ERR_BASE - 300;    ///< No image disk(s) found
const int32_t ST_ERR_DIR                = ST_ERR_BASE - 301;    ///< Could not open or create directory
const int32_t ST_ERR_NOT_DIR            = ST_ERR_BASE - 302;    ///< Specified path is not a directory
const int32_t ST_ERR_IS_DIR             = ST_ERR_BASE - 303;    ///< Specified path is a directory
const int32_t ST_ERR_NOT_FILE           = ST_ERR_BASE - 304;    ///< Specified path is not a file
const int32_t ST_ERR_IS_FILE            = ST_ERR_BASE - 305;    ///< Specified path is a file
const int32_t ST_ERR_FILE_OPEN          = ST_ERR_BASE - 306;    ///< Could not open file
const int32_t ST_ERR_FILE_READ          = ST_ERR_BASE - 307;    ///< Could not read file
const int32_t ST_ERR_FILE_WRITE         = ST_ERR_BASE - 308;    ///< Could not write file
const int32_t ST_ERR_FILE_PATH          = ST_ERR_BASE - 309;    ///< Invalid file path
const int32_t ST_ERR_FILE_DELETE        = ST_ERR_BASE - 310;    ///< Could not delete file
const int32_t ST_ERR_FILE_NOT_FOUND     = ST_ERR_BASE - 311;    ///< Cound not find specified file
const int32_t ST_ERR_INPUT_STREAM       = ST_ERR_BASE - 312;    ///< Could not access input stream
const int32_t ST_ERR_OUTPUT_STREAM      = ST_ERR_BASE - 313;    ///< Could not access output stream
const int32_t ST_ERR_SYM_LINK           = ST_ERR_BASE - 314;    ///< Could not create symbolic link
const int32_t ST_ERR_FILE_SEEK          = ST_ERR_BASE - 315;    ///< Could not seek in file

// File, capture set, capture run names
const int32_t ST_ERR_NAME_EMPTY         = ST_ERR_BASE - 350;    ///< file name is empty
const int32_t ST_ERR_NAME_LENGTH        = ST_ERR_BASE - 351;    ///< file name is too long
const int32_t ST_ERR_NAME_CHAR          = ST_ERR_BASE - 352;    ///< invalid character in file name
const int32_t ST_ERR_NAME_END_DOT       = ST_ERR_BASE - 353;    ///< file name ends in '.'
const int32_t ST_ERR_NAME_NON_PRINT     = ST_ERR_BASE - 354;    ///< non-printing char in file name
const int32_t ST_ERR_SET_EXISTS         = ST_ERR_BASE - 355;    ///< Capture Set already exists
const int32_t ST_ERR_RUN_EXISTS         = ST_ERR_BASE - 356;    ///< Capture Run already exists
const int32_t ST_ERR_SET_NOT_FOUND      = ST_ERR_BASE - 357;    ///< Capture Set not found
const int32_t ST_ERR_RUN_NOT_FOUND      = ST_ERR_BASE - 358;    ///< Capture Run not found
const int32_t ST_ERR_FRAME_NOT_FOUND    = ST_ERR_BASE - 359;    ///< Frame not found
const int32_t ST_ERR_NO_RUN_STATUS      = ST_ERR_BASE - 360;    ///< Run status metadata file not present
const int32_t ST_ERR_TRIGGER_MODE       = ST_ERR_BASE - 361;    ///< Incorrect trigger mode
const int32_t ST_ERR_TRIGGER_COUNT      = ST_ERR_BASE - 362;    ///< Incorrect trigger count
const int32_t ST_ERR_FRAME_SIZE         = ST_ERR_BASE - 363;    ///< Incorrect Frame size
const int32_t ST_ERR_FRAME_STRUCTURE    = ST_ERR_BASE - 364;    ///< Incorrect frame structure
const int32_t ST_ERR_FRAME_NUMBER       = ST_ERR_BASE - 365;    ///< Invalid frame number
const int32_t ST_ERR_NO_FRAME_BUFFERS   = ST_ERR_BASE - 366;    ///< No frame buffers available, try later
const int32_t ST_ERR_SUBFRAME_ID        = ST_ERR_BASE - 367;    ///< Invalid subframe id
const int32_t ST_ERR_IMAGE_SIZE         = ST_ERR_BASE - 368;    ///< Invalid image size
const int32_t ST_ERR_FRAME_PARAMS_SIZE  = ST_ERR_BASE - 369;    ///< Invalid frame parameters size
const int32_t ST_ERR_TELEMETRY_SIZE     = ST_ERR_BASE - 370;    ///< Invalid telemetry size
const int32_t ST_ERR_SET_NAME           = ST_ERR_BASE - 371;    ///< Invalid set name
const int32_t ST_ERR_RUN_NAME           = ST_ERR_BASE - 372;    ///< Invalid run name
const int32_t ST_ERR_NO_BACKGROUND      = ST_ERR_BASE - 373;    ///< Run contains no background image
const int32_t ST_ERR_RUN_ACTIVE         = ST_ERR_BASE - 374;    ///< Run is in process
const int32_t ST_ERR_RUN_NOT_ACTIVE     = ST_ERR_BASE - 375;    ///< No run is in process
const int32_t ST_ERR_SUBFRAME_NUMBER    = ST_ERR_BASE - 376;    ///< Invalid subframe number
const int32_t ST_ERR_DISK_VOLUME        = ST_ERR_BASE - 377;    ///< Invalid disk volume
const int32_t ST_ERR_RUN_STATUS         = ST_ERR_BASE - 378;    ///< Error saving capture run status
const int32_t ST_ERR_FRAME_MARKER1      = ST_ERR_BASE - 380;    ///< Raw frame error - marker1
const int32_t ST_ERR_FRAME_MARKER2      = ST_ERR_BASE - 381;    ///< Raw frame error - marker 2
const int32_t ST_ERR_FRAME_TIMESTAMP    = ST_ERR_BASE - 382;    ///< Raw frame error - timestamp
const int32_t ST_ERR_FRAME_FRAMENUMBER  = ST_ERR_BASE - 383;    ///< Raw frame error - frame number
const int32_t ST_ERR_IMAGE_COUNT        = ST_ERR_BASE - 384;    ///< Invalid Keck image count
const int32_t ST_ERR_CAP_COUNT          = ST_ERR_BASE - 385;    ///< No Keck capacitors are selected

// Device errors
const int32_t ST_ERR_DEV_UNDEFINED      = ST_ERR_BASE - 400;    ///< device reference is undefined
const int32_t ST_ERR_DEV_OPEN           = ST_ERR_BASE - 401;    ///< Unable to open device
const int32_t ST_ERR_DEV_NOT_OPEN       = ST_ERR_BASE - 402;    ///< device is not open
const int32_t ST_ERR_DEV_READ           = ST_ERR_BASE - 403;    ///< Unable to read from device
const int32_t ST_ERR_DEV_WRITE          = ST_ERR_BASE - 404;    ///< Unable to write to device
const int32_t ST_ERR_DEV_BUSY           = ST_ERR_BASE - 405;    ///< Device is busy
const int32_t ST_ERR_DEV_DMA_FAIL       = ST_ERR_BASE - 406;    ///< Device DMA read failed
const int32_t ST_ERR_DEV_READ_ONLY      = ST_ERR_BASE - 407;    ///< Attempt to write to read only register
const int32_t ST_ERR_DEV_WRITE_ONLY     = ST_ERR_BASE - 408;    ///< Attempt to read from write only register
const int32_t ST_ERR_DEV_NACK           = ST_ERR_BASE - 409;    ///< Device reports a NACK
const int32_t ST_ERR_DEV_NACK_SOF       = ST_ERR_BASE - 410;    ///< Device reports a NACK, with the errro bit "invalid SOF"
const int32_t ST_ERR_DEV_NO_RESPONSE    = ST_ERR_BASE - 411;    ///< No response from the sensor head
const int32_t ST_ERR_DEV_SERDES         = ST_ERR_BASE - 412;    ///< Timeout waiting for SERDES alignment
const int32_t ST_ERR_DEV_COMMAND        = ST_ERR_BASE - 413;    ///< Invalid Device Maintenance Command
const int32_t ST_ERR_RESET_TIMEOUT      = ST_ERR_BASE - 414;    ///< Timeout waiting for ACP reset to complete
const int32_t ST_ERR_DEV_RMW            = ST_ERR_BASE - 415;    ///< Read/Modify/Write operation not supported
const int32_t ST_ERR_DEV_COUNT          = ST_ERR_BASE - 416;    ///< Too few host interface boards found
const int32_t ST_ERR_DMA_BUF_COUNT      = ST_ERR_BASE - 417;    ///< No DMA buffers defined
const int32_t ST_ERR_NO_POWER           = ST_ERR_BASE - 418;    ///< Sensor power is not enabled

// Quartus errors
const int32_t ST_ERR_PAD_TYPE           = ST_ERR_BASE - 450;    ///< Invalid PAD type
const int32_t ST_ERR_QUARTUS_INDEX      = ST_ERR_BASE - 451;    ///< Invalid Quartus index
const int32_t ST_ERR_QUARTUS_COUNT      = ST_ERR_BASE - 452;    ///< Invalid Quartus count
const int32_t ST_ERR_QUARTUS_CONNECTION = ST_ERR_BASE - 453;    ///< Unable to connect to Quartus
const int32_t ST_ERR_QUARTUS_0          = ST_ERR_BASE - 460;    ///< Cannot connect to Quartus 0
const int32_t ST_ERR_QUARTUS_1          = ST_ERR_BASE - 461;    ///< Cannot connect to Quartus 1
const int32_t ST_ERR_QUARTUS_2          = ST_ERR_BASE - 462;    ///< Cannot connect to Quartus 2
const int32_t ST_ERR_QUARTUS_3          = ST_ERR_BASE - 463;    ///< Cannot connect to Quartus 3
const int32_t ST_ERR_NO_QUARTII         = ST_ERR_BASE - 464;    ///< No quarti are defined
const int32_t ST_ERR_NO_HOSTIF          = ST_ERR_BASE - 465;    ///< No host interface found

// Aurora Communication Protocol Errors
const int32_t ST_ERR_ACP_NO_XCVR        = ST_ERR_BASE - 480;    ///< No transceiver connected
const int32_t ST_ERR_ACP_NO_CONNECTION  = ST_ERR_BASE - 481;    ///< Sensor head is not connected
const int32_t ST_ERR_ACP_LOST_PACKET    = ST_ERR_BASE - 482;    ///< ACP interface lost a packet
const int32_t ST_ERR_ACP_DATA_RX_OVER   = ST_ERR_BASE - 483;    ///< ACP data receive fifo overflow
const int32_t ST_ERR_ACP_DATA_TX_OVER   = ST_ERR_BASE - 484;    ///< ACP data transmit fifo overflow
const int32_t ST_ERR_ACP_IMAGE_RX_OVER  = ST_ERR_BASE - 485;    ///< ACP image receive fifo overflow
const int32_t ST_ERR_ACP_IMAGE_TX_OVER  = ST_ERR_BASE - 486;    ///< ACP image transmit fifo overflow
const int32_t ST_ERR_ACP_STATUS_ERROR   = ST_ERR_BASE - 487;    ///< ACP status register error
const int32_t ST_ERR_ACP_CHECKSUM       = ST_ERR_BASE - 488;   /// Invalid ACP Checksum
const int32_t ST_ERR_ACP_ACK_NACK       = ST_ERR_BASE - 489;   /// Invalid ACP Ack/Nack

// Command line parser errors
const int32_t ST_ERR_CMD_STATE          = ST_ERR_BASE - 500;    ///< Invalid config line parsing state
const int32_t ST_ERR_CMD_CHAR           = ST_ERR_BASE - 501;    ///< Invalid character following quoted field
const int32_t ST_ERR_CMD_COMMENT        = ST_ERR_BASE - 502;    ///< Comment start character at end of line
const int32_t ST_ERR_CMD_ESCAPE         = ST_ERR_BASE - 503;    ///< Escape character at end of line
const int32_t ST_ERR_CMD_QUOTE          = ST_ERR_BASE - 504;    ///< No closing quote found
const int32_t ST_ERR_CMD_DELIM          = ST_ERR_BASE - 505;    ///< Invalid field delimiter
const int32_t ST_ERR_CMD_FORMAT         = ST_ERR_BASE - 506;    ///< Invalid config file format
const int32_t ST_ERR_CMD_COMMA          = ST_ERR_BASE - 507;    ///< comma found in command line

const int32_t ST_ERR_CMD_UNKNOWN        = ST_ERR_BASE - 600;    ///< command interpreter - unknown command
const int32_t ST_ERR_CMD_BLANK          = ST_ERR_BASE - 601;    ///< command interpreter - blank line
const int32_t ST_ERR_CMD_EXIT_OK        = ST_ERR_BASE - 602;    ///< command interpreter - exit with OK
const int32_t ST_ERR_CMD_EXIT_FAIL      = ST_ERR_BASE - 603;    ///< command interpreter - exit with FAIL
const int32_t ST_ERR_CMD_LEVEL          = ST_ERR_BASE - 604;    ///< command interpreter nesting level exceeded
const int32_t ST_ERR_CMD_LINE           = ST_ERR_BASE - 605;    ///< invalid command line

// Data Store/Data Dictionary errors
const int32_t ST_ERR_DATA_FILE          = ST_ERR_BASE - 700;    ///< could not open data dictionary file
const int32_t ST_ERR_DATA_JSON          = ST_ERR_BASE - 701;    ///< invalid data dictionary JSON file format
const int32_t ST_ERR_DATA_UNKNOWN       = ST_ERR_BASE - 702;    ///< unknown parameter id
const int32_t ST_ERR_DATA_LOW           = ST_ERR_BASE - 703;    ///< value is below low limit
const int32_t ST_ERR_DATA_HIGH          = ST_ERR_BASE - 704;    ///< value is above high limit
const int32_t ST_ERR_DATA_INVALID       = ST_ERR_BASE - 705;    ///< value is invalid
const int32_t ST_ERR_DATA_RO            = ST_ERR_BASE - 706;    ///< parameter is read-only
const int32_t ST_ERR_DATA_WO            = ST_ERR_BASE - 707;    ///< parameter is write-only
const int32_t ST_ERR_DATA_ACCESS        = ST_ERR_BASE - 708;    ///< unable to access parameter raw value
const int32_t ST_ERR_CFG_JSON           = ST_ERR_BASE - 709;    ///< invalid config data JSON file format
const int32_t ST_ERR_INFO_JSON          = ST_ERR_BASE - 710;    ///< invalid server info JSON file format
const int32_t ST_ERR_ALIAS_PROTECTED    = ST_ERR_BASE - 711;    ///< Alias cannot be modified or deleted
const int32_t ST_ERR_DATA_RTSUP_VER     = ST_ERR_BASE - 712;    ///< Data dictionary not compatible with RTSUP version
const int32_t ST_ERR_DATA_HFPGA_VER     = ST_ERR_BASE - 713;    ///< Data dictionary not compatible with HFPGA version
const int32_t ST_ERR_DATA_SFPGA_VER     = ST_ERR_BASE - 714;    ///< Data dictionary not compatible with SFPGA version

// Named Pipe/Scripting interface errors
const int32_t ST_ERR_PIPE_OPEN_FAIL     = ST_ERR_BASE - 800;    ///< Unable to open named pipe
const int32_t ST_ERR_PIPE_OPEN          = ST_ERR_BASE - 801;    ///< Named pipe already open
const int32_t ST_ERR_PIPE_NOT_OPEN      = ST_ERR_BASE - 802;    ///< Named pipe not open
const int32_t ST_ERR_PIPE_NAME          = ST_ERR_BASE - 803;    ///< Invalid pipe name
const int32_t ST_ERR_PIPE_DATA_LEN      = ST_ERR_BASE - 804;    ///< Data length exceeds maximum
const int32_t ST_ERR_PIPE_WRITE         = ST_ERR_BASE - 805;    ///< Incomplete write to pipe
const int32_t ST_ERR_PIPE_READ          = ST_ERR_BASE - 806;    ///< Incomplete read from pipe
const int32_t ST_ERR_PIPE_VALUE         = ST_ERR_BASE - 807;    ///< Invalid value read from pipe
const int32_t ST_ERR_SCRIPT_WIN         = ST_ERR_BASE - 808;    ///< Scripting not supported in windows

// Run Status codes - NOT ERRORS
const int32_t ST_RUN_STATUS_ARMED           = ST_STATUS_BASE - 1;   ///< Run is armed
const int32_t ST_RUN_STATUS_NONSTOP         = ST_STATUS_BASE - 2;   ///< Run has no termination criteria
const int32_t ST_RUN_STATUS_STOPPED         = ST_STATUS_BASE - 3;   ///< Run terminated by stopRun command
const int32_t ST_RUN_STATUS_MAX_FRAME_COUNT = ST_STATUS_BASE - 4;   ///< Run terminated by max frame count
const int32_t ST_RUN_STATUS_MAX_RUN_TIME    = ST_STATUS_BASE - 5;   ///< Run terminated by max run time
const int32_t ST_RUN_STATUS_WAITING         = ST_STATUS_BASE - 6;   ///< Run waiting for frames
const int32_t ST_RUN_STATUS_DMA_ERROR       = ST_STATUS_BASE - 7;   ///< Run terminated by DMA error
const int32_t ST_RUN_STATUS_DISK_FULL       = ST_STATUS_BASE - 8;   ///< Run terminated because disk is full
const int32_t ST_RUN_STATUS_FRAME_ERROR     = ST_STATUS_BASE - 9;   ///< Run terminated due to frame format error
const int32_t ST_RUN_STATUS_UNKNOWN         = ST_STATUS_BASE -99;   ///< Run completion status is unknown;

// Program level status coded - NOT ERRORS
const int32_t ST_STATUS_EXIT_OK             = ST_STATUS_BASE - 100; ///< Request program exit

/// @}

//******************************************************************
// End of file
//******************************************************************
#endif // ST_ERROR_H

//******************************************************************
/// @file stutil_logger.h
/// @brief Debug logging singleton class
/// @author      W.R. Lear
/// @date        09/07/2019
/// @copyright   Copyright (c) 2019 Sydor Technologies, 
///              All rights reserved.
///
//******************************************************************
#ifndef STUTIL_LOGGER_H
#define STUTIL_LOGGER_H

#include <iostream>
#include <fstream>
#include <mutex>
//#include <stdarg.h>

namespace STUTIL
{

//******************************************************************
// Definitions and Constants
//******************************************************************
const uint32_t LOG_DEF_MAX_FILES = 10;
const uint32_t LOG_DEF_MAX_KB = 10000;

const uint32_t LOG_MAX_MAX_FILES = 100;
const uint32_t LOG_MIN_KB = 1;
const uint32_t LOG_MIN_MAX_KB = 10;
const uint32_t LOG_MAX_MAX_KB = 1000000;

const uint32_t LOG_MAX_LINE_BYTES = 4096;

#define LOG_DEF_EXTENSION ".log"
#define LOG_DEF_DIR "."

//******************************************************************
// Log levels
//******************************************************************
const uint32_t LOG_NONE      = 0;            ///< No messages
const uint32_t LOG_FATAL     = 1;            ///< Fatal Error messages
const uint32_t LOG_ERROR     = 2;            ///< Error messages
const uint32_t LOG_WARNING   = 3;            ///< Warning messages
const uint32_t LOG_INFO      = 4;            ///< Informational messages
const uint32_t LOG_TRACE     = 5;            ///< Function trace messages
const uint32_t LOG_INFO1     = 6;            ///< Low level informational messages
const uint32_t LOG_TRACE1    = 7;            ///< Low level trace messages
const uint32_t LOG_INFO2     = 8;            ///< Very low level informational messages
const uint32_t LOG_DEBUG     = 9;            ///< Diagnostic/Debug messages
const uint32_t LOG_DEBUG1    = 10;           ///< Low level Diagnostic/Debug messages
const uint32_t LOG_DEBUG2    = 11;           ///< Very low level Diagnostic/Debug messages
const uint32_t LOG_DEBUG3    = 12;           ///< Extremely low level Diagnostic/Debug messages
const uint32_t LOG_ALL       = 12;           ///< All messages
const uint32_t LOG_DEFAULT   = LOG_INFO;     ///< Default log level


//******************************************************************
// Log Macros
//******************************************************************
// For logging macros to work, all pointers to the logger instance
// must have the same name, defined by LOGPTR. This should be defined
// at the project level
//
// To disable all logging, define DISABLE_LOGGING at the project
// or file level.

#ifndef LOGPTR
    #define LOGPTR pLog         ///< Default (and recommended) log pointer name
#endif

//#define DISABLE_LOGGING       ///< Define to disable ALL logging
#define ENABLE_VERBOSE_LOGGING  ///< Define to enable verbose logging levels
#define USE_ONLY_LOGX           ///< Define to show file and line no. on all messages

#ifdef DISABLE_LOGGING
    // Standard logging
    #define LOGFATAL( ... ) (void)0
    #define LOGERROR( ... ) (void)0
    #define LOGWARN(  ... ) (void)0
    #define LOGINFO(  ... ) (void)0
    #define LOGTRACE( ... ) (void)0
    #define LOGDEBUG( ... ) (void)0
    #define LOGWRITE( msg ) (void)0
    // Verbose logging
    #define LOGINFO1(  ...) (void)0
    #define LOGTRACE1( ...) (void)0
    #define LOGINFO2(  ...) (void)0
    #define LOGDEBUG1( ...) (void)0
    #define LOGDEBUG2( ...) (void)0
    #define LOGDEBUG3( ...) (void)0
    #define LOGWRITE1( msg) (void)0
    #define LOGWRITE2( msg) (void)0
    #define LOGWRITE3( msg) (void)0
// Conditional Logging
    #define LOGERROR_IF( ...) (void)0
    #define LOGIWARN_IF( ...) (void)0
    #define LOGINFO_IF(  ...) (void)0
    #define LOGTRACE_IF( ...) (void)0
    #define LOGDEBUG_IF( ...) (void)0
#else
    #ifdef USE_ONLY_LOGX
        // Normal logging
        #define LOGFATAL( ... )  LOGxFATAL( __VA_ARGS__ )
        #define LOGERROR( ... )  LOGxERROR( __VA_ARGS__ )
        #define LOGWARN(  ... )  LOGxWARN( __VA_ARGS__ )
        #define LOGINFO(  ... )  LOGxINFO( __VA_ARGS__ )
        #define LOGTRACE( ... )  LOGxTRACE( __VA_ARGS__ )
        #define LOGDEBUG( ... )  LOGxDEBUG( __VA_ARGS__ )
        #define LOGWRITE( msg )  LOGxWRITE(msg)
        // Conditional Logging
        #define LOGERROR_IF(flg, ... ) LOGxERROR_IF(flg, __VA_ARGS__ )
        #define LOGWARN_IF( flg, ... ) LOGxWARN_IF(flg,  __VA_ARGS__ )
        #define LOGINFO_IF( flg, ... ) LOGxINFO_IF(flg, __VA_ARGS__ )
        #define LOGTRACE_IF(flg, ... ) LOGxTRACE_IF(flg, __VA_ARGS__ )
        #define LOGDEBUG_IF(flg, ... ) LOGxDEBUG_IF(flg, __VA_ARGS__ )
        // Verbose logging
        #define LOGINFO1(  ...)  LOGxINFO1( __VA_ARGS__ )
        #define LOGTRACE1( ...)  LOGxTRACE1( __VA_ARGS__ )
        #define LOGINFO2(  ...)  LOGxINFO2( __VA_ARGS__ )
        #define LOGDEBUG1( ...)  LOGxDEBUG1( __VA_ARGS__ )
        #define LOGDEBUG2( ...)  LOGxDEBUG2( __VA_ARGS__ )
        #define LOGDEBUG3( ...)  LOGxDEBUG3( __VA_ARGS__ )
        #define LOGWRITE1( msg ) LOGxWRITE1(msg)
        #define LOGWRITE2( msg ) LOGxWRITE2(msg)
        #define LOGWRITE3( msg ) LOGxWRITE3(msg)

    #else
        // Normal logging
        #define LOGFATAL( ... )  if(LOGPTR) { LOGPTR->addLog(STUTIL::LOG_FATAL,   __VA_ARGS__ ); } (void)0
        #define LOGERROR( ... )  if(LOGPTR) { LOGPTR->addLog(STUTIL::LOG_ERROR,   __VA_ARGS__ ); } (void)0
        #define LOGWARN(  ... )  if(LOGPTR) { LOGPTR->addLog(STUTIL::LOG_WARNING, __VA_ARGS__ ); } (void)0
        #define LOGINFO(  ... )  if(LOGPTR) { LOGPTR->addLog(STUTIL::LOG_INFO,    __VA_ARGS__ ); } (void)0
        #define LOGTRACE( ... )  if(LOGPTR) { LOGPTR->addLog(STUTIL::LOG_TRACE,   __VA_ARGS__ ); } (void)0
        #define LOGDEBUG( ... )  if(LOGPTR) { LOGPTR->addLog(STUTIL::LOG_DEBUG,   __VA_ARGS__ ); } (void)0
        #define LOGWRITE( msg )  if(LOGPTR) { LOGPTR->logWrite(STUTIL::LOG_NONE, msg); }
        // Conditional Logging
        #define LOGERROR_IF(flg, ... )  if((flg) && LOGPTR) { LOGPTR->addLog(STUTIL::LOG_ERROR,   __VA_ARGS__ ); } (void)0
        #define LOGWARN_IF( flg, ... )  if((flg) && LOGPTR) { LOGPTR->addLog(STUTIL::LOG_WARNING, __VA_ARGS__ ); } (void)0
        #define LOGINFO_IF( flg, ... )  if((flg) && LOGPTR) { LOGPTR->addLog(STUTIL::LOG_INFO,    __VA_ARGS__ ); } (void)0
        #define LOGTRACE_IF(flg, ... )  if((flg) && LOGPTR) { LOGPTR->addLog(STUTIL::LOG_TRACE,   __VA_ARGS__ ); } (void)0
        #define LOGDEBUG_IF(flg, ... )  if((flg) && LOGPTR) { LOGPTR->addLog(STUTIL::LOG_DEBUG,   __VA_ARGS__ ); } (void)0
        #ifdef ENABLE_VERBOSE_LOGGING
            // Verbose logging enabled
            #define LOGINFO1(  ... ) if(LOGPTR) { LOGPTR->addLog(STUTIL::LOG_INFO1,  __VA_ARGS__ ); } (void)0
            #define LOGTRACE1( ... ) if(LOGPTR) { LOGPTR->addLog(STUTIL::LOG_TRACE1, __VA_ARGS__ ); } (void)0
            #define LOGINFO2(  ... ) if(LOGPTR) { LOGPTR->addLog(STUTIL::LOG_INFO2,  __VA_ARGS__ ); } (void)0
            #define LOGDEBUG1( ... ) if(LOGPTR) { LOGPTR->addLog(STUTIL::LOG_DEBUG1, __VA_ARGS__ ); } (void)0
            #define LOGDEBUG2( ... ) if(LOGPTR) { LOGPTR->addLog(STUTIL::LOG_DEBUG2, __VA_ARGS__ ); } (void)0
            #define LOGDEBUG2( ... ) if(LOGPTR) { LOGPTR->addLog(STUTIL::LOG_DEBUG3, __VA_ARGS__ ); } (void)0
            #define LOGWRITE1( msg ) if(LOGPTR) { LOGPTR->logWrite(STUTIL::LOG_DEBUG1, msg); }
            #define LOGWRITE2( msg ) if(LOGPTR) { LOGPTR->logWrite(STUTIL::LOG_DEBUG2, msg); }
            #define LOGWRITE3( msg ) if(LOGPTR) { LOGPTR->logWrite(STUTIL::LOG_DEBUG3, msg); }
#else
            // Verbose logging disabled
            #define LOGINFO1(  ... ) (void)0
            #define LOGTRACE1( ... ) (void)0
            #define LOGDEBUG1( ... ) (void)0
            #define LOGDEBUG2( ... ) (void)0
            #define LOGDEBUG3( ... ) (void)0
            #define LOGWRITE1(...) (void)0
            #define LOGWRITE2(...) (void)0
            #define LOGWRITE3(...) (void)0
            #endif // ENABLE_EXTENDED_LOGGING
    #endif // USE_ONLY_LOGX
#endif


#ifdef DISABLE_LOGGING
    // Standard logging
    #define LOGxFATAL( ... ) (void)0
    #define LOGxERROR( ... ) (void)0
    #define LOGxWARN(  ... ) (void)0
    #define LOGxINFO(  ... ) (void)0
    #define LOGxTRACE( ... ) (void)0
    #define LOGxDEBUG( ... ) (void)0
    #define LOGxWRITE( msg ) (void)0
    // Conditional logging
    #define LOGxERROR_IF( ... ) (void)0
    #define LOGxIWARN_IF( ... ) (void)0
    #define LOGxINFO_IF(  ... ) (void)0
    #define LOGxTRACE_IF( ... ) (void)0
    #define LOGxDEBUG_IF( ... ) (void)0
    // Verbose logging
    #define LOGxINFO1(  ...) (void)0
    #define LOGxTRACE1( ...) (void)0
    #define LOGxINFO2(  ...) (void)0
    #define LOGxDEBUG1( ...) (void)0
    #define LOGxDEBUG2( ...) (void)0
    #define LOGxDEBUG3( ...) (void)0
    #define LOGxWRITE1( msg) (void)0
    #define LOGxWRITE2( msg) (void)0
    #define LOGxWRITE3( msg) (void)0
#else
    // normal logging
    #define LOGxFATAL( ... )  if(LOGPTR) { LOGPTR->addLogExtra(STUTIL::LOG_FATAL,   __FILE__, __LINE__, __VA_ARGS__ ); } (void)0
    #define LOGxERROR( ... )  if(LOGPTR) { LOGPTR->addLogExtra(STUTIL::LOG_ERROR,   __FILE__, __LINE__, __VA_ARGS__ ); } (void)0
    #define LOGxWARN(  ... )  if(LOGPTR) { LOGPTR->addLogExtra(STUTIL::LOG_WARNING, __FILE__, __LINE__, __VA_ARGS__ ); } (void)0
    #define LOGxINFO(  ... )  if(LOGPTR) { LOGPTR->addLogExtra(STUTIL::LOG_INFO,    __FILE__, __LINE__, __VA_ARGS__ ); } (void)0
    #define LOGxTRACE( ... )  if(LOGPTR) { LOGPTR->addLogExtra(STUTIL::LOG_TRACE,   __FILE__, __LINE__, __VA_ARGS__ ); } (void)0
    #define LOGxDEBUG( ... )  if(LOGPTR) { LOGPTR->addLogExtra(STUTIL::LOG_DEBUG,   __FILE__, __LINE__, __VA_ARGS__ ); } (void)0
    #define LOGxWRITE( msg )  if(LOGPTR) { LOGPTR->logWrite(STUTIL::LOG_NONE, msg); } (void)0
    // Conditional logging
    #define LOGxERROR_IF(flg, ... )  if((flg) && LOGPTR) { LOGPTR->addLogExtra(STUTIL::LOG_ERROR,   __FILE__, __LINE__, __VA_ARGS__ ); } (void)0
    #define LOGxWARN_IF( flg, ... )  if((flg) && LOGPTR) { LOGPTR->addLogExtra(STUTIL::LOG_WARNING, __FILE__, __LINE__, __VA_ARGS__ ); } (void)0
    #define LOGxINFO_IF( flg, ... )  if((flg) && LOGPTR) { LOGPTR->addLogExtra(STUTIL::LOG_INFO,    __FILE__, __LINE__, __VA_ARGS__ ); } (void)0
    #define LOGxTRACE_IF(flg, ... )  if((flg) && LOGPTR) { LOGPTR->addLogExtra(STUTIL::LOG_TRACE,   __FILE__, __LINE__, __VA_ARGS__ ); } (void)0
    #define LOGxDEBUG_IF(flg, ... )  if((flg) && LOGPTR) { LOGPTR->addLogExtra(STUTIL::LOG_DEBUG,   __FILE__, __LINE__, __VA_ARGS__ ); } (void)0
    #ifdef ENABLE_VERBOSE_LOGGING
        // Verbose logging enabled
        #define LOGxINFO1(  ... ) if(LOGPTR) { LOGPTR->addLogExtra(STUTIL::LOG_INFO1,  __FILE__, __LINE__, __VA_ARGS__ ); } (void)0
        #define LOGxTRACE1( ... ) if(LOGPTR) { LOGPTR->addLogExtra(STUTIL::LOG_TRACE1, __FILE__, __LINE__, __VA_ARGS__ ); } (void)0
        #define LOGxINFO2(  ... ) if(LOGPTR) { LOGPTR->addLogExtra(STUTIL::LOG_INFO2,  __FILE__, __LINE__, __VA_ARGS__ ); } (void)0
        #define LOGxDEBUG1( ... ) if(LOGPTR) { LOGPTR->addLogExtra(STUTIL::LOG_DEBUG1, __FILE__, __LINE__, __VA_ARGS__ ); } (void)0
        #define LOGxDEBUG2( ... ) if(LOGPTR) { LOGPTR->addLogExtra(STUTIL::LOG_DEBUG2, __FILE__, __LINE__, __VA_ARGS__ ); } (void)0
        #define LOGxDEBUG3( ... ) if(LOGPTR) { LOGPTR->addLogExtra(STUTIL::LOG_DEBUG3, __FILE__, __LINE__, __VA_ARGS__ ); } (void)0
        #define LOGxWRITE1( msg ) if(LOGPTR) { LOGPTR->logWrite(STUTIL::LOG_DEBUG1, msg); } (void)0
        #define LOGxWRITE2( msg ) if(LOGPTR) { LOGPTR->logWrite(STUTIL::LOG_DEBUG2, msg); } (void)0
        #define LOGxWRITE3( msg ) if(LOGPTR) { LOGPTR->logWrite(STUTIL::LOG_DEBUG3, msg); } (void)0
#else
        // Verbose logging disabled
        #define LOGxINFO1(  ... ) (void)0
        #define LOGxTRACE1( ... ) (void)0
        #define LOGxINFO2(  ... ) (void)0
        #define LOGxDEBUG1( ... ) (void)0
        #define LOGxDEBUG2( ... ) (void)0
        #define LOGxDEBUG3( ... ) (void)0
        #define LOGxWRITE1( ... ) (void)0
        #define LOGxWRITE2( ... ) (void)0
        #define LOGxWRITE3( ... ) (void)0
#endif

#endif

//******************************************************************
// Data structures, enumerations and type definitions
//******************************************************************

//******************************************************************
// Class Definitions
//******************************************************************

//------------------------------------------------------------------
/// Debug Log Class
/// This class provides a basic debug logging capability.
///
class Logger
{
private:
    static Logger *sInstance;              ///< Singleton logger instance
    static bool sInitialized;              ///< true if logger has been initialized
    static std::mutex sInstanceCS;         ///< mutex to restrict access during getInstance   

    std::string mLogDir;                   ///< Log directory
    std::string mBaseName;                 ///< Log file base name
    std::string mBasePath;                 ///< Log file base path
    std::string mLogFilePath;              ///< Full Path to current log file
    std::string mId;                       ///< Optional Module Identifer string
    uint32_t mMaxFiles;                    ///< Maximum number of log files to retain
    size_t mMaxKb;                         ///< Approximate Maximum file size in KBytes
    uint32_t mLogLevel;                    ///< Current log level threshold
    uint32_t mStdOutLevel;                 ///< Current stdout log level threshold

    size_t mCurSize;                       ///< Current log file size;
    bool mEnabled;                         ///< True if logging is enabled
    std::fstream mFile;                    ///< Log file stream
    std::recursive_mutex mLogCS;           ///< Logger critical section mutex

private:
    // Constructor/Destructor are private - apps must use getInstance()
    Logger(void);                           ///< private default constructor
    Logger(Logger const&);                  ///< private copy constructor
    //Logger& operator=(Logger const&) {};    ///< private assignment operator

    static const char* sLogLevelNames[];

public:
    virtual ~Logger(void);                  ///< destructor

    /// check if logger has been instantiated and initialized
    static bool exists(void) { return ((sInstance != nullptr) && sInitialized); }

    /// get singleton instance of logger
    /// Instantiates the first logger instance, or returns the existing instance.
    /// The first call to this method also initialized the logger.
    /// All parameters are ignored on subsequent calls.
    ///
    /// @param[in] basename              Base name for logfiles (default="")
    /// @param[in] logdir                Path of directory containing log files (default=".")
    /// @param[in] maxfiles              Maximum number of log files to retain (default=stutil_LOG_MAX_FILES)
    /// @param[in] maxkb                 Approximate maximum file size in KBytes (default=STUTIL_LOG_DEF_MAX_KB)
    ///
    /// @returns pointer to singleton Logger instance
    ///
    /// @note, Logging will be disabled if base name is null or empty, logdir is not writeable,
    /// maxfiles=0, or maxkb=0
    ///
    static Logger *getInstance(const char* basename = "",
        const char* logdir = LOG_DEF_DIR,
        uint32_t maxfiles = LOG_DEF_MAX_FILES,
        uint32_t maxkb = LOG_DEF_MAX_KB);

    /// return true if Logger has been instantiated
    static bool isValid(void) { return (sInstance != nullptr); }

    /// Get path of the directory in which log files are written
    std::string getLogDir(void) {return mLogDir;}

    /// Get base log file name - without timestamp or extension
    std::string getBaseName(void) {return mBaseName;}

    /// Get the current log file path
    std::string getLogFilePath(void) {return mLogFilePath;}

    /// Get maximum  number of retained log files
    uint32_t getMaxFiles(void) {return mMaxFiles;}

    /// Get approximate maximum file size in KBytes
    uint32_t getMaxKb(void) {return static_cast<uint32_t>(mMaxKb);}

    /// Get the current log Identifier string
    std::string getLogId(void) {return mId;}

    /// Set a log Identifier string to appear in each message
    void setLogId(const std::string& id = "") {mId = id;}

    /** Set the log level threshold.
        * Specifies which log message levels will be logged.
        * The lower the level, the fewer messages will be logged.<br>
        * When level is 0, all messages are inhibited.
        * When level is >= STUTIL_LOG_ALL, all messages are logged
        *
        * @param[in] loglevel              Highest log level to be logged (default=STUTIL_LOG_DEFAULT)
        */
    void setLogLevel(uint32_t loglevel=LOG_DEFAULT);

    /// Get the current log level threshold
    uint32_t getLogLevel(void) {return mLogLevel;}

    /// Get the current log level name
    static const char* getLogLevelName(uint32_t level);

    /** Set the stdout log level threshold.
        * When a message is written to the log file, it will also be written to stdout
        * if the message level is lower the stdout log level threshold
        *
        * @param[in] level                 Highest log level to be written to stdout (default=0)
        */
    void setStdOutLevel(uint32_t level=0) {mStdOutLevel = level;}

    /// Get the current stdout log level threshhold
    uint32_t getStdOutLevel(void) {return mStdOutLevel;}

    // Return true if logging is enabled
    bool isEnabled(void) {return mEnabled;}

    /** Write a message to the log file
    * If logging is enabled and the specified message level is
    * lower than or equal to the current log level threshold,
    * the message will be formatted and written to the log file
    * @param level                     message level
    * @param format                    message format string
    * @param ...                       optional list of arguments for format string
    */
    void addLog(uint32_t level, const char* format, ...);

    /** Write a message to the log file, including file name and line number
    * If logging is enabled and the specified message level is
    * lower than or equal to the current log level threshold,
    * the message will be formatted and written to the log file
    * @param level                     message level
    * @param fileName                  name of source file from where the message was logged
    * @param lineNum                   source file line number from where the message was logged
    * @param format                    message format string
    * @param ...                       optional list of arguments for format string
    */
    void addLogExtra(uint32_t level, const char * fileName, int lineNum, const char* format, ...);

    /** Write a message to the log file and return the specified status
    * If logging is enabled and the current log level threshold is higher
    * than STUTIL_LOG_ERROR, the message will be formatted and written to the log file.
    *
    * @param status                    error status to return
    * @param format                    message format string
    * @param ...                       optional list of arguments for format string
    *
    * @returns status
    */
    int32_t statusLog(int32_t status, const char* format, ...);

    /** Write a message to the log file using a varargs list
        * If logging is enabled and the specified message level is
        * lower than or equal to the current log level threshold,
        * the message will be formatted and written to the log file
        * @param level                     message level
        * @param format                    message format string
        * @param ap                        varargs list
        */
    void logV(uint32_t level, const char* format, va_list ap);

        // Write a string to the logfile
    void logWrite(uint32_t level, const std::string& line);

    /** Write a message to the log file without regard to log level
    * @param level                     log level
    * @param id                        identifier string
    * @param format                    message format string
    * @param ...                       optional list of arguments for format string
    */
    void logWrite(uint32_t level, const char* id, const char* format, ...);

    /** Write a message to the log file without regard to log level
        * @param level                     log level
        * @param id                        identifier string
        * @param format                    message format string
        * @param ap                        varargs list
        */
    void logWriteV(uint32_t level, const char* id, const char* format, va_list ap);

    void logWriteV(uint32_t level, uint32_t stdoutlevel, const char* id, const char* format, va_list ap);

private:
    /** Initialize the logger instance
      */
    int32_t initLogger(const char* basename = "",
                        const char* logdir = LOG_DEF_DIR,
                        uint32_t maxfiles = LOG_DEF_MAX_FILES,
                        uint32_t maxkb = LOG_DEF_MAX_KB);

    /** Get a string representation of the current time
        * @returns pointer to formatted timestamp
        */
    std::string getTimeStampStr();

    /// Get a string representation of the specified log level
    std::string getLevelStr(uint32_t level);

    /** Open and initialize a log file
        * Closes any currently opened log file, creates a new log file in the log directory,
        * and deletes the oldest file if the maximum number of log files has been exceeded.
        *
        * @param[in] checkExisting     Try to open existing file if true (default = false)
        * @returns false if a file cannot be created.
        */
    bool openFile(bool checkExisting = false);

    /// Close the current log file, if one is open
    void closeFile(void);

    // Write the header and specified string to logfile
    void writeFile(const std::string& header, const std::string& line);

};

} //namespace STUTIL

//******************************************************************
// End of file
//******************************************************************
#endif // STUTIL_LOGGER_H

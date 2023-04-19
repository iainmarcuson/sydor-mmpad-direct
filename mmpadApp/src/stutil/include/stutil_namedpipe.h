//******************************************************************
/// @file stutil_namedpipe.h
/// @brief Cross Platform named pipe IPC class
/// @author      W.R. Lear
/// @date        01/05/2020
/// @copyright   Copyright (c) 2020 Sydor Technologies, 
///              All rights reserved.
///
/// This is a lightweight cross-platform inter-process communication
/// class based on Linux fifos or Windows named pipes
///
//******************************************************************
#ifndef STUTIL_NAMED_PIPE_H
#define STUTIL_NAMED_PIPE_H

#include <string>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

namespace STUTIL
{

//******************************************************************
// Definitions and Constants
//******************************************************************
    #define CTOS_SUFFIX  "_ctos"    ///< Client to Server pipe name suffix
    #define STOC_SUFFIX  "_stoc"    ///< Server to Client pipe name suffix

#define PIPE_OUT_BUFFER_SIZE  (512 * 1024)
#define PIPE_IN_BUFFER_SIZE   (512 * 1024)
#define PIPE_DEFAULT_TIMEOUT  (100)
    
#ifdef _WIN32
    #define PIPE_DIR  "\\\\.\\pipe\\"
    #define INVALID_PIPE INVALID_HANDLE_VALUE
    typedef HANDLE HPIPE;
#else
    #define PIPE_DIR  "/tmp/"
    #define INVALID_PIPE (-1)
    typedef int HPIPE;
#endif

/// PIPE OPEN MODE
typedef enum
{
    PIPE_RO = 0,
    PIPE_WO = 1,
    PIPE_RW = 2
} PIPE_OPEN_MODE;


//******************************************************************
// Class Definitions
//******************************************************************

//------------------------------------------------------------------
/// NamedPipe Class
class NamedPipe
{
protected:
    bool mIsServer;         ///< true if this is the server side of a pipe 
    std::string mReadName;  ///< Incoming pipe name
    HPIPE mReadPipe;        ///< Incoming pipe handle or descriptor
    std::string mWriteName; ///< Outgoing pipe name
    HPIPE mWritePipe;       ///< Outgoing pipe handle or descriptor
   
    //----------------------------------------------
    /// create a new pipe
    ///
    /// @param[in] path     path of pipe to create
    /// @param[out] hpipe   receives pipe handle if successful
    /// @param[in] mode     pipe open mode (ro, wo, rw)
    ///
    /// @returns 0 on success, else negative error code
    int32_t createPipe(const std::string& path, HPIPE& hpipe, PIPE_OPEN_MODE mode);
    
    //----------------------------------------------
    /// open an existing pipe, if not already open
    ///
    /// @param[in] path     path of pipe to open
    /// @param[out] hpipe   receives pipe handle if successful
    /// @param[in] mode     pipe open mode (ro, wo, rw)
    ///
    /// @returns 0 on success, else negative error code
    int32_t openPipe(const std::string& path, HPIPE& hpipe, PIPE_OPEN_MODE mode);
    
    //----------------------------------------------
    /// close an open pipe, if not already closed
    ///
    /// @param[in] hpipe    handle of pipe to close
    ///
    void closePipe(HPIPE hpipe);
    
public:
    //----------------------------------------------
    /// Constructor - create or open the specified pipe pair
    NamedPipe();
    
    //----------------------------------------------
    /// Destructor - close the specified pipe pair
    ~NamedPipe();

    //----------------------------------------------
    /// Open or create the specified named pipe pair
    ///
    /// @returns 0 on success, else negative error code
    int32_t open(const std::string& name, bool isServer = false);
    
    //----------------------------------------------
    /// close the pipe pair, if open
    ///
    void close();

    //----------------------------------------------
    /// Wait for the client to connect to the pipe.
    ///
    /// Needed only on the server end. Has no effect 
    /// On the client end.
    ///
    int32_t waitForConnection();

    //----------------------------------------------
    /// End a server connection
    ///
    /// Needed only on the server end. Has no effect 
    /// On the client end.
    ///
    int32_t disconnect();

    //----------------------------------------------
    /// read bytes from the incoming pipe
    ///
    /// @param[out] buf         buffer to receive data
    /// @param[in]  maxBytes    maximum number of bytes to read
    ///
    /// @returns actual number of bytes read on success, else negative error code
    int32_t read(void* buf, uint32_t maxBytes);

    //----------------------------------------------
    /// read a string from the incoming pipe
    ///
    /// @param[out] str         string to receive data
    /// @param[in]  maxBytes    maximum number of bytes to read
    ///
    /// @returns actual number of bytes read on success, else negative error code
    int32_t read(std::string& str, uint32_t maxBytes);

    //----------------------------------------------
    /// read a packet (length + data) from the incoming pipe
    ///
    /// @param[out] str         string to receive data
    ///
    /// @returns 0 on success, else negative error code
    int32_t readPacket(std::string& str);

    //----------------------------------------------
    /// read a 32-bit hexadecimal integer from the incoming pipe
    ///
    /// @param[in] value       variable to receive value
    ///
    /// @returns 0 on success, else negative error code
    int32_t readHexInt32(int32_t& value);

    //----------------------------------------------
    /// write bytes to the outgoing pipe
    ///
    /// @param[in] buf         buffer containing bytes to write
    /// @param[in] nBytes      number of bytes to write
    ///
    /// @returns actual number of bytes written on success, else negative error code
    int32_t write(const void* buf, uint32_t nBytes);

    //----------------------------------------------
    /// write a string to the outgoing pipe
    ///
    /// @param[in] str         string to write
    ///
    /// @returns actual number of bytes written on success, else negative error code
    int32_t write(const std::string str);

    //----------------------------------------------
    /// write a packet (length + data) to the outgoing pipe
    ///
    /// @param[in] str         string to write
    ///
    /// @returns 0 on success, else negative error code
    int32_t writePacket(const std::string str);

    //----------------------------------------------
    /// write a 32-bit hexadecimal integer value to the outgoing pipe
    ///
    /// @param[in] value        value to write
    ///
    /// @returns 0 on success, else negative error code
    int32_t writeHexInt32(int32_t value);

};

} //namespace STUTIL

//******************************************************************
// End of file
//******************************************************************
#endif // STUTIL_NAMED_PIPE_H

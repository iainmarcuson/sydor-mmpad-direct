//******************************************************************
/// @file stutil_file.h
/// @brief File related utility Functions
/// @author      W.R. Lear
/// @date        04/20/2019
/// @copyright   Copyright (c) 2019 Sydor Technologies, 
///              All rights reserved.
///
/// utility functions related to directories and files
///
//******************************************************************
#ifndef STUTIL_FILE_H
#define STUTIL_FILE_H

#include <string>
#include <vector>

namespace STUTIL
{

//******************************************************************
// Definitions and Constants
//******************************************************************
#define STUTIL_MAX_PATH_LENGTH (257)

//******************************************************************
// Data structures, enumerations and type definitions
//******************************************************************

typedef struct
{
    bool     valid;         ///< true if info is valid
    bool     isDir;         ///< true if this is a directory
    bool     isFile;        ///< true if this is a regular file;
    uint64_t size;          ///< file size in bytes
    uint64_t ctime;         ///< creation time
    uint64_t mtime;         ///< modification time
} FileInfo;

//******************************************************************
// File Function Definitions
//******************************************************************

//------------------------------------------------------------------
/** Check if a path is a directory
 *
 * @param[in] path           String containing relative or absolute directory path
 *
 * @returns true if path is a directory
 */
bool isDir(const std::string& path);

//------------------------------------------------------------------
/** Check if a path is a file
 *
 * @param[in] path           String containing relative or absolute file path
 *
 * @returns true if path is a file
 */
bool isFile(const std::string& path);

//------------------------------------------------------------------
/** Check if file exists
* 
* @param[in] path           String containing relative or absolute file path
* 
* @returns true if path is a file or directory
*/
bool exists(const std::string& path);

//------------------------------------------------------------------
/** Check if file is readable
*
* @param[in] path           String containing relative or absolute file path
*
* @returns true if path exists and is readable
*/
bool isReadable(const std::string& path);

//------------------------------------------------------------------
/** Check if file is writeable
*
* @param[in] path           String containing relative or absolute file path
*
* @returns true if path exists and is writeable
*/
bool isWriteable(const std::string& path);

//------------------------------------------------------------------
/** Check if a file exists in a specified directory
 *
 * @param[in] dir           String containing relative or absolute directory path
 * @param[in] fname         String containing file name
 *
 * @returns true if file exists in the directory
 */
bool isFile(const std::string& dir, const std::string& fname);

//------------------------------------------------------------------
/** Remove a file from a directory
 *
 * @param[in] dir           String containing relative or absolute directory path
 * @param[in] fname         String containing file name
 *
 * @returns true if file exists in the directory
 */
bool rmFile(const std::string& dir, const std::string& fname);

//------------------------------------------------------------------
/** Remove file(s) from a directory using file prefix
*
* @param[in] dir           String containing relative or absolute directory path
* @param[in] prefix        String containing file name prefix
*
* @returns true if file exists in the directory
*/
bool rmFiles(const std::string& dir, const std::string& prefix);

//------------------------------------------------------------------
/** Touch a file in a directory
 *
 * @param[in] dir           String containing relative or absolute directory path
 * @param[in] fname         String containing file name
 *
 * @returns true if successful
 */
bool touchFile(const std::string& dir, const std::string& fname);

//------------------------------------------------------------------
/** Create or validate a directory
 *
 * @param[in] path           String containing relative or absolute directory path
 *
 * @retval ST_ERR_OK        Directory exists or was created
 * @retval ST_ERR_NULL_PTR Path is empty or pointer is null
 * @retval ST_ERR_LENGTH   Path exceeds MAX_PATH_LENGTH
 * @retval ST_ERR_DIR      Could not create the directory
 * @retval ST_ERR_NOT_DIR  Path exists but is not a directory
 */
 int32_t makeDir(const char* path);
 
//------------------------------------------------------------------
/** Create or validate all directories in a path
*
* @param[in] path          String containing relative or absolute directory path
*
* @retval ST_ERR_OK        All directories in the path exist or were created
* @retval ST_ERR_NULL_PTR Path is empty or pointer is null
* @retval ST_ERR_LENGTH   Path exceeds MAX_PATH_LENGTH
* @retval ST_ERR_DIR      Could not create one of the directories in the path
* @retval ST_ERR_NOT_DIR  An entry in the path is not a directory
*/
int32_t makeDirs(const char* path);

//------------------------------------------------------------------
/// Create or validata one or more hierarchical directories
///
/// @param[in] dir1         String the first directory path
/// @param[in] dir2         The second directory name (default = none)
/// @param[in] dir3         The third directory name (default = none)
/// @param[in] dir4         The fourth directory name (default = none)
/// @param[in] dir5         The fifth directory name (default = none)
///
/// @retval ST_ERR_OK       All directories in the path exist or were created
/// @retval ST_ERR_NULL_PTR fully formed path is empty
/// @retval ST_ERR_LENGTH   Fully formed path exceeds MAX_PATH_LENGTH
/// @retval ST_ERR_DIR      Could not create one of the directories in the path
///
int32_t makeDirs(const std::string& dir1, 
                 const std::string& dir2 = "",
                 const std::string& dir3 = "",
                 const std::string& dir4 = "",
                 const std::string& dir5 = "");

//------------------------------------------------------------------
/// recursively delete a directory and it's content
/// @param[in] dirpath      directory to delete
void deleteDir(const std::string& dirpath);

//------------------------------------------------------------------
/// Get the creation date for a file or directory


//------------------------------------------------------------------
/// Get list of all files (and not directories) in a directory that start with prefix - nonrecursive
/// @param[in] dirpath      directory to search
/// @param[in] prefix       file prefix to search for
/// @returns vector of all filenames in the directory
std::vector<std::string> getFileList(const std::string& dirpath, const std::string& prefix);

//------------------------------------------------------------------
/// Find files (but not directories) whose names start with prefix
/// and return the count (and optional total size in bytes) - non-recursive
/// @param[in] dirpath      directory to search
/// @param[in] prefix       file prefix to search for
/// @param[out] pSize       optional destination to receive total size
/// @returns number of files found 
uint32_t getFileCount(const std::string& dirpath, const std::string& prefix, uint64_t* pSize = nullptr);

//------------------------------------------------------------------
/// Get list of all directories (and not files) in a directory that start with prefix - nonrecursive
/// @param[in] dirpath      directory to search
/// @param[in] prefix       file prefix to search for
/// @returns vector of all names of directories in the directory
std::vector<std::string> getDirList(const std::string& dirpath, const std::string& prefix);

//------------------------------------------------------------------
/// Get the number of directories (and not files) in a directory that start with prefix - nonrecursive
/// @param[in] dirpath      directory to search
/// @param[in] prefix       file prefix to search for
/// @returns number of files found 
uint32_t getDirCount(const std::string& dirpath, const std::string& prefix);

//------------------------------------------------------------------
/// Expand a file or directory path to an absolute path
/// @param path               Relative path to directory or file
///
/// @returns absolute path, or empty string if not found
std::string getFullPath(const std::string& path);

//------------------------------------------------------------------
/// Get File extension, if any from a file name or path string
/// @param path             File name or path
///
/// @returns the file extension (everything following last '.')
/// or an empty string if no extension
std::string getFileExtension(const std::string& path);

//------------------------------------------------------------------
// Get file name, if any, from a path string
/// @param path              File name or path
///
/// @returns the file name and extension (everything following the last "/")
/// or an empty string if path is empty or ends in '/'
std::string getFileName(const std::string& path);

//------------------------------------------------------------------
// Get file base name, if any, from a path string
/// @param path              File name or path
///
/// @returns the file name without extension 
/// (everything between the last '/' and the last '.')
/// or an empty string if no extension is found.
std::string getFileBaseName(const std::string& path);

//------------------------------------------------------------------
/// Get file directory from a path string
/// @param path              File name or path
///
/// @returns the file directory path 
/// (everything up to the last '/')
/// or returns "." if path has no directory component)
std::string getFileDir(const std::string& path);

//------------------------------------------------------------------
/// Get information about a file or directory
/// @param[in] path         File or directory path
///
/// @returns 64bit file creation date
STUTIL::FileInfo getFileInfo(const std::string& path);

//------------------------------------------------------------------
/// Search for file name in one or more directories.
/// @param dirs               vector of directory paths to search.
/// @param fname              File name to search for
/// @param ext                Optional extension to try if file has none.
///
/// @returns full file path, or empty string if not found
std::string findFile(const std::vector<std::string>& dirs,
                    const std::string& fname,
                    const std::string& ext = "");

//------------------------------------------------------------------
/// Get Current user's home directory
/// @param user             User name (defaults to current user)
/// @returns full path to user home directory or "" if not found
std::string getUserHomeDir(const std::string& user="");

//------------------------------------------------------------------
/// Get Current user's default data directory
///
/// @returns full path to the user's default data directory or "" if not found
std::string getUserDataDir(void);

//------------------------------------------------------------------
/// Check if file exists
///
/// @returns true if file exists
///
bool fileExists(const std::string fname);

//------------------------------------------------------------------
/// check if string is a valid windows or linux file/directory name
///
/// @param[in] fname    prospective file name
/// @param[in] maxlen   optional max name length
///
/// @returns 0 if valid, else negative error code
///
int32_t checkFileName(const std::string fname, uint32_t maxlen = 0);

//------------------------------------------------------------------
/// Get total disk size and available space
///
/// @param[in]  path             Any directory path on the desired file system
/// @param[out] pTotalBytes      Pointer to variable to receive total bytes
/// @param[out] pAvailableBytes  Pointer to variable to available bytes
///
/// @note either output pointer can be null
///
/// @return 0 if successful, else negative error code
///
int32_t getDiskSpace(const std::string& path,
                     uint64_t* pTotalBytes , uint64_t* pAvailableBytes );

//----------------------------------------------
/// Change the group ID of a file or directory
///
/// @param[in] dirPath      directory path
/// @param[in] groupName    group name
/// @param[in] rtnIn        if < 0, do not run, return this value
///
/// @return 0 if ok, else negative error code
///
int32_t setGroup(const std::string dirPath,
                 const std::string groupName,
                 int32_t rtnIn = 0);

//----------------------------------------------
/// Change the group ID of a file or directory
///
/// @param[in] dirPath      directory path
/// @param[in] name         directory or file name
/// @param[in] groupName    group name
/// @param[in] rtnIn        if < 0, do not run, return this value
///
/// @return 0 if ok, else negative error code
///
int32_t setGroup(const std::string dirPath,
    const std::string name,
    const std::string groupName,
    int32_t rtnIn = 0);

//----------------------------------------------
/// Change the group ID of a file or directory
///
/// @param[in] dirPath      directory path
/// @param[in] name1        directory name
/// @param[in] name2        directory or file name
/// @param[in] groupName    group name
/// @param[in] rtnIn        if < 0, do not run, return this value
///
/// @return 0 if ok, else negative error code
///
int32_t setGroup(const std::string dirPath,
    const std::string name1,
    const std::string name2,
    const std::string groupName,
    int32_t rtnIn = 0);

//----------------------------------------------
/// Create a symbolic link to a file or directory
///
/// @param[in] targetPath   path to target file or directory
/// @param[in] symPath      path to new symbolic link
/// @param[in] isDirectory  true if target is a directory (default = false)
/// 
/// @note Requires admin privilege on Windows
/// 
/// @return 0 if ok, else negative error code
/// 
int32_t createSymLink(const std::string targetPath,
                      const std::string symPath,
                      bool isDirectory = false);


}  // STSTUTIL
//******************************************************************
// End of file
//******************************************************************
#endif //STUTIL_FILE_H

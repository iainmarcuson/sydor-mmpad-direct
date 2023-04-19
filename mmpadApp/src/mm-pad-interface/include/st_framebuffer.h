//*******************************************************************
/// @file st_framebuffer.h
/// Sydor MM/MegaPAD Generic Frame buffer wrapper class 
///
/// @author      W.R. Lear
/// @date        10/09/2020
/// @copyright   Copyright (c) 2020 Sydor Technologies, all rights reserved.
///
/// This C++ class holds a single X-PAD frame buffer along with
/// associated metadata, parameters, and accessor methods
///
//*******************************************************************
#ifndef ST_FRAME_BUFFER_H
#define ST_FRAME_BUFFER_H

#include <string>
#include <cstring>
#include <vector>
#include <atomic>
#include "st_errors.h"
#include "stutil_queue.hpp"
#include "stutil_logger.h"
#include "stutil_misc.h"
#include "st_if_defs.h"

namespace ST_INTERFACE
{

//******************************************************************
// Definitions and Constants
//******************************************************************

/// There needs to be SOME valid default - it should rarely if ever be needed
#define ST_FRAME_DEFAULT_FRAME_TYPE ST_SYS_MMPAD

/// Overlay (sFrameOverlay) bitmask values
#define ST_FRAME_OVERLAY_NULL            0   ///< No overlays
#define ST_FRAME_OVERLAY_GRID            1   ///< Gridlines
#define ST_FRAME_OVERLAY_BOUNCING_BALL   2   ///< Bouncing ball

//******************************************************************
// Data structures, enumerations and type definitions
//******************************************************************
class StFrameBuffer;
/// Queue of frame buffer pointers
typedef STUTIL::SafeQueue<StFrameBuffer*> StFrameBufferQueue;

//------------------------------------------------------------------
/// X-PAD StFrameBuffer header
/// 
#pragma pack(push, 1)
struct StFrameHeader
{
    // Frame parameters (16 bytes)
    uint16_t headerBytes;       ///< Total header length in bytes
    uint16_t id;                ///< StFrameBuffer id magic number: 'ST' (0x5354)
    uint16_t version;           ///< Header version: Major.minor: 0xMMmm)
    uint16_t frameType;         ///< Frame type (STSystemType enum)
    uint32_t frameBytes;        ///< Total frame length in bytes
    uint32_t frameStatus;       ///< Frame status flags (ST_FRAME_STAT_XXXX)

    // Image parameters (16 bytes)
    uint32_t imageBytes;        ///< Total image buffer length in bytes
    uint16_t imageWidth;        ///< Image width in pixels
    uint16_t imageHeight;       ///< Image height in lines
    uint8_t  pixelBytes;        ///< Actual pixel size in bytes
    uint8_t  pixelType;         ///< Pixel data type (STDataType)
    uint8_t  reserved1[6];      ///< Reserved for 16-byte alignment

    // Data section lengths (16 bytes)
    uint32_t telemetryBytes;    ///< Telemetry section length in bytes
    uint32_t data1Bytes;        ///< Optional data section 1 length in bytes
    uint32_t data2Bytes;        ///< Optional data section 2 length in bytes
    uint32_t data3Bytes;        ///< Optional data section 3 length in bytes

    // Frame metadata (40 bytes)
    STFrameMetadata metadata;   ///< Frame metadata

    uint8_t capCount;           ///< Frame Capacitor count

    static const uint32_t rsvd3Bytes = ST_FRAME_HEADER_BYTES - 
                                       (3 * 16) -           // params and data sections
                                       sizeof(metadata) -   // metadata
                                       sizeof(capCount) -   // capacitor count
                                       sizeof(uint64_t);    // frameTimestamp
    uint8_t  reserved3[rsvd3Bytes];

    uint64_t frameTimestamp;    ///< timestamp for internal server use

    /// Constructor
    StFrameHeader(STSystemType type = ST_FRAME_DEFAULT_FRAME_TYPE)
    {
        memset(this, 0, sizeof(StFrameHeader));
        headerBytes = ST_FRAME_HEADER_BYTES;
        id = ST_FRAME_ID;
        version = ST_FRAME_VERSION;
        frameType = static_cast<uint16_t>(type);
        frameBytes = ST_FRAME_HEADER_BYTES + ST_FRAME_FOOTER_BYTES;
        frameStatus = ST_FRAME_STAT_DEFAULT;
    }

    //----------------------------------------------
    /// Compute rounded up sizes
    ///
    /// Potentially this function modifies the following to ensure
    /// alignment rules are followed.
    ///
    /// imageWidth
    /// imageHeight
    /// imageBytes
    /// telemetryBytes
    /// data1Bytes
    /// data2Bytes
    /// data3Bytes
    ///
    /// frameBytes is recomputed.
    ///
    void computeRoundUp(void)
    {
        imageWidth  = STUTIL::roundUp(imageWidth, 2);
        imageHeight = STUTIL::roundUp(imageHeight, 2);
        imageBytes  = STUTIL::roundUp(imageWidth * imageHeight * pixelBytes, 4);
        telemetryBytes = STUTIL::roundUp(telemetryBytes, 4);
        data1Bytes  = STUTIL::roundUp(data1Bytes, 4);
        data2Bytes  = STUTIL::roundUp(data2Bytes, 4);
        data3Bytes  = STUTIL::roundUp(data3Bytes, 4);
        uint32_t totalBytes = ST_FRAME_HEADER_BYTES +
                        imageBytes +
                        telemetryBytes +
                        data1Bytes +
                        data2Bytes +
                        data3Bytes +
                        ST_FRAME_FOOTER_BYTES ;
        // Total size must be a multiple of the image line size
        frameBytes = STUTIL::roundUp(totalBytes, imageWidth * pixelBytes);
    }

};
#pragma pack(pop)


//******************************************************************
// MegaPAD Framebuffer class
//******************************************************************
class StFrameBuffer
{
private:
    static STSystemType sDefFrameType;  ///< Default frame type if not specified in constructor
    static uint32_t sOverlay;           ///< Bitmask of ST_FRAME_OVERLAY_XXXX (default = 0, None)
    // Overlay dot variables for the bouncing ball
    static uint16_t sOverlayDotX;       ///< dot current X position
    static uint16_t sOverlayDotY;       ///< dot current Y position
    static uint8_t sOverlayDotW;        ///< dot width
    static uint8_t sOverlayDotH;        ///< dot height
    static uint8_t sOverlayDotDX;       ///< dot delta X per move
    static uint8_t sOverlayDotDY;       ///< dot delta Y per move
    static bool sOverlayDotRight;       ///< dot moving to the left
    static bool sOverlayDotDown;        ///< dot moving downward

    STUTIL::Logger *pLog;               ///< standard stutil logging
    StFrameHeader   mHeader;            ///< frame header
    uint8_t        *mBufferPtr;         ///< pointer to allocated buffer
    uint8_t        *mImagePtr;          ///< pointer to image section
    bool            mComplete[ST_MAX_SUBFRAME_COUNT];  ///< Individual subframe complete flags
    bool            mAllComplete;       ///< True if all subframes are complete

    //************************************************************
    // Static methods 
    //************************************************************
public:

    //----------------------------------------------
    /// Get the default frame type used when the default constructor is called
    static STSystemType getDefaultFrameType(void) { return sDefFrameType; }

    //----------------------------------------------
    /// Set the default frame type used when the default constructor is called
    static void setDefaultFrameType(STSystemType frameType = ST_FRAME_DEFAULT_FRAME_TYPE)
    { sDefFrameType = (ST_SYS_NONE == frameType)? ST_FRAME_DEFAULT_FRAME_TYPE: frameType; }

    //----------------------------------------------
    // Get pixel size in bytes from pixel type
    static uint8_t getPixelBytesFromType(STDataType pixelType);

    //----------------------------------------------
    // Get number of subFrames from frame type
    static uint8_t getSubframeCount(STSystemType frameType);

    //----------------------------------------------
    /// Set the frame overlay bitmask
    static void setFrameOverlay(uint32_t frameOverlay) {sOverlay = frameOverlay;}

    //----------------------------------------------
    /// Get the frame overlay bitmask
    static uint32_t getFrameOverlay(void) {return sOverlay;}

    //************************************************************
    // Public methods 
    //************************************************************
public:

    //----------------------------------------------
    /// Constructor - short form (also default)
    ///
    /// @param[in] frameType        type of system generating the frame
    /// @param[in] noTelemetry      No telemetry section if true
    /// @param[in] imageBytes       total image buffer size in bytes
    /// @param[in] data1Bytes       additional data section size in bytes
    /// @param[in] data2Bytes       additional data section size in bytes
    /// @param[in] data3Bytes       additional data section size in bytes
    /// 
    /// The default constructor (all params at default values)
    /// creates a minimally sized instance containing only the
    /// frame header and footer.
    /// 
    /// Specifying only frameType creates an instance
    /// large enough to hold a raw Frame and telemetry for that type
    /// 
    /// Specifying frameType and noTelemetry create an instance
    /// large enough to hold only a raw image of that type.
    /// 
    /// If imageBytes is greater than the minimum size required for the
    /// specified frame type, the image buffer will increased to 
    /// that size. Otherwise, this value is ignored.
    /// 
    /// Specifying one or more dataBytes values appends the specified
    /// amount of memory to the framebuffer, and sets the appropriate
    /// data segment size values in the header.
    /// 
    /// @note: imageBytes and dataBytes will be rounded up to a multiple
    /// of 4 bytes. 
    /// 
    StFrameBuffer(STSystemType frameType = ST_SYS_NONE,
                  bool noTelemetry = false,
                  uint32_t imageBytes = 0,
                  uint32_t data1Bytes = 0,
                  uint32_t data2Bytes = 0,
                  uint32_t data3Bytes = 0);

    //----------------------------------------------
    /// Constructor
    ///
    /// @param[in] frameType        type of system generating the frame
    /// @param[in] pixelType        image pixelType
    /// @param[in] imageWidth       image width in pixels
    /// @param[in] imageHeight      image height in lines
    /// @param[in] noTelemetry      No telemetry section if true
    /// @param[in] imageBytes       total image size in bytes
    /// @param[in] data1Bytes       additional data section size in bytes
    /// @param[in] data2Bytes       additional data section size in bytes
    /// @param[in] data3Bytes       additional data section size in bytes
    /// 
    /// frameType, pixelType, imageWidth, and imageHeight must be set to valid
    /// values. 
    /// 
    /// Specifying noTelemetry creates an instance with no telemetry section.
    ///
    /// When imageBytes is greater than the raw image length for the
    /// specified frameType, the image buffer length will be set to 
    /// that size. Otherwise, the image buffer will be set to the raw image length.
    ///
    /// Specifying one or more dataBytes values appends the specified
    /// amount of memory to the framebuffer, and sets the appropriate
    /// data segment size values in the header.
    /// 
    /// @note: imageBytes and dataBytes will be rounded up to a multiple
    /// of 4 bytes. 
    /// 
    StFrameBuffer(STSystemType frameType,
                  STDataType pixelType,
                  uint16_t imageWidth,
                  uint16_t imageHeight,
                  bool noTelemetry = false,
                  uint32_t imageBytes = 0,
                  uint32_t data1Bytes = 0,
                  uint32_t data2Bytes = 0,
                  uint32_t data3Bytes = 0);

    //----------------------------------------------
    /// Destructor
    ~StFrameBuffer();  

    //----------------------------------------------
    /// Overloaded '=' operator for deep copy
    StFrameBuffer& operator=(const StFrameBuffer &f1);

    //-----------------------------------------------
    /// Resize frame buffer, attempting to preserve data if requested
    ///
    /// @param[in] preserve         preserve existing data if true
    /// @param[in] frameType        type of system generating the frame
    /// @param[in] noTelemetry      No telemetry if true
    /// @param[in] imageBytes       image buffer size in bytes
    /// @param[in] data1Bytes       additional data section size in bytes
    /// @param[in] data2Bytes       additional data section size in bytes
    /// @param[in] data3Bytes       additional data section size in bytes
    ///
    /// When preserve is true, the current content of each data section will be preserved
    /// to the extent allowed by the other section lengths. This parameter is ignored
    /// when frameType is different from the current frameType.
    /// 
    /// (Note: preserve is currently minimally implemented. It does not correctly resize
    /// images.)
    /// 
    /// When frameType == ST_SYS_NONE, the current frame type will be unchanged.
    /// 
    /// Setting noTelemetry true will delete existing telemetry data, if any.
    /// 
    /// When imageBytes is <= 1, the current image buffer length will be unchanged,
    /// unless it is not large enough to hold a raw image for the new frameType.
    ///
    /// When 1 < imageBytes < raw image length for the specified
    /// frame type, the image buffer will be set to the minimum of the existing length
    /// and the raw image length.
    /// 
    /// When imageBytes >= the raw image length for the
    /// specified frameType, the image buffer will be set to that length.
    /// 
    /// When a new data section length is greater than the current length, the balance
    /// will be padded with 0. When a new section length is non-zero but 
    /// less than the current length, the content will be truncated.
    /// 
    /// When a dataByte value is 0, that data section will be deleted
    /// When a dataByte value is 1, the length of the data section will be unchanged
    /// When a dataByte value is > 1, the data section length will be adjusted
    /// to the new value.
    /// 
    /// imageBytes and dataBytes values will be rounded up to a multiple of 4 bytes.
    /// 
    /// @return 0 if ok, else negative error code
    ///
    int32_t resize(bool preserve,
                   STSystemType frameType = ST_SYS_NONE,
                   bool noTelemetry = false,
                   uint32_t imageBytes = 1,
                   uint32_t data1Bytes = 1,
                   uint32_t data2Bytes = 1,
                   uint32_t data3Bytes = 1 );

    //-----------------------------------------------
    /// Resize frame buffer, attempting to preserve data if requested
    ///
    /// @param[in] preserve         preserve existing data if true
    /// @param[in] frameType        new frame type (ST_SYS_NONE = no change)
    /// @param[in] pixelType        image pixelType
    /// @param[in] imageWidth       image width in pixels
    /// @param[in] imageHeight      image height in lines
    /// @param[in] noTelemetry      No telemetry section if true
    /// @param[in] imageBytes       total image size in bytes
    /// @param[in] data1Bytes       additional data section size in bytes
    /// @param[in] data2Bytes       additional data section size in bytes
    /// @param[in] data3Bytes       additional data section size in bytes
    ///
    /// When preserve is true, the current content of each data section will be preserved
    /// to the extent allowed by the other section lengths. This parameter is ignored
    /// when frameType is different from the current frameType.
    /// 
    /// (Note: preserve is currently minimally implemented. It does not correctly resize
    /// images.)
    /// 
    /// When frameType == ST_SYS_NONE, the current frame type will be unchanged.
    /// 
    /// When imageWidth and imageHeight are <= 1, the current values remain
    /// unchanged. When >= raw image sizes the image buffer will be resized
    /// as needed to hold the raw image 
    /// 
    /// When pixelType is ST_ANY, the current value remains unchanged.
    /// When pixelType is different from the current value, the image buffer size 
    /// will be adjusted as needed. Pixel data will be converted if preserve is true.
    /// 
    /// Setting noTelemetry true will delete any existing telemetry data. Otherwise,
    /// the telemetry data section will be adjusted as needed for the specified frame type.
    /// 
    /// When imageBytes is <= 1, the current image buffer length will be unchanged,
    /// unless it is not large enough to hold a raw image for the new frameType.
    ///
    /// When 1 < imageBytes < raw image length for the specified
    /// frame type, the image buffer will be set to the minimum of the existing length
    /// and the raw image length.
    /// 
    /// When imageBytes >= the raw image length for the
    /// specified frameType, the image buffer will be resized to that length.
    /// 
    /// When a new data section length is greater than the current length, the balance
    /// will be padded with 0. When a new section length is non-zero but 
    /// less than the current length, the content will be truncated.
    /// 
    /// When a dataByte value is 0, that data section will be deleted
    /// When a dataByte value is 1, the length of the data section will be unchanged
    /// When a dataByte value is > 1, the data section length will be adjusted
    /// to the new value.
    /// 
    /// imageBytes and dataBytes values will be rounded up to a multiple of 4 bytes.
    /// 
    /// imageBytes and dataBytes will be rounded up to a multiple of 4 bytes.
    /// 
    /// @return 0 if ok, else negative error code
    ///
    int32_t resize(bool preserve,
                   STSystemType frameType,
                   STDataType pixelType,
                   uint16_t imageWidth,
                   uint16_t imageHeight,
                   bool noTelemetry = false,
                   uint32_t imageBytes = 1,
                   uint32_t data1Bytes = 1,
                   uint32_t data2Bytes = 1,
                   uint32_t data3Bytes = 1 );

    //-----------------------------------------------
    /// Pack the frame buffer image and telemetry
    ///
    /// Ensure the image buffer and telemetry sections are no larger than
    /// necessary to contain the current frame definition. 
    /// 
    /// The content of other data sections, if any, remain intact.
    /// 
    /// This is especially useful after applying a correction operation
    /// which changes the size of the image
    ///
    void pack(void);

    //----------------------------------------------
    /// Return frame Type
    STSystemType getFrameType() { return static_cast<STSystemType>(mHeader.frameType); }

    //----------------------------------------------
    /// Return true if this is a Mega-Pad frame buffer
    bool isMegaPad(void)
    {
        return (ST_SYS_MEGAPAD == getFrameType());
    }

    //----------------------------------------------
    /// Return true if this is aan MM-Pad frame buffer
    bool isMMPad(void)
    {
        return (ST_SYS_MMPAD == getFrameType());
    }

    //----------------------------------------------
    /// Return true if this is a Keck-Pad frame buffer
    bool isKeckPad(void)
    {
        return (ST_SYS_KECKPAD == getFrameType());
    }

    //----------------------------------------------
    /// Get the total raw frame buffer length in bytes
    uint32_t getFrameBytes(void) { return mHeader.frameBytes; }

    //----------------------------------------------
    /// Get the frame number (1-based)
    uint32_t getFrameNumber(void) { return mHeader.metadata.runFrameNumber; }

    //----------------------------------------------
    /// Get the frame header 
    StFrameHeader *getFrameHeader(){return &mHeader;};
    
    //----------------------------------------------
    /// Set the frame number (1-based)
    void setFrameNumber(uint32_t number) {mHeader.metadata.runFrameNumber = number;}

    //----------------------------------------------
    /// Get the image width in pixels
    uint16_t getImageWidth(void) { return mHeader.imageWidth; }

    //----------------------------------------------
    /// Get the image height in lines
    uint16_t getImageHeight(void) { return mHeader.imageHeight; }

    //----------------------------------------------
    /// Get the total number of pixels in the image
    uint32_t getImagePixelCount(void) { return mHeader.imageWidth * mHeader.imageHeight; }

    //----------------------------------------------
    /// Get the pixel type
    STDataType getPixelType(void) { return static_cast<STDataType>(mHeader.pixelType); }

    //----------------------------------------------
    /// Get a pointer to the specified pixel
    void* getPixelPtr(uint16_t x, uint16_t y);

    //----------------------------------------------
    /// Get number of bytes per pixel
    uint8_t getPixelBytes(void) { return mHeader.pixelBytes; }

    //----------------------------------------------
    /// Get the image buffer size in bytes
    uint32_t getImageBytes(void) { return mHeader.imageBytes; }

    //----------------------------------------------
    /// Get telemetry section size in bytes
    uint32_t getTelemetryBytes(void) { return mHeader.telemetryBytes; }

    //----------------------------------------------
    /// Get data section 1 size in bytes
    uint32_t getData1Bytes(void) { return mHeader.data1Bytes; }

    //----------------------------------------------
    /// Get data section 2 size in bytes
    uint32_t getData2Bytes(void) { return mHeader.data2Bytes; }

    //----------------------------------------------
    /// Get data section 3 size in bytes
    uint32_t getData3Bytes(void) { return mHeader.data3Bytes; }

    //----------------------------------------------
    /// Get pointer to the raw frame buffer
    uint8_t* getBufferPtr(void) { return mBufferPtr; }

    //-----------------------------------------------
    /// Get image buffer pointer
    uint8_t* getImagePtr(void);

    //-----------------------------------------------
    /// Get telemetry buffer pointer
    ///
    /// @return nullptr if frame buffer contains no telemetry
    uint16_t* getTelemetryPtr(void);

    //-----------------------------------------------
    /// Get data buffer 1 pointer
    uint8_t* getData1Ptr(void);

    //-----------------------------------------------
    /// Get data buffer 2 pointer
    uint8_t* getData2Ptr(void);

    //-----------------------------------------------
    /// Get data buffer 3 pointer
    uint8_t* getData3Ptr(void);

    //-----------------------------------------------
    /// Get footer pointer
    uint64_t* getFooterPtr(void);

    //-----------------------------------------------
    /// Get subframe count
    uint8_t getSubFrameCount(void);

    //-----------------------------------------------
    /// Get capacitor count
    uint8_t getCapCount(void);

    //-----------------------------------------------
    /// Clear the complete flags
    void clearComplete(void);

    //----------------------------------------------
    /// Return true if all subframes have been written to this frame
    bool isComplete(void);

    //----------------------------------------------
    /// Return true if the specified subframe has been written to this frame
    bool isComplete(uint32_t subFrame);

    //----------------------------------------------
    /// Load a raw subframe frame (from hardware) into the proper quadrant of the framebuffer
    ///
    /// @param subFrame     pointer to the source subframe
    /// @param frameNumber  frame number
    /// @param index        subframe index (0=upper left)
    ///
    /// @return 0 on success, else negative error code
    int32_t loadSubFrame(const void* pSubFrame, uint32_t frameNumber, uint32_t index);

    //-----------------------------------------------
    /// Deserialize the frame buffer object instance from the specified source buffer
    ///
    /// @param pSrc         pointer to source data
    /// @param srcBytes     number of bytes to copy
    /// 
    /// @return 0 if ok, else negative error code
    /// 
    int32_t deserializeFrom(const void* pSrc, size_t srcBytes);

    //-----------------------------------------------
    /// Serialize the frame buffer object instance to an external buffer
    ///
    /// @param pDest        pointer to destination buffer
    /// @param maxBytes     max number of bytes to copy
    /// 
    /// @return 0 if ok, else negative error code
    /// 
    int32_t serializeTo(void* pDest, size_t maxBytes);

    //--------------------------------
    /// Copy XPAD Metadata to the framebuffer Metadata
    ///
    /// @param pSrc         pointer to source metadata
    ///
    /// @return 0 if ok, else negative error code
    ///
    int32_t setMetadata(const STFrameMetadata* pSrc);

    //-----------------------------------------------
    /// Copy the frame Metadata to XPAD Metadata
    ///
    /// @param pDest         pointer to destination telemetry
    ///
    /// @return 0 if ok, else negative error code
    ///
    int32_t getMetadata(STFrameMetadata* pDest);

    //-----------------------------------------------
    /// convert the telemetry data from raw values to uint16_t
    ///
    /// @param pTelem       pointer to raw telemetry for one subFrame
    ///
    /// @return vector of converted telemetry data
    ///
    static std::vector<uint16_t> convertTelemetry(const STRawTelemetry* pTelem);

    //-----------------------------------------------
    /// set the telemetry data from raw values
    ///
    /// @param pRawTelem        pointer to raw telemetry for one subframe
    /// @param index            subframe index (0=upper left)
    /// 
    /// @return 0 if ok, else negative error code
    /// 
    int32_t setTelemetry(const STRawTelemetry* pRawTelem, uint32_t index = 0);

    //-----------------------------------------------
    /// Update the object frame header from the raw framebuffer content
    ///
    /// @return 0 if ok, else negative error code
    ///
    int32_t updateFrameHeader(void);

    //-----------------------------------------------
    /// Clear all pixels in an image
    /// 
    /// @return 0 if ok, else negative error code
    /// 
    int32_t clearImage();

    //----------------------------------------------
    /// Apply overlays
    ///
    /// Depending on the sFrameOverlay bitmask, apply the appropriate
    /// (simulated) overlays to the frame.
    void applyOverlays(void);

    //----------------------------------------------
    /// Get the frame timestamp value
    uint64_t getFrameTimestamp(void) { return mHeader.frameTimestamp; }

    //----------------------------------------------
    /// Set the frame timestamp value
    void setFrameTimestamp(uint64_t timestamp) { mHeader.frameTimestamp = timestamp; }

    //************************************************************
    // Private methods
    //************************************************************
private:

    //-----------------------------------------------
    /// Copy a frame into the frame buffer
    ///
    /// @param pSrc         pointer to source data
    /// @param srcBytes     number of bytes to copy
    ///
    /// @return 0 if ok, else negative error code
    ///
    int32_t copyFrameFrom(const void* pSrc, uint32_t srcBytes);

    //-----------------------------------------------
    /// Copy the frame to an external destination
    ///
    /// @param pDest        pointer to destination buffer
    /// @param maxBytes     max number of bytes to copy
    ///
    /// @return 0 if ok, else negative error code
    ///
    int32_t copyFrameTo(void* pDest, uint32_t maxBytes);

    //-----------------------------------------------
    /// Copy an image into to the frame buffer
    ///
    /// @param pSrc         pointer to source data
    /// @param srcBytes     number of bytes to copy
    ///
    /// @return 0 if ok, else negative error code
    ///
    int32_t copyImageFrom(const void* pSrc, uint32_t srcBytes);

    //-----------------------------------------------
    /// Copy the image to an external destination
    ///
    /// @param pDest        pointer to destination buffer
    /// @param pixelType    pixel data type
    /// @param width        image width
    /// @param height       image height
    ///
    /// @return 0 if ok, else negative error code
    ///
    int32_t copyImageTo(void* pDest, STDataType pixelType, uint16_t width, uint16_t height);

    //----------------------------------------------
    /// Load a raw MM-PAD frame into the framebuffer
    ///
    /// @param pRawFrame     pointer to the source raw frame
    /// @param frameNumber   expected frame number
    /// 
    /// @note will fail if framebuffer is not large enough to
    /// hold an MM-PAD raw frame
    ///
    /// @return 0 if ok, else negative error code
    /// 
    int32_t loadMMRawFrame(const MXRawFrame* pRawFrame, uint32_t frameNumber);

    //----------------------------------------------
    /// Load a raw MEGA-PAD frame into one quadrant of the framebuffer
    ///
    /// @param pRawFrame    pointer to the source raw frame
    /// @param frameNumber  expected frame number
    /// @param index        subframe index (0=upper left)
    ///
    /// @note will fail if framebuffer is not large enough to
    /// hold an MEGA-PAD raw frame
    ///
    /// @return 0 if ok, else negative error code
    /// 
    int32_t loadMGRawFrame(const MXRawFrame* pRawFrame, uint32_t frameNumber, uint32_t index);

    //----------------------------------------------
    /// Load a raw KECK-PAD frame into the framebuffer
    ///
    /// @param pRawFrame     pointer to the source raw frame
    /// @param frameNumber   frame number
    ///
    /// @note will fail if framebuffer is not large enough to
    /// hold a Keck-PAD raw frame
    ///
    /// @return 0 if ok, else negative error code
    /// 
    int32_t loadKKRawFrame(const KKRawFrame* pRawFrame, uint32_t frameNumber);

    //----------------------------------------------
    /// Apply a grid overlay to the image
    ///
    void applyGridOverlay(void);

    //----------------------------------------------
    /// Apply bouncing dot overlay to the image
    ///
    void applyDotOverlay(void);

    //-----------------------------------------------
    /// Get grid overlay columns
    uint16_t getGridOverlayCols(void);

    //-----------------------------------------------
    /// Get grid overlay rows
    uint16_t getGridOverlayRows(void);

    //----------------------------------------------
    /// Compute capacitor count from capacitor select flags
    uint8_t computeCapCount(uint32_t capSelects);

}; // class StFrameBuffer

}  // RTSUP
//******************************************************************
// End of file
//******************************************************************
#endif //ST_FRAME_BUFFER_H

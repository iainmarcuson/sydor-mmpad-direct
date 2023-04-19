//*******************************************************************
/// @file st_sim_data.h
/// Sydor Server simulated data generation class
///
/// @author      W.R. Lear
/// @date        10/03/2019
/// @copyright   Copyright (c) 2019 Sydor Technologies, all rights reserved.
///
/// This header file defines a class for generating simulated
/// frame and telemetry data for the Sydor Technologies MM-PAD System
///
//*******************************************************************
#ifndef ST_SIM_DATA_H
#define ST_SIM_DATA_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string>
#include "st_if_defs.h"
#include "st_errors.h"
#include "stutil_logger.h"

namespace STUTIL
{
    class Logger;
}

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
    
typedef enum
{
    SIM_IMAGE_CLEAR,
    SIM_IMAGE_SWEEP_HORIZ,
    SIM_IMAGE_SWEEP_VERT
} SIM_IMAGE;

typedef enum
{
    SIM_META_CLEAR,     ///< all zeros
    SIM_META_BASIC,     ///< timestamp and frame numbers
    SIM_META_SAVED      ///< restore saved metadata
} SIM_META;

typedef enum
{
    SIM_TELEM_CLEAR,
    SIM_TELEM_CONST,
    SIM_TELEM_SAVED
} SIM_TELEM;

typedef struct
{
    // Sweep
    double   mSwGamma;              ///< Sweep gamma exponent
    uint32_t mSwMin;                ///< Minimum pixel value for sweep
    uint32_t mSwMax;                ///< Maximum pixel value for sweep

    // Grid
    bool mGridEnable;               ///< grid enabled if true

    // Moving Dot
    bool     mDotEnable;            ///< dot enabled if true

} SimImageDef;

typedef struct
{
    SIM_IMAGE curImage;
    SIM_META curMeta;
    SIM_TELEM curTelem;

    STFrameMetadata meta;           ///< template for generating simulated metadata
    STRawTelemetry telem;           ///< template for generating telemetry data
    SimImageDef image;              ///< Image definitions

} SimFrameDef;

///@} end of typedefs

//******************************************************************
/// @name StSimData Class definition
///@{
    
class StSimData
{
protected:
    // Common
    STUTIL::Logger* pLog;           ///< Debug Logger instance
    SimFrameDef mFrame;
    StFrameBuffer *mpRefFrameBuffer;

    static const uint32_t SW_N_PIXELS = MX_RAW_IMAGE_WIDTH * MX_RAW_IMAGE_HEIGHT;
    double   mSwCurve[SW_N_PIXELS]; ///< Base power curve for sweep

protected:
    //----------------------------------------------
    // Draw a Horizontal sweep
    void drawSweep(StFrameBuffer& FrameBuffer, bool horiz);

    //----------------------------------------------
    // Generate the base gamma curve
    void genGammaCurve(void);

public:
    //----------------------------------------------
    /// Constructor
    StSimData();

    //----------------------------------------------
    /// Destructor
    ~StSimData();

    //----------------------------------------------
    // Get Sim Image Definition
    SimImageDef getSimImageDef(void) { return mFrame.image; }

    //----------------------------------------------
    // Set Sim Image Definition
    void setSimImageDef(const SimImageDef& def);

    //----------------------------------------------
    // Get Sim Metadata Definition
    STFrameMetadata getSimMetadataDef(void) { return mFrame.meta; }

    //----------------------------------------------
    // Get Sim Frame Definition
    SimFrameDef getSimFrameDef(void) { return mFrame; }

    //----------------------------------------------
    // Set Sim Frame Definition
    void setSimFrameDef(const SimFrameDef& def);

    //----------------------------------------------
    // Set Sim Metadata Definition
    void setSimMetaDef(const STFrameMetadata& def);

    //----------------------------------------------
    void setSimMetaIntegrationTime(const uint32_t time)
    {
        mFrame.meta.integrationTime = time;
    }

    //----------------------------------------------
    void setSimMetaInterFrameTime(const uint32_t time)
    {
        mFrame.meta.interFrameTime = time;
    }

    //----------------------------------------------
    void setSimMetaExposureMode(const uint32_t mode)
    {
        mFrame.meta.exposureMode = mode;
    }

    //----------------------------------------------
    void setRefFrameBuffer(StFrameBuffer *pFrameBuffer)
    {
        mpRefFrameBuffer = pFrameBuffer;
    }

    //----------------------------------------------
    // Get Sim Telemetry Definition
    STRawTelemetry getTelemetryDef(void) { return mFrame.telem; }

    //----------------------------------------------
    // Set Sim Telemetry Definition
    void setTelemetryDef(const STRawTelemetry& def);

    //----------------------------------------------
    // Sweep parameters
    uint32_t getSwMin(void) { return mFrame.image.mSwMin; }
    uint32_t getSwMax(void) { return mFrame.image.mSwMax; }
    double   getSwGamma(void) { return mFrame.image.mSwGamma; }
    void     setSwMin(uint32_t min) { mFrame.image.mSwMin = min; }
    void     setSwMax(uint32_t max) { mFrame.image.mSwMax = max; }
    void     setSwGamma(double gamma);
    void     resetSweep(void);

    //----------------------------------------------
    // "Bouncing Dot" parameters
    bool     isDotEnabled(void) { return mFrame.image.mDotEnable; }
    void     resetDot(void);

    //----------------------------------------------
    // Grid parameters
    bool     isGridEnabled(void) { return mFrame.image.mGridEnable; }
    void     enableGrid(bool en) { mFrame.image.mGridEnable = en; }
    void     resetGrid(void);

    //----------------------------------------------
    /// Start Capture set
    void startCaptureSet(void);

    //----------------------------------------------
    /// Start Capture run
    void startCaptureRun(void);

    //----------------------------------------------
    /// Generate simulated image
    void genSimImage(StFrameBuffer& frameBuffer);

    //----------------------------------------------
    // Generate simulated frame meta data
    void genSimMetadata(STFrameMetadata* pMetadata);
    
    //----------------------------------------------
    /// Generate simulated telemetry sensor data
    void genSimTelemetry(STRawTelemetry* pTelemetry);

    //----------------------------------------------
    /// Generate simulated frame
    void genSimFrame(StFrameBuffer& frameBuffer);

};
///@} end of StSimData class

} // namespace ST_INTERFACE

//******************************************************************
// End of file
//******************************************************************

#endif //ST_SIM_DATA_H

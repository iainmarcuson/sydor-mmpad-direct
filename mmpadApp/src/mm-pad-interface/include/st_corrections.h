//**************************************************************
/// @file st_corrections.h
/// Sydor MMPAD Corrections Class
///
/// @author      Iain Marcuson
/// @date        03/10/2022
/// @copyright   Copyright (c) 2022 Sydor Technologies, all rights reserved.
///
/// This C++ class will perform corrections on an X-PAD frame buffer
///
//**************************************************************

#ifndef ST_CORRECTIONS_H
#define ST_CORRECTIONS_H

#include "st_errors.h"
#include "st_framebuffer.h"
#include "st_if_defs.h"
#include "mmpad_types.h"

#define KK_MAX_CAPS 8 // Maximum number of caps for Keck

namespace ST_INTERFACE
{
    
    class StCorrections
    {
    public:
        static StCorrections &GetInstance()
            {
                static StCorrections* singleton = new StCorrections();
                return *singleton;
            }
        void applyGradient(StFrameBuffer &f1);
        void scaleImage(StFrameBuffer &f1, double scaleValue);
        int32_t accumulateImage(StFrameBuffer &fSrc, StFrameBuffer &fDest);
        // Subtracts Computes fDest = fFg - fBg.  fBg is required to be double.  fDest needs the same type as fFg.
        int32_t subtractImage(StFrameBuffer &fFg, StFrameBuffer &fBg, StFrameBuffer &fDest);
        bool isGeocorr() { return b_do_geocorr;};
        bool isDebounce() { return b_do_debounce;};
        bool isBgSub() { return b_do_bg_sub;};
        void setGeocorr(bool enable) { b_do_geocorr = enable;};
        void setDebounce(bool enable) { b_do_debounce = enable;};
        void setBgSub(bool enable) { b_do_bg_sub = enable;};
        void setBgInit(bool enable) { b_bg_init = enable;};
        bool getBgInit() { return b_bg_init;};
        int32_t applyCorrections(StFrameBuffer &frame_src, StFrameBuffer &frame_dest);
        int32_t FrameBufferToMMPAD(StFrameBuffer &frame_src, mmpad_image_t &img_dest, e_mmpad_img_type data_type = MMPAD_DBL);
        int32_t createBgImage(int img_width, int img_height, int num_frames = 1);
        mmpad_image_t *getBgImage(){return bg_img;};
        int *getCapList(){return cap_list;};
        int *getCapReg(){return cap_reg;};
        int getCapCnt(){return cap_cnt;};
        void setCapCnt(int cnt){cap_cnt = cnt;};
        
    private:
        StCorrections()
            {
                int cap_idx;
                bg_img = nullptr;		// Always start with the background empty
                b_do_geocorr = false;
                b_do_debounce = false;
                b_do_bg_sub = false;
                debounce_method = 0;
                b_bg_init = false;

                for (cap_idx = 0; cap_idx < KK_MAX_CAPS; cap_idx++)
                {
                    cap_reg[cap_idx] = -1; // Point to invalid frame in stack
                    cap_list[cap_idx] = -1; // Flag as invalid
                }

                cap_cnt = 0;    // No caps to start
            }
    
        bool b_do_geocorr;		// Boolean to enable gecorrection
        bool b_do_debounce;		// Boolean to do debouncing
        bool b_do_bg_sub;		// Boolean to do background subtraction
        int debounce_method;	// Debounce method TODO Make an enum
        mmpad_image_t *bg_img;      // Holds the background image
        bool b_bg_init;             // Boolean to indicate background initialized
        int cap_reg[KK_MAX_CAPS]; // Where the valid caps point to
        int cap_list[KK_MAX_CAPS]; // Maps ordinal of cap number 
        int cap_cnt;              // Count of valid caps
    };
}

#endif

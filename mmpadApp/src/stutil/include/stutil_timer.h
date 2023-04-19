//******************************************************************
/// @file stutil_timer.h
/// @brief High Resolution Timer class
/// @author      W.R. Lear
/// @date        04/20/2019
/// @copyright   Copyright (c) 2019 Sydor Technologies, 
///              All rights reserved.
///
//******************************************************************
#ifndef STUTIL_TIMER_H
#define STUTIL_TIMER_H

#include <iostream>
#include <chrono>

//#define USE_HIGH_RES_CLOCK
namespace STUTIL
{
    //******************************************************************
    // Definitions and Constants
    //******************************************************************

    //******************************************************************
    // Data structures, enumerations and type definitions
    //******************************************************************
    typedef std::chrono::seconds Seconds;
    typedef std::chrono::milliseconds MilliSeconds;
    typedef std::chrono::microseconds MicroSeconds;
    typedef std::chrono::nanoseconds NanoSeconds;
    typedef std::chrono::duration<double> FpSeconds;

#define USE_HIGH_RES_CLOCK 0 //it isn't easy to mix the different clocks, so standardize on this one.

#if USE_HIGH_RES_CLOCK
    typedef std::chrono::high_resolution_clock AlClock;
#else
    typedef std::chrono::steady_clock AlClock;
#endif

    //******************************************************************
    // Class Definitions
    //******************************************************************

    //-=-= TODO: Add a resettable "Watchdog timer" option

    //------------------------------------------------------------------
    /** Timer Class
     * This is a simple wrapper around the chrono high resolution timer class
     */
    class Timer
    {
    private:
        AlClock::time_point mStart;
        AlClock::duration mStop;

    public:
        /// Get a 64-bit millisecond resolution timestamp
        static uint64_t getTimeStampMSec(void);

        /// Get a 64-bit microsecond resoluton timestamp
        static uint64_t getTimeStampUSec(void);

        /// Get a 64-bit nanosecond resoluton timestamp
        static uint64_t getTimeStampNSec(void);

        /// Get a 32-bit millisecond resolution timestamp
        static uint32_t getTimeStampMSec32(void);

        /// Get a 32-bit microsecond resoluton timestamp
        static uint32_t getTimeStampUSec32(void);

        /// Get a 32-bit nanosecond resoluton timestamp
        static uint32_t getTimeStampNSec32(void);

        /// Sleep until the specified time in uSec
        static void sleepUntilUSec(uint64_t uSec);

        /// Sleep until the specified time in mSec
        static void sleepUntilMSec(uint64_t mSec);

        /// Sleep until the specified time in Sec
        static void sleepUntilSec(uint64_t sec);

        /// Sleep for the specified duration in uSec
        static void sleepForUSec(uint64_t uSec);

        /// Sleep for the specified duration in mSec
        static void sleepForMSec(uint64_t mSec);

        /// Sleep for the specified duration in Sec
        static void sleepForSec(uint64_t sec);

        explicit Timer(bool run = false);
        ~Timer(void);

        /// "Start" the timer if not running
        void start(bool restart = false);

        /// "Stop" the timer if running
        void stop(void);

        /// Return true if timer is "Running"
        bool isRunning(void);
        
        /// Get timer resolution in fractional seconds
        double getResolutionSec(void) const;

        /// get timer resolution in nanoseconds
        uint64_t getResolutionNSec(void) const;

        /// get elapsed time as a duration, without stopping timer
        inline AlClock::duration getElapsedTime(void) const;

        /// Get elapsed time in fractional seconds, without stopping timer
        double getElapsedSec(void) const;

        /// Get elapsed time in milliseconds, without stopping timer
        uint64_t getElapsedMSec(void) const;

        /// Get elapsed time in microseconds, without stopping timer
        uint64_t getElapsedUSec(void) const;

        /// Get elapsed time in nanoseconds, without stopping timer
        uint64_t getElapsedNSec(void) const;

        /// Overload the << operator
        template <typename T, typename Traits>
        friend std::basic_ostream<T, Traits>& operator<<(std::basic_ostream<T, Traits>& out, const Timer& timer)
        {
            return out << timer.getElapsedSec();
        }
    };

} //namespace STUTIL

//******************************************************************
// End of file
//******************************************************************
#endif // STUTIL_TIMER_H

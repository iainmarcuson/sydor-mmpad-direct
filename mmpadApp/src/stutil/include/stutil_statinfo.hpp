//******************************************************************
/// @file stutil_statinfo.hpp
/// @brief Simple class for maintaining statistical information about a parameter
/// @author      W.R. Lear
/// @date        04/10/2022
///
/// StatInfo is a simple class for maintaining basic statistical information
/// about a time sequence of numeric parameter value samples. 
///
/// A StatInfo instance receives parameter sample updates (typically periodic,
/// but this is not required), and computes the following information:
///
///     minimum value
///     maximum value
///     accumulated total
///     number of value updates
///     average value
///     average rate of change (change in value per second)
///
/// All computations start fresh when the init() method is calledd.
///
/// The get() method returns a StatValues struct containing a snapshot
/// of all computed values. The values within the struct represent a 
/// single point in time, even if another thread is making simuntaneous
/// callse to add().
///
/// For convenience, overloads of the add() method are provided to allow
/// entry of signed and unsigned integer parameter data types, 
/// but all values are maintained and returned as doubles. 
/// Therefore, resolution will be reduced when the accumulated total
/// exceeds +/- 2**52
///


//******************************************************************
#ifndef STUTIL_STATINFO_HPP
#define STUTIL_STATINFO_HPP

#include <stdint.h>
#include <mutex>
#include "stutil_timer.h"

namespace STUTIL
{
//******************************************************************
// Definitions and Constants
//******************************************************************

//******************************************************************
// Data structures, enumerations and type definitions
//******************************************************************

//******************************************************************
// Class Definitions
//******************************************************************

//----------------------------------------------
/// Main Statistics class
class StatInfo
{
public:
    //----------------------------------------------
    /// Struct to return values
    struct StatValues
    {
    public:
        double lastValue;           ///< Last added value
        double minValue;            ///< minimum value
        double maxValue;            ///< maximum value
        double accumValue;          ///< accumulated total of all values
        double elapsedSeconds;      ///< time between first and last added value
        uint32_t count;             ///< number of values added
        double avgValue;            ///< average of all entered values
        double avgRate;             ///< average rate (accumValue/elapsedSeconds)
        bool   minChanged;          ///< true if minValue changed in last add
        bool   maxChanged;          ///< true if maxValue changed in last add
    };

private:
    StatValues mValues;             ///< curent values
    Timer      mTimer;              ///< elapsed time
    
    mutable std::mutex mMutex;      ///< access control mutex

public:
    //----------------------------------------------
    /// Constructor
    ///
    StatInfo(void)
    {
        init();
    }

    //----------------------------------------------
    /// Destructor
    ~StatInfo(void)
    {}

    //----------------------------------------------
    /// initialize all computations
    ///
    void init(void)
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mValues = {};
        mTimer.stop();
    }
    
    //----------------------------------------------
    /// Add a sample
    ///
    void add(double value)
    {
        std::unique_lock<std::mutex> lock(mMutex);
        if (0 == mValues.count)
        {
            // This is the first add since init()
            mTimer.start();
            mValues.accumValue = value;
            mValues.minValue   = value;
            mValues.maxValue   = value;
            mValues.avgValue   = value;
            mValues.avgRate    = 0.0;
            mValues.count      = 1;
        }
        else
        {
            // This is not our first rodeo
            mValues.elapsedSeconds = mTimer.getElapsedSec();
            mValues.count++;
            if (value > mValues.maxValue)
            {
                mValues.maxValue = value;
                mValues.minChanged = false;
                mValues.maxChanged = true;
            }
            else if (value < mValues.minValue)
            {
                mValues.minValue = value;
                mValues.minChanged = true;
                mValues.maxChanged = false;
            }
            else
            {
                mValues.minChanged = false;
                mValues.maxChanged = false;
            }
            mValues.accumValue += value;
            mValues.avgValue    = mValues.accumValue / mValues.count;
            mValues.avgRate     = mValues.accumValue / mValues.elapsedSeconds;
        }
        mValues.lastValue = value;
    }

    //----------------------------------------------
    /// Add a sample
    ///
    void add(uint32_t sample)
    {
        add(static_cast<double>(sample));
    }

    //----------------------------------------------
    /// Add a sample
    ///
    void add(int32_t sample)
    {
        add(static_cast<double>(sample));
    }

    //----------------------------------------------
    /// Add a sample
    ///
    void add(uint64_t sample)
    {
        add(static_cast<double>(sample));
    }

    //----------------------------------------------
    /// Add a sample
    ///
    void add(int64_t sample)
    {
        add(static_cast<double>(sample));
    }

    //----------------------------------------------
    /// Get the last value
    double getLast(void) { return mValues.lastValue; };

    //----------------------------------------------
    /// Get the average value
    double getAvg(void) { return mValues.avgValue; };

    //----------------------------------------------
    /// Get the minimum value
    double getMin(void) { return mValues.minValue; };

    //----------------------------------------------
    /// Get the maximum value
    double getMax(void) { return mValues.maxValue; };

    //----------------------------------------------
    /// Get the accumulated value
    double getAccum(void) { return mValues.accumValue; };

    //----------------------------------------------
    /// Get the value count
    double getCount(void) { return mValues.count; };

    //----------------------------------------------
    /// Get the elapsed time
    double getElapsed(void) { return mValues.elapsedSeconds; };

    //----------------------------------------------
    /// Get the average rate
    double getRate(void) { return mValues.avgRate; };

    //----------------------------------------------
    /// true if minimum value changed
    bool minChanged(void) { return mValues.minChanged;}

    //----------------------------------------------
    /// true if maximum value changed
    bool maxChanged(void) { return mValues.maxChanged;}

    //----------------------------------------------
    /// Get all current values
    ///
    StatInfo::StatValues get(void)
    {
        std::unique_lock<std::mutex> lock(mMutex);
        return mValues;
    }
};  // class StatInfo

} // namespace STUTIL

//******************************************************************
// End of file
//******************************************************************
#endif // STUTIL_STATINFO_H

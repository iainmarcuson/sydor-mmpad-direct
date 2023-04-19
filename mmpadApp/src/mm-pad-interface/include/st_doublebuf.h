//******************************************************************
/// @file st_doublebuf.h
/// @brief Simple double buffer template
/// @author      W.R. Lear
/// @date        10/06/2020
/// @copyright   2019,2020 Sydor Technologies, All rights reserved.
///
/// This file contains a very simple thread-safe templated double buffer
/// class used to obtain sample frame and telemetry data from the 
/// realtime supervisor.
///
/// @note Type 'T' assignment operator '=' must correctly copy the 
/// entire object. This includes scalars, arrays/structs
/// of scalars, and non-trivial classes which overload the
/// assignment operator.
/// 
/// @note This is NOT a circular queue. The output thread blocks while
/// the input thread is writing. The input thread blocks only when
/// the output thread is retrieving its pointer
///
//******************************************************************
#ifndef STUTIL_DOUBLEBUF_H
#define STUTIL_DOUBLEBUF_H

#include <stdint.h>
#include <stdbool.h>
#include <mutex>

namespace ST_INTERFACE
{
    
//------------------------------------------------------------------
/// DoubleBuf class
template<typename T>
class DoubleBuf
{
protected:

     T mData[2];            ///< Actual data buffers
  bool mIn0;
  bool mSwap;
  std::mutex mInputMutex;   ///< Mutex to serialize message access
  
public:
    //----------------------------------------------
    /// Constructor
    DoubleBuf(void)
        : mIn0(true)
        , mSwap(false)
    {
    }
    
    //----------------------------------------------
    /// Destructor 
    ~DoubleBuf()
    {
    }
    
    //----------------------------------------------
    /// copy data from a source object to the current input buffer
    /// 
    /// @param[in] src     pointer to source object to copy
    /// 
    void set(const T& src)
    {
        int index = mIn0 ? 0 : 1;
        std::lock_guard<std::mutex> guard(mInputMutex);
        mData[index] = src;
        mSwap = true;
    }

    //----------------------------------------------
    /// copy data from the output buffer to a destination object
    void get(T& dest)
    {
        std::lock_guard<std::mutex> guard(mInputMutex);
        if (mSwap)
        {
            mIn0 = !mIn0;
            mSwap = false;
        }
        if(mIn0)
        {
            dest = mData[1];
        }
        else
        {
            dest = mData[0];
        }
   }

    //------------------------------------------------------------------
    // Low level methods - typically not used 
    //------------------------------------------------------------------

    //----------------------------------------------
    /// Lock input and return pointer to buffer.
    T* getLockedInputPtr(void)
    {
        mInputMutex.lock();
        return mIn0 ? &mData[0] : &mData[1];
    }

    //----------------------------------------------
    /// release input and indicate swap is needed
    void releaseInput(void)
    {
        mSwap = true;
        mInputMutex.unlock();
    }

    //----------------------------------------------
    /// get a pointer to the current output buffer
    T* getOutputPtr(void)
    {
        std::lock_guard<std::mutex> guard(mInputMutex);
        if (mSwap)
        {
            mIn0 = !mIn0;
            mSwap = false;
        }
        return mIn0 ? &mData[1] : &mData[0];
    }

};  // class DoubleBuf

} // namespace STUTIL
//******************************************************************
// End of file
//******************************************************************
#endif // STUTIL_DOUBLEBUF_H

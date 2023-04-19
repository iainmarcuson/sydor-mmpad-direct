//******************************************************************
/// @file stutil_queue.hpp
/// @brief Simple thread safe multi-producer/multi-consumer queue template
/// @author      W.R. Lear
/// @date        07/25/2020
/// @copyright   Copyright (c) 2020 Sydor Technologies,  
///              All rights reserved.
///
/// SafeQueue is a templated queue class that is safe to use
/// for multi-producer/multi-consumer applications, and maintains
/// statistics about queue usage.
///
//******************************************************************
#ifndef STUTIL_QUEUE_HPP
#define STUTIL_QUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>

namespace STUTIL
{
//******************************************************************
// Definitions and Constants
//******************************************************************

//******************************************************************
// Data structures, enumerations and type definitions
//******************************************************************

//------------------------------------------------------------------
/// Queue statistics struct
typedef struct
{
    double   accumEntries;          ///< accum number of entries for average
    uint32_t pushCount;             ///< number of pushes since reset
    uint32_t maxEntries;            ///< max nmber of queue entries since reset
    uint32_t minEntries;            ///< minimum number of queue entries since reset
    uint32_t overflowCount;         ///< number of overflows since reset
} QueueInfo;

//******************************************************************
// Class Definitions
//******************************************************************

//----------------------------------------------
/// Main Thread Safe queue class
template <class T>
class SafeQueue
{
private:
    std::queue<T> mQueue;               ///< base queue
    mutable std::mutex mQMutex;         ///< access control mutex
    std::condition_variable mQReady;    ///< queue data available signal 
    uint32_t mQueueLength;              ///< max queue length (0 = no limit)
    QueueInfo mInfo;                    ///< statistics

public:
    //----------------------------------------------
    /// Constructor
    ///
    /// @param length       optional max queue length (default = no limit)
    SafeQueue(int32_t length = 0)
        : mQueue()
        , mQMutex()
        , mQReady()
        , mQueueLength(length)
    {
        resetStatistics();
    }

    //----------------------------------------------
    /// Destructor
    ~SafeQueue(void)
    {}

    //----------------------------------------------
    /// Add an entry to the queue.
    /// Tracks overflows but does not enforce max queue length
    ///
    /// @param entry        entry to add
    ///
    void push(T entry)
    {
        std::unique_lock<std::mutex> lock(mQMutex);
        mQueue.push(entry);

        // update statistics
        mInfo.pushCount++;
        uint32_t entryCount = (uint32_t)mQueue.size();
        if (entryCount > mInfo.maxEntries)
        {
            mInfo.maxEntries = entryCount;
        }
        mInfo.accumEntries += entryCount;
        
        // Keep track of overflows
        if (entryCount > mQueueLength)
        {
            mInfo.overflowCount++;
        }
        lock.unlock();
        mQReady.notify_one();
    }

    //----------------------------------------------
    /// Add an entry to queue if not full
    ///
    /// @param entry        entry to add
    ///
    /// @return false and increment overflow count if the queue is full
    bool tryPush(T entry)
    {
        std::unique_lock<std::mutex> lock(mQMutex);
        int32_t entryCount = (int32_t)mQueue.size() + 1;
        if (entryCount <= mQueueLength)
        {
            mQueue.push(entry);

            // update statistics
            mInfo.pushCount++;
            if (entryCount > mInfo.maxEntries)
            {
                mInfo.maxEntries = entryCount;
            }
            mInfo.accumEntries += entryCount;

            lock.unlock();
            mQReady.notify_one();
            return true;
        }
        else
        {
            mInfo.overflowCount++;
            return false;
        }
    }

    //----------------------------------------------
    /// remove an entry from the queue.
    /// wait if the queue is empty
    T pop(void)
    {
        std::unique_lock<std::mutex> lock(mQMutex);
        while (mQueue.empty())
        {
            mQReady.wait(lock, [this] { return !mQueue.empty(); });
        }
        T val = std::move(mQueue.front());
        mQueue.pop();
        if (mQueue.size() < mInfo.minEntries)
        {
            mInfo.minEntries = mQueue.size();
        }
        return val;
    }

    //----------------------------------------------
    /// Try to get an entry from the queue
    ///
    /// @param[out] entry       reference to returned entry
    /// @param[in] timeout      optional max mSec to wait (default = 0)
    ///
    /// @return false if the queue is empty
    ///
    bool tryPop(T& entry, std::chrono::milliseconds timeout = std::chrono::milliseconds::zero())
    {
        std::unique_lock<std::mutex> lock(mQMutex);
        if (!mQReady.wait_for(lock, timeout, [this] { return !mQueue.empty(); }))
        {
            if (mQueue.empty())
            {
                return false;
            }
        }
        entry = std::move(mQueue.front());
        mQueue.pop();
        if (mQueue.size() < mInfo.minEntries)
        {
            mInfo.minEntries = static_cast<uint32_t>(mQueue.size());
        }
        return true;
    }

    //----------------------------------------------
    /// Get the overflow queue length for overflow (0 if no limit)
    uint32_t getQueueLength(void) { return mQueueLength; }

    //----------------------------------------------
    // Set the overflow queue length (0 if no limit)
    void setQueueLength(int32_t length) { mQueueLength = length; }

    //----------------------------------------------
    /// Get a snapshot of the current number of entries in the queue
    uint32_t getSize() { return (int32_t)mQueue.size(); }

    //----------------------------------------------
    /// Return true if queue is empty (but may change immediately)
    bool isEmpty() { return mQueue.empty(); }

    //----------------------------------------------
    /// Get average number of queue entries since the last reset
    double getAverageSize()
    {
        std::lock_guard<std::mutex> lock(mQMutex);
        return mInfo.accumEntries / mInfo.pushCount;
    }

    //----------------------------------------------
    /// Get highest number of entries in the queue since the last reset
    uint32_t getMaxEntries() { return mInfo.maxEntries; }

    //----------------------------------------------
    /// Get the number of overflows since the last reset
    uint32_t getOverflowCount() { return mInfo.overflowCount; }

    //----------------------------------------------
    /// Get the number of pushes since the last reset
    uint32_t getPushCount() { return mInfo.pushCount; }

    //----------------------------------------------
    /// Reset the queue statistics
    void resetStatistics(void)
    {
        std::lock_guard<std::mutex> lock(mQMutex);
        uint32_t nEntries = getSize();
        mInfo.accumEntries = nEntries;
        mInfo.maxEntries = nEntries;
        mInfo.minEntries = nEntries;
        mInfo.pushCount = 0;
        mInfo.overflowCount = 0;
    }

    //----------------------------------------------
    /// flush the queue contents
    void flush(void)
    {
        std::lock_guard<std::mutex> lock(mQMutex);
        while (!mQueue.empty())
        {
            mQueue.pop();
        }
    }

};  // class SafeQueue

} // namespace STUTIL

//******************************************************************
// End of file
//******************************************************************
#endif // STUTIL_QUEUE_H

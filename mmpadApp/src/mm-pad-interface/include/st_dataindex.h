//******************************************************************
/// @file st_dataindex.h
/// @brief Class to contain description of sets and runs on X-PAD server
/// @author      W.R. Lear
/// @date        10/23/2019
/// @copyright   2019 Sydor Technologies, All rights reserved.
///
/// This file defines the StDataIndex class which is used
/// to describe the collection of capture sets and runs
/// available on a Sydor X-PAD Server
///
//******************************************************************
#ifndef ST_DATAINDEX_H
#define ST_DATAINDEX_H

#include <stdint.h>
#include <string>
#include "st_if_defs.h"
#include "stutil_string.h"
#include "stutil_logger.h"
#include "nlohmann_json.hpp"

namespace ST_INTERFACE
{

//******************************************************************
/// StRunDef class
///
/// This class contains all information for a single capture run
/// in the Data Index
///
class StRunDef
{
protected:
    STUTIL::Logger* pLog;
    std::string mName;
    uint64_t mTimeStamp;
    mutable uint32_t mFrameCount;
    mutable uint32_t mFrameFileCount;
    mutable uint32_t mCapCount;
    mutable uint32_t mCapSelect;
    std::string mDmaError;
    std::string mDiskError;

public:

    //----------------------------------------------
    /// Constructor
    StRunDef(void);
    
    //----------------------------------------------
    /// Destructor 
    ~StRunDef();
    
    //----------------------------------------------
    /// get name
    std::string getName(void) const { return mName; }

    //----------------------------------------------
    /// set name
    void setName(const std::string& name) { mName = name; }

    //----------------------------------------------
    /// get timestamp
    uint64_t getTimeStamp(void) const { return mTimeStamp; }

    //----------------------------------------------
    /// set timestamp
    void setTimeStamp(uint64_t value) { mTimeStamp = value; }

    //----------------------------------------------
    /// get frame count
    uint32_t getFrameCount(void) const { return mFrameCount; }

    //----------------------------------------------
    /// set frame count
    void setFrameCount(uint32_t value) { mFrameCount = value; }

    //----------------------------------------------
    /// get the capacitor count
    uint32_t getCapCount(void) const { return mCapCount; }

    //----------------------------------------------
    /// set the capacitor count
    void setCapCount(uint32_t value) { mCapCount = value; }

    //----------------------------------------------
    /// get the capacitor select flags
    uint32_t getCapSelect(void) const { return mCapSelect; }

    //----------------------------------------------
    /// set the capacitor select
    void setCapSelect(uint32_t value) { mCapSelect = value; }

    //----------------------------------------------
    /// get frame file count
    uint32_t getFrameFileCount(void) const { return mFrameFileCount; }

    //----------------------------------------------
    /// get the run DMA/driver error
    std::string getDmaError(void) const { return mDmaError; }

    //----------------------------------------------
    /// set the run DMA/driver error
    void setDmaError(std::string dmaError) { mDmaError = dmaError; }

    //----------------------------------------------
    /// get the disk error status for the run
    std::string getDiskError(void) const { return mDiskError; }

    //----------------------------------------------
    /// set he disk error status for the run
    void setDiskError(std::string diskError) { mDiskError = diskError; }
    
    //----------------------------------------------
    /// De-serialize from a JSON string
    int32_t fromJson(const std::string& jsonStr);

    //----------------------------------------------
    /// De-serialize from a JSON object
    int32_t fromJson(const nlohmann::json& j);
    
    //----------------------------------------------
    /// Serialize to a JSON object
    void toJson(nlohmann::json& j) const;

    //----------------------------------------------
    /// Serialize to a JSON string
    void toJson(std::string jsonStr, int32_t indent = 4) const;

}; // Class StRunDef


//******************************************************************
/// StSetDef class
///
/// This class contains all information for a single capture set
/// in the Data Index
///
class StSetDef
{
protected:
    STUTIL::Logger* pLog;
    uint64_t mTimeStamp;
    std::string mName;
    std::string mDescription;
    std::string mTags;
    std::vector<StRunDef> mRuns;
    mutable uint64_t mFrameCount;
    mutable uint32_t mFrameFileCount;

public:

    //----------------------------------------------
    /// Constructor
    StSetDef(void);
    
    //----------------------------------------------
    /// Destructor 
    ~StSetDef();

    static bool compareRuns(StRunDef& run1, StRunDef& run2)
    {
        return run1.getTimeStamp() < run2.getTimeStamp();
    }

    void sortRuns()
    {
        std::sort(mRuns.begin(), mRuns.end(), StSetDef::compareRuns);
    }

    //----------------------------------------------
    /// get name
    std::string getName(void) const { return mName; }

    //----------------------------------------------
    /// set name
    void setName(const std::string& name) { mName = name; }

    //----------------------------------------------
    /// get description
    std::string getDescription(void) const { return mDescription; }

    //----------------------------------------------
    /// set description
    void setDescription(const std::string& description) { mDescription = description; }

    //----------------------------------------------
    /// get tags
    std::string gettags(void) const { return mTags; }

    //----------------------------------------------
    /// set tags
    void setTags(const std::string& tags) { mTags = tags; }

    //----------------------------------------------
    /// get timestamp
    uint64_t getTimeStamp(void) const { return mTimeStamp; }

    //----------------------------------------------
    /// set timestamp
    void setTimeStamp(uint64_t value) { mTimeStamp = value; }

    //----------------------------------------------
    /// get run count
    uint32_t getRunCount(void) const { return static_cast<uint32_t>(mRuns.size()); }

    //----------------------------------------------
    /// get total frame count
    uint64_t getFrameCount(void) const { return mFrameCount; }

    //----------------------------------------------
    /// get total frame file count
    uint32_t getFrameFileCount(void) const { return mFrameFileCount; }

    //----------------------------------------------
    /// add a run definition to set
    void addRun(const StRunDef& rundef);

    //----------------------------------------------
    std::vector<StRunDef> getRuns(void) const { return mRuns; }
   
    //----------------------------------------------
    /// get the specified run definition
    StRunDef getRun(uint32_t index) const;

    //----------------------------------------------
    /// get the index of the specified run
    uint32_t getRunIndex(const std::string& name, bool exact = false);

    //----------------------------------------------
    /// get the specified run definition
    StRunDef getRun(const std::string& name, bool exact = false);
        
    //----------------------------------------------
    /// De-serialize from a JSON string
    int32_t fromJson(const std::string& jsonStr);

    //----------------------------------------------
    /// De-serialize from a JSON object
    int32_t fromJson(const nlohmann::json& j);

    //----------------------------------------------
    /// Serialize to a JSON object
    void toJson(nlohmann::json& j) const;

    //----------------------------------------------
    /// Serialize to a JSON string
    void toJson(std::string jsonStr, int32_t indent = 4) const;

}; // Class StSetDef
   
//******************************************************************
/// StDataIndex class
class StDataIndex
{
protected:
    STUTIL::Logger* pLog;
    uint64_t mTimeStamp;
    mutable uint32_t mRunCount;
    mutable uint64_t mFrameCount;
    mutable uint32_t mFrameFileCount;
    std::vector<StSetDef> mSets;
  
public:
    //----------------------------------------------
    /// Constructor
    StDataIndex(void);
    
    //----------------------------------------------
    /// Destructor 
    ~StDataIndex();

    //----------------------------------------------
    // Clear the content
    void clear(void);

    static bool compareSets(StSetDef& set1, StSetDef& set2)
    {
        return STUTIL::toLower(set1.getName()) < STUTIL::toLower(set2.getName());
    }

    void sortSets()
    {
        std::sort(mSets.begin(), mSets.end(), StDataIndex::compareSets);
    }

    //----------------------------------------------
    /// get set count
    uint32_t getSetCount(void) const { return static_cast<uint32_t>(mSets.size()); }

    //----------------------------------------------
    /// get total run count
    uint32_t getRunCount(void) const;

    //----------------------------------------------
    // get run count for specified set index
    uint32_t getRunCount(uint32_t setIndex);

    //----------------------------------------------
    /// get total frame count
    uint64_t getFrameCount(void) const { return mFrameCount; }

    //----------------------------------------------
    /// get total frame file count
    uint32_t getFrameFileCount(void) const { return mFrameFileCount; }

    //----------------------------------------------
    /// add a set definition to set
    void addSet(const StSetDef& setdef);

    //----------------------------------------------
    /// get the collection of all sets
    std::vector<StSetDef> getSets(void) const { return mSets; }

    //----------------------------------------------
    /// get the specified set
    StSetDef getSet(uint32_t index) const;

    //----------------------------------------------
    /// get the index of the specified set
    uint32_t  getSetIndex(const std::string& name, bool exact = false);

    //----------------------------------------------
    /// get the specified set definition
    StSetDef getSet(const std::string& name, bool exact = false);

    //----------------------------------------------
    /// get the index of the specified run
    uint32_t getRunIndex(uint32_t setIndex, const std::string& name, bool exact = false);

    //----------------------------------------------
    /// get the specified run
    StRunDef getRun(uint32_t setIndex, const std::string& name, bool exact = false);
     
    //----------------------------------------------
    /// De-serialize from a JSON object
    ///
    /// @param[in] j     JSON object
    ///
    /// @return 0 if ok, negative error code on any error
    ///
    int32_t fromJson(const nlohmann::json& j);
    
    //----------------------------------------------
    /// De-serialize from a JSON string
    int32_t fromJson(const std::string& jsonStr);

    //----------------------------------------------
    /// De-serialize from a JSON string
    int32_t fromJson(const char* jsonStr) { return fromJson(std::string(jsonStr)); }

    //----------------------------------------------
    /// Serialize to a JSON object
    void toJson(nlohmann::json& j) const;  

    //----------------------------------------------
    /// Serialize to a JSON string
    void toJson(std::string& jsonStr, int32_t indent = 4) const;

private:

};  // class StDataIndex

} // namespace ST_INTERFACE
//******************************************************************
// End of file
//******************************************************************
#endif // ST_DATAINDEX_H

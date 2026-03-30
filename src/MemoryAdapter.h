#include "ClassAdapter.h"
#include "Robot.h"

class MemoryAdapter :
  public ClassAdapter<MemoryAdapter, Robot::Memory> {

  public:
    static Napi::Function Init(Napi::Env env);
    
    MemoryAdapter(const Napi::CallbackInfo& info);

    Napi::Value isValid(const Napi::CallbackInfo& info);
    Napi::Value getProcess(const Napi::CallbackInfo& info);
    Napi::Value getStats(const Napi::CallbackInfo& info);
    Napi::Value getRegion(const Napi::CallbackInfo& info);
    Napi::Value getRegions(const Napi::CallbackInfo& info);
    Napi::Value setAccess(const Napi::CallbackInfo& info);
    Napi::Value getPtrSize(const Napi::CallbackInfo& info);
    Napi::Value getMinAddress(const Napi::CallbackInfo& info);
    Napi::Value getMaxAddress(const Napi::CallbackInfo& info);
    Napi::Value getPageSize(const Napi::CallbackInfo& info);
    Napi::Value find(const Napi::CallbackInfo& info);
    Napi::Value createCache(const Napi::CallbackInfo& info);
    void clearCache(const Napi::CallbackInfo& info);
    void deleteCache(const Napi::CallbackInfo& info);
    Napi::Value isCaching(const Napi::CallbackInfo& info);
    Napi::Value getCacheSize(const Napi::CallbackInfo& info);
    Napi::Value readData(const Napi::CallbackInfo& info);
    Napi::Value writeData(const Napi::CallbackInfo& info);
    Napi::Value readInt8(const Napi::CallbackInfo& info);
    Napi::Value readInt16(const Napi::CallbackInfo& info);
    Napi::Value readInt32(const Napi::CallbackInfo& info);
    Napi::Value readInt64(const Napi::CallbackInfo& info);
    Napi::Value readReal32(const Napi::CallbackInfo& info);
    Napi::Value readReal64(const Napi::CallbackInfo& info);
    Napi::Value readPtr(const Napi::CallbackInfo& info);
    Napi::Value readBool(const Napi::CallbackInfo& info);
    Napi::Value readString(const Napi::CallbackInfo& info);
    Napi::Value writeInt8(const Napi::CallbackInfo& info);
    Napi::Value writeInt16(const Napi::CallbackInfo& info);
    Napi::Value writeInt32(const Napi::CallbackInfo& info);
    Napi::Value writeInt64(const Napi::CallbackInfo& info);
    Napi::Value writeReal32(const Napi::CallbackInfo& info);
    Napi::Value writeReal64(const Napi::CallbackInfo& info);
    Napi::Value writePtr(const Napi::CallbackInfo& info);
    Napi::Value writeBool(const Napi::CallbackInfo& info);
    Napi::Value writeString(const Napi::CallbackInfo& info);

    class StatsAdapter :
      public ClassAdapter<StatsAdapter, Robot::Memory::Stats>,
      public ClassAdapter<StatsAdapter, Robot::Memory::Stats>::Eq {

      public:
        static Napi::Function Init(Napi::Env env);
        
        StatsAdapter(const Napi::CallbackInfo& info);

        Napi::Value systemReads(const Napi::CallbackInfo& info);
        void systemReads(const Napi::CallbackInfo& info, const Napi::Value& value);
        Napi::Value cachedReads(const Napi::CallbackInfo& info);
        void cachedReads(const Napi::CallbackInfo& info, const Napi::Value& value);
        Napi::Value systemWrites(const Napi::CallbackInfo& info);
        void systemWrites(const Napi::CallbackInfo& info, const Napi::Value& value);
        Napi::Value accessWrites(const Napi::CallbackInfo& info);
        void accessWrites(const Napi::CallbackInfo& info, const Napi::Value& value);
        Napi::Value readErrors(const Napi::CallbackInfo& info);
        void readErrors(const Napi::CallbackInfo& info, const Napi::Value& value);
        Napi::Value writeErrors(const Napi::CallbackInfo& info);
        void writeErrors(const Napi::CallbackInfo& info, const Napi::Value& value);

    };

    class RegionAdapter :
      public ClassAdapter<RegionAdapter, Robot::Memory::Region>,
      public ClassAdapter<RegionAdapter, Robot::Memory::Region>::Eq,
      public ClassAdapter<RegionAdapter, Robot::Memory::Region>::Cmp {

      public:
        static Napi::Function Init(Napi::Env env);
        
        RegionAdapter(const Napi::CallbackInfo& info);

        Napi::Value valid(const Napi::CallbackInfo& info);
        void valid(const Napi::CallbackInfo& info, const Napi::Value& value);
        Napi::Value bound(const Napi::CallbackInfo& info);
        void bound(const Napi::CallbackInfo& info, const Napi::Value& value);
        Napi::Value start(const Napi::CallbackInfo& info);
        void start(const Napi::CallbackInfo& info, const Napi::Value& value);
        Napi::Value stop(const Napi::CallbackInfo& info);
        void stop(const Napi::CallbackInfo& info, const Napi::Value& value);
        Napi::Value size(const Napi::CallbackInfo& info);
        void size(const Napi::CallbackInfo& info, const Napi::Value& value);
        Napi::Value readable(const Napi::CallbackInfo& info);
        void readable(const Napi::CallbackInfo& info, const Napi::Value& value);
        Napi::Value writable(const Napi::CallbackInfo& info);
        void writable(const Napi::CallbackInfo& info, const Napi::Value& value);
        Napi::Value executable(const Napi::CallbackInfo& info);
        void executable(const Napi::CallbackInfo& info, const Napi::Value& value);
        Napi::Value access(const Napi::CallbackInfo& info);
        void access(const Napi::CallbackInfo& info, const Napi::Value& value);
        Napi::Value private_(const Napi::CallbackInfo& info);
        void private_(const Napi::CallbackInfo& info, const Napi::Value& value);
        Napi::Value guarded(const Napi::CallbackInfo& info);
        void guarded(const Napi::CallbackInfo& info, const Napi::Value& value);

        Napi::Value contains(const Napi::CallbackInfo& info);
    };

  private:
    Napi::Value readTypeImpl(const Napi::CallbackInfo& info, int type, Robot::uint32 length, int countIdx);
    Napi::Value writeTypeImpl(const Napi::CallbackInfo& info, int type, Robot::uint32 length);

};

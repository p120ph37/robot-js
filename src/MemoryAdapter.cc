#include "MemoryAdapter.h"
#include "RangeAdapter.h"
#include "ProcessAdapter.h"

namespace {
  enum DataType {
    TypeInt8   = 1,
    TypeInt16  = 2,
    TypeInt32  = 3,
    TypeInt64  = 4,
    TypeReal32 = 5,
    TypeReal64 = 6,
    TypeBool   = 7,
    TypeString = 8,
  };
}

Napi::Function MemoryAdapter::Init(Napi::Env env) {
  return DefineClass(env, "Memory", {
    StaticValue("Stats", MemoryAdapter::StatsAdapter::GetConstructor(env)),
    StaticValue("Region", MemoryAdapter::RegionAdapter::GetConstructor(env)),
    StaticValue("DEFAULT", Napi::Number::New(env, Robot::Memory::Default)),
    StaticValue("SKIP_ERRORS", Napi::Number::New(env, Robot::Memory::SkipErrors)),
    StaticValue("AUTO_ACCESS", Napi::Number::New(env, Robot::Memory::AutoAccess)),
    InstanceMethod("isValid", &MemoryAdapter::isValid),
    InstanceMethod("getProcess", &MemoryAdapter::getProcess),
    InstanceMethod("getStats", &MemoryAdapter::getStats),
    InstanceMethod("getRegion", &MemoryAdapter::getRegion),
    InstanceMethod("getRegions", &MemoryAdapter::getRegions),
    InstanceMethod("setAccess", &MemoryAdapter::setAccess),
    InstanceMethod("getPtrSize", &MemoryAdapter::getPtrSize),
    InstanceMethod("getMinAddress", &MemoryAdapter::getMinAddress),
    InstanceMethod("getMaxAddress", &MemoryAdapter::getMaxAddress),
    InstanceMethod("getPageSize", &MemoryAdapter::getPageSize),
    InstanceMethod("find", &MemoryAdapter::find),
    InstanceMethod("createCache", &MemoryAdapter::createCache),
    InstanceMethod("clearCache", &MemoryAdapter::clearCache),
    InstanceMethod("deleteCache", &MemoryAdapter::deleteCache),
    InstanceMethod("isCaching", &MemoryAdapter::isCaching),
    InstanceMethod("getCacheSize", &MemoryAdapter::getCacheSize),
    InstanceMethod("readData", &MemoryAdapter::readData),
    InstanceMethod("writeData", &MemoryAdapter::writeData),
    InstanceMethod("readInt8", &MemoryAdapter::readInt8),
    InstanceMethod("readInt16", &MemoryAdapter::readInt16),
    InstanceMethod("readInt32", &MemoryAdapter::readInt32),
    InstanceMethod("readInt64", &MemoryAdapter::readInt64),
    InstanceMethod("readReal32", &MemoryAdapter::readReal32),
    InstanceMethod("readReal64", &MemoryAdapter::readReal64),
    InstanceMethod("readPtr", &MemoryAdapter::readPtr),
    InstanceMethod("readBool", &MemoryAdapter::readBool),
    InstanceMethod("readString", &MemoryAdapter::readString),
    InstanceMethod("writeInt8", &MemoryAdapter::writeInt8),
    InstanceMethod("writeInt16", &MemoryAdapter::writeInt16),
    InstanceMethod("writeInt32", &MemoryAdapter::writeInt32),
    InstanceMethod("writeInt64", &MemoryAdapter::writeInt64),
    InstanceMethod("writeReal32", &MemoryAdapter::writeReal32),
    InstanceMethod("writeReal64", &MemoryAdapter::writeReal64),
    InstanceMethod("writePtr", &MemoryAdapter::writePtr),
    InstanceMethod("writeBool", &MemoryAdapter::writeBool),
    InstanceMethod("writeString", &MemoryAdapter::writeString),
  });
}

MemoryAdapter::MemoryAdapter(const Napi::CallbackInfo& info) : ClassAdapter(info) {
  if(WrapAdaptee(info) || ConstructDefault(info) || CopyThat(info)) return;
  if(ProcessAdapter::IsInstance(info[0])) {
    adaptee = Robot::Memory(ProcessAdapter::Unwrap(info[0])->adaptee);
    return;
  }
  throw Napi::TypeError::New(env, "Invalid arguments");
}

Napi::Value MemoryAdapter::isValid(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.IsValid());
}

Napi::Value MemoryAdapter::getProcess(const Napi::CallbackInfo& info) {
  return ProcessAdapter::New(env, adaptee.GetProcess());
}

Napi::Value MemoryAdapter::getStats(const Napi::CallbackInfo& info) {
  return MemoryAdapter::StatsAdapter::New(env, adaptee.GetStats(info[0].IsUndefined() ? false : info[0].As<Napi::Boolean>()));
}

Napi::Value MemoryAdapter::getRegion(const Napi::CallbackInfo& info) {
  return MemoryAdapter::RegionAdapter::New(env, adaptee.GetRegion(info[0].As<Napi::Number>().Int64Value()));
}

Napi::Value MemoryAdapter::getRegions(const Napi::CallbackInfo& info) {
  auto regions = adaptee.GetRegions(
    info[0].IsUndefined() ? 0 : info[0].As<Napi::Number>(),
    info[1].IsUndefined() ? -1 : info[1].As<Napi::Number>()
  );
  auto arr = Napi::Array::New(env, regions.size());
  for(size_t i = 0; i < regions.size(); i++) {
    arr[i] = MemoryAdapter::RegionAdapter::New(env, regions[i]);
  }
  return arr;
}

Napi::Value MemoryAdapter::setAccess(const Napi::CallbackInfo& info) {
  if(MemoryAdapter::RegionAdapter::IsInstance(info[0])) {
    if(info[1].IsNumber()) {
      return Napi::Boolean::New(env, adaptee.SetAccess(
        MemoryAdapter::RegionAdapter::Unwrap(info[0])->adaptee,
        info[1].As<Napi::Number>().Int64Value()
      ));
    }
    return Napi::Boolean::New(env, adaptee.SetAccess(
        MemoryAdapter::RegionAdapter::Unwrap(info[0])->adaptee,
        info[1].ToBoolean(),
        info[2].ToBoolean(),
        info[3].ToBoolean()
    ));
  }
  throw Napi::TypeError::New(env, "Invalid arguments");
}

Napi::Value MemoryAdapter::getPtrSize(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.GetPtrSize());
}

Napi::Value MemoryAdapter::getMaxAddress(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.GetMaxAddress());
}

Napi::Value MemoryAdapter::getMinAddress(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.GetMinAddress());
}

Napi::Value MemoryAdapter::getPageSize(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.GetPageSize());
}

Napi::Value MemoryAdapter::find(const Napi::CallbackInfo& info) {
  auto addresses = adaptee.Find(
    info[0].ToString().Utf8Value().c_str(),
    info[1].IsUndefined() ? 0 : info[1].As<Napi::Number>(),
    info[2].IsUndefined() ? -1 : info[2].As<Napi::Number>(),
    info[3].IsUndefined() ? 0 : info[3].As<Napi::Number>(),
    info[4].IsUndefined() ? nullptr : info[4].ToString().Utf8Value().c_str()
  );
  auto arr = Napi::Array::New(env, addresses.size());
  for(size_t i = 0; i < addresses.size(); i++) {
    arr[i] = Napi::Number::New(env, addresses[i]);
  }
  return arr;
}

Napi::Value MemoryAdapter::createCache(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.CreateCache(
    info[0].As<Napi::Number>().Int64Value(),
    info[1].As<Napi::Number>().Int64Value(),
    info[2].As<Napi::Number>().Int64Value(),
    info[3].IsUndefined() ? 0 : info[3].As<Napi::Number>().Int64Value(),
    info[4].IsUndefined() ? 0 : info[4].As<Napi::Number>().Int64Value()
  ));
}

void MemoryAdapter::clearCache(const Napi::CallbackInfo& info) {
  adaptee.ClearCache();
}

void MemoryAdapter::deleteCache(const Napi::CallbackInfo& info) {
  adaptee.DeleteCache();
}

Napi::Value MemoryAdapter::isCaching(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.IsCaching());
}

Napi::Value MemoryAdapter::getCacheSize(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.GetCacheSize());
}

Napi::Value MemoryAdapter::readData(const Napi::CallbackInfo& info) {
  if(!info[1].IsBuffer()) {
    throw Napi::TypeError::New(env, "Invalid arguments");
  }
  auto buffer = info[1].As<Napi::Buffer<uint8_t>>();
  uint64_t length = info[2].IsUndefined() ? buffer.Length() : info[2].As<Napi::Number>().Int64Value();
  if(buffer.Length() < length) {
    throw Napi::RangeError::New(env, "Buffer is too small");
  }
  return Napi::Number::New(env, adaptee.ReadData(
    info[0].As<Napi::Number>().Int64Value(),
    buffer.Data(),
    length,
    info[3].IsUndefined() ? Robot::Memory::Default : (Robot::Memory::Flags)(info[3].As<Napi::Number>().Int32Value())
  ));
}

Napi::Value MemoryAdapter::writeData(const Napi::CallbackInfo& info) {
  if(!info[1].IsBuffer()) {
    throw Napi::TypeError::New(env, "Invalid arguments");
  }
  auto buffer = info[1].As<Napi::Buffer<uint8_t>>();
  uint64_t length = info[2].IsUndefined() ? buffer.Length() : info[2].As<Napi::Number>().Int64Value();
  if(buffer.Length() < length) {
    throw Napi::RangeError::New(env, "Buffer is too small");
  }
  return Napi::Number::New(env, adaptee.WriteData(
    info[0].As<Napi::Number>().Int64Value(),
    buffer.Data(),
    length,
    info[3].IsUndefined() ? Robot::Memory::Default : (Robot::Memory::Flags)(info[3].As<Napi::Number>().Int32Value())
  ));
}

Napi::Value MemoryAdapter::readTypeImpl(const Napi::CallbackInfo& info, int type_, Robot::uint32 length, int countIdx) {
  auto address = (Robot::uintptr) info[0].As<Napi::Number>().DoubleValue();
  auto type    = (DataType) type_;
  auto count   = (Robot::uint32) 1;
  auto stride  = (Robot::uint32) 0;

  if(!info[countIdx].IsUndefined())   count  = info[countIdx].As<Napi::Number>().Uint32Value();
  if(!info[countIdx+1].IsUndefined()) stride = info[countIdx+1].As<Napi::Number>().Uint32Value();

  if(count == 0 || length == 0) return env.Null();

  if(count == 1) {
    if(type == TypeString) {
      auto data = new char[length];
      if(adaptee.ReadData(address, data, length) != length)
        { delete[] data; return env.Null(); }
      auto res = Napi::String::New(env, std::string(data, length).data());
      delete[] data; return res;
    } else {
      Robot::int64 data = 0;
      if(adaptee.ReadData(address, &data, length) != length)
        return env.Null();
      switch(type) {
        case TypeInt8  : return Napi::Number::New(env,  (int32_t) *(int8_t  *) &data);
        case TypeInt16 : return Napi::Number::New(env,  (int32_t) *(int16_t *) &data);
        case TypeInt32 : return Napi::Number::New(env,  (int32_t) *(int32_t *) &data);
        case TypeInt64 : return Napi::Number::New(env,  (double)  *(Robot::int64 *) &data);
        case TypeReal32: return Napi::Number::New(env,  (double)  *(float   *) &data);
        case TypeReal64: return Napi::Number::New(env,  (double)  *(double  *) &data);
        case TypeBool  : return Napi::Boolean::New(env, (bool)    *(bool    *) &data);
        default: return env.Null();
      }
    }
  }

  if(stride == 0) stride = length;
  if(stride < length) throw Napi::RangeError::New(env, "Stride is too small");

  auto size   = (Robot::uintptr)count * stride + length - stride;
  auto data   = new char[size];
  auto offset = data;

  if(adaptee.ReadData(address, data, size) != size)
    { delete[] data; return env.Null(); }

  auto res = Napi::Array::New(env, count);
  for(Robot::uint32 i = 0; i < count; ++i, offset += stride) {
    switch(type) {
      case TypeInt8  : res[i] = Napi::Number::New(env,  (int32_t) *(int8_t  *) offset); continue;
      case TypeInt16 : res[i] = Napi::Number::New(env,  (int32_t) *(int16_t *) offset); continue;
      case TypeInt32 : res[i] = Napi::Number::New(env,  (int32_t) *(int32_t *) offset); continue;
      case TypeInt64 : res[i] = Napi::Number::New(env,  (double)  *(Robot::int64 *) offset); continue;
      case TypeReal32: res[i] = Napi::Number::New(env,  (double)  *(float   *) offset); continue;
      case TypeReal64: res[i] = Napi::Number::New(env,  (double)  *(double  *) offset); continue;
      case TypeBool  : res[i] = Napi::Boolean::New(env, (bool)    *(bool    *) offset); continue;
      case TypeString: res[i] = Napi::String::New(env, std::string(offset, length).data()); continue;
      default: break;
    }
  }
  delete[] data;
  return res;
}

Napi::Value MemoryAdapter::readInt8(const Napi::CallbackInfo& info) { return readTypeImpl(info, TypeInt8, 1, 1); }
Napi::Value MemoryAdapter::readInt16(const Napi::CallbackInfo& info) { return readTypeImpl(info, TypeInt16, 2, 1); }
Napi::Value MemoryAdapter::readInt32(const Napi::CallbackInfo& info) { return readTypeImpl(info, TypeInt32, 4, 1); }
Napi::Value MemoryAdapter::readInt64(const Napi::CallbackInfo& info) { return readTypeImpl(info, TypeInt64, 8, 1); }
Napi::Value MemoryAdapter::readReal32(const Napi::CallbackInfo& info) { return readTypeImpl(info, TypeReal32, 4, 1); }
Napi::Value MemoryAdapter::readReal64(const Napi::CallbackInfo& info) { return readTypeImpl(info, TypeReal64, 8, 1); }
Napi::Value MemoryAdapter::readBool(const Napi::CallbackInfo& info) { return readTypeImpl(info, TypeBool, 1, 1); }
Napi::Value MemoryAdapter::readString(const Napi::CallbackInfo& info) {
  return readTypeImpl(info, TypeString, info[1].As<Napi::Number>().Uint32Value(), 2);
}
Napi::Value MemoryAdapter::readPtr(const Napi::CallbackInfo& info) {
  return adaptee.GetPtrSize() == 4
    ? readTypeImpl(info, TypeInt32, 4, 1)
    : readTypeImpl(info, TypeInt64, 8, 1);
}

Napi::Value MemoryAdapter::writeTypeImpl(const Napi::CallbackInfo& info, int type_, Robot::uint32 length) {
  auto address = (Robot::uintptr) info[0].As<Napi::Number>().DoubleValue();
  auto type    = (DataType) type_;

  if(type == TypeString) {
    if(!info[1].IsString()) throw Napi::TypeError::New(env, "Invalid arguments");
    auto str = info[1].As<Napi::String>().Utf8Value();
    if(length == 0) length = str.length() + 1;
    if(length == 0) return Napi::Boolean::New(env, true);
    if(length > (Robot::uint32)str.length() + 1)
      throw Napi::RangeError::New(env, "Length is too large");
    return Napi::Boolean::New(env, adaptee.WriteData(address, str.c_str(), length) == length);
  } else {
    Robot::int64 data = 0;
    switch(type) {
      case TypeInt8  : *(int8_t  *) &data = (int8_t)  info[1].As<Napi::Number>().Int32Value(); break;
      case TypeInt16 : *(int16_t *) &data = (int16_t) info[1].As<Napi::Number>().Int32Value(); break;
      case TypeInt32 : *(int32_t *) &data = (int32_t) info[1].As<Napi::Number>().Int32Value(); break;
      case TypeInt64 : *(Robot::int64 *) &data = (Robot::int64) info[1].As<Napi::Number>().DoubleValue(); break;
      case TypeReal32: *(float   *) &data = (float)   info[1].As<Napi::Number>().DoubleValue(); break;
      case TypeReal64: *(double  *) &data = (double)  info[1].As<Napi::Number>().DoubleValue(); break;
      case TypeBool  : *(bool    *) &data = (bool)    info[1].As<Napi::Boolean>().Value(); break;
      default: return Napi::Boolean::New(env, false);
    }
    return Napi::Boolean::New(env, adaptee.WriteData(address, &data, length) == length);
  }
}

Napi::Value MemoryAdapter::writeInt8(const Napi::CallbackInfo& info) { return writeTypeImpl(info, TypeInt8, 1); }
Napi::Value MemoryAdapter::writeInt16(const Napi::CallbackInfo& info) { return writeTypeImpl(info, TypeInt16, 2); }
Napi::Value MemoryAdapter::writeInt32(const Napi::CallbackInfo& info) { return writeTypeImpl(info, TypeInt32, 4); }
Napi::Value MemoryAdapter::writeInt64(const Napi::CallbackInfo& info) { return writeTypeImpl(info, TypeInt64, 8); }
Napi::Value MemoryAdapter::writeReal32(const Napi::CallbackInfo& info) { return writeTypeImpl(info, TypeReal32, 4); }
Napi::Value MemoryAdapter::writeReal64(const Napi::CallbackInfo& info) { return writeTypeImpl(info, TypeReal64, 8); }
Napi::Value MemoryAdapter::writeBool(const Napi::CallbackInfo& info) { return writeTypeImpl(info, TypeBool, 1); }
Napi::Value MemoryAdapter::writeString(const Napi::CallbackInfo& info) {
  return writeTypeImpl(info, TypeString, info[2].IsUndefined() ? 0 : info[2].As<Napi::Number>().Uint32Value());
}
Napi::Value MemoryAdapter::writePtr(const Napi::CallbackInfo& info) {
  return adaptee.GetPtrSize() == 4
    ? writeTypeImpl(info, TypeInt32, 4)
    : writeTypeImpl(info, TypeInt64, 8);
}

Napi::Function MemoryAdapter::RegionAdapter::Init(Napi::Env env) {
  return DefineClass(env, "Region", {
    InstanceAccessor("valid", &MemoryAdapter::RegionAdapter::valid, &MemoryAdapter::RegionAdapter::valid),
    InstanceAccessor("bound", &MemoryAdapter::RegionAdapter::bound, &MemoryAdapter::RegionAdapter::bound),
    InstanceAccessor("start", &MemoryAdapter::RegionAdapter::start, &MemoryAdapter::RegionAdapter::start),
    InstanceAccessor("stop", &MemoryAdapter::RegionAdapter::stop, &MemoryAdapter::RegionAdapter::stop),
    InstanceAccessor("size", &MemoryAdapter::RegionAdapter::size, &MemoryAdapter::RegionAdapter::size),
    InstanceAccessor("readable", &MemoryAdapter::RegionAdapter::readable, &MemoryAdapter::RegionAdapter::readable),
    InstanceAccessor("writable", &MemoryAdapter::RegionAdapter::writable, &MemoryAdapter::RegionAdapter::writable),
    InstanceAccessor("executable", &MemoryAdapter::RegionAdapter::executable, &MemoryAdapter::RegionAdapter::executable),
    InstanceAccessor("access", &MemoryAdapter::RegionAdapter::access, &MemoryAdapter::RegionAdapter::access),
    InstanceAccessor("private", &MemoryAdapter::RegionAdapter::private_, &MemoryAdapter::RegionAdapter::private_),
    InstanceAccessor("guarded", &MemoryAdapter::RegionAdapter::guarded, &MemoryAdapter::RegionAdapter::guarded),
    InstanceMethod("contains", &MemoryAdapter::RegionAdapter::contains),
    InstanceMethod("lt", &MemoryAdapter::RegionAdapter::lt),
    InstanceMethod("gt", &MemoryAdapter::RegionAdapter::gt),
    InstanceMethod("le", &MemoryAdapter::RegionAdapter::le),
    InstanceMethod("ge", &MemoryAdapter::RegionAdapter::ge),
    InstanceMethod("eq", &MemoryAdapter::RegionAdapter::eq),
    InstanceMethod("ne", &MemoryAdapter::RegionAdapter::ne),
  });
}

MemoryAdapter::RegionAdapter::RegionAdapter(const Napi::CallbackInfo& info) : ClassAdapter(info) {
  if(WrapAdaptee(info) || ConstructDefault(info) || CopyThat(info)) return;
  throw Napi::TypeError::New(env, "Invalid arguments");
}

Napi::Value MemoryAdapter::RegionAdapter::valid(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.Valid);
}
void MemoryAdapter::RegionAdapter::valid(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.Valid = value.ToBoolean();
}

Napi::Value MemoryAdapter::RegionAdapter::bound(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.Bound);
}
void MemoryAdapter::RegionAdapter::bound(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.Bound = value.ToBoolean();
}

Napi::Value MemoryAdapter::RegionAdapter::start(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.Start);
}
void MemoryAdapter::RegionAdapter::start(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.Start = value.As<Napi::Number>().Int64Value();
}

Napi::Value MemoryAdapter::RegionAdapter::stop(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.Stop);
}
void MemoryAdapter::RegionAdapter::stop(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.Stop = value.As<Napi::Number>().Int64Value();
}

Napi::Value MemoryAdapter::RegionAdapter::size(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.Size);
}
void MemoryAdapter::RegionAdapter::size(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.Size = value.As<Napi::Number>().Int64Value();
}

Napi::Value MemoryAdapter::RegionAdapter::readable(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.Readable);
}
void MemoryAdapter::RegionAdapter::readable(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.Readable = value.ToBoolean();
}

Napi::Value MemoryAdapter::RegionAdapter::writable(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.Writable);
}
void MemoryAdapter::RegionAdapter::writable(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.Writable = value.ToBoolean();
}

Napi::Value MemoryAdapter::RegionAdapter::executable(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.Executable);
}
void MemoryAdapter::RegionAdapter::executable(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.Executable = value.ToBoolean();
}

Napi::Value MemoryAdapter::RegionAdapter::access(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.Access);
}
void MemoryAdapter::RegionAdapter::access(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.Access = value.As<Napi::Number>();
}

Napi::Value MemoryAdapter::RegionAdapter::private_(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.Private);
}
void MemoryAdapter::RegionAdapter::private_(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.Private = value.ToBoolean();
}

Napi::Value MemoryAdapter::RegionAdapter::guarded(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.Guarded);
}
void MemoryAdapter::RegionAdapter::guarded(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.Guarded = value.ToBoolean();
}

Napi::Value MemoryAdapter::RegionAdapter::contains(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.Contains(info[0].As<Napi::Number>().Int64Value()));
}

Napi::Function MemoryAdapter::StatsAdapter::Init(Napi::Env env) {
  return DefineClass(env, "Stats", {
    InstanceAccessor("systemReads", &MemoryAdapter::StatsAdapter::systemReads, &MemoryAdapter::StatsAdapter::systemReads),
    InstanceAccessor("cachedReads", &MemoryAdapter::StatsAdapter::cachedReads, &MemoryAdapter::StatsAdapter::cachedReads),
    InstanceAccessor("systemWrites", &MemoryAdapter::StatsAdapter::systemWrites, &MemoryAdapter::StatsAdapter::systemWrites),
    InstanceAccessor("accessWrites", &MemoryAdapter::StatsAdapter::accessWrites, &MemoryAdapter::StatsAdapter::accessWrites),
    InstanceAccessor("readErrors", &MemoryAdapter::StatsAdapter::readErrors, &MemoryAdapter::StatsAdapter::readErrors),
    InstanceAccessor("writeErrors", &MemoryAdapter::StatsAdapter::writeErrors, &MemoryAdapter::StatsAdapter::writeErrors),
    InstanceMethod("eq", &MemoryAdapter::StatsAdapter::eq),
    InstanceMethod("ne", &MemoryAdapter::StatsAdapter::ne),
  });
}

MemoryAdapter::StatsAdapter::StatsAdapter(const Napi::CallbackInfo& info) : ClassAdapter(info) {
  if(WrapAdaptee(info) || ConstructDefault(info) || CopyThat(info)) return;
  throw Napi::TypeError::New(env, "Invalid arguments");
}

Napi::Value MemoryAdapter::StatsAdapter::systemReads(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.SystemReads);
}
void MemoryAdapter::StatsAdapter::systemReads(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.SystemReads = value.As<Napi::Number>();
}

Napi::Value MemoryAdapter::StatsAdapter::cachedReads(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.CachedReads);
}
void MemoryAdapter::StatsAdapter::cachedReads(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.CachedReads = value.As<Napi::Number>();
}

Napi::Value MemoryAdapter::StatsAdapter::systemWrites(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.SystemWrites);
}
void MemoryAdapter::StatsAdapter::systemWrites(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.SystemWrites = value.As<Napi::Number>();
}

Napi::Value MemoryAdapter::StatsAdapter::accessWrites(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.AccessWrites);
}
void MemoryAdapter::StatsAdapter::accessWrites(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.AccessWrites = value.As<Napi::Number>();
}

Napi::Value MemoryAdapter::StatsAdapter::readErrors(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.ReadErrors);
}
void MemoryAdapter::StatsAdapter::readErrors(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.ReadErrors = value.As<Napi::Number>();
}

Napi::Value MemoryAdapter::StatsAdapter::writeErrors(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.WriteErrors);
}
void MemoryAdapter::StatsAdapter::writeErrors(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.WriteErrors = value.As<Napi::Number>();
}

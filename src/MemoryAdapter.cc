#include "MemoryAdapter.h"
#include "RangeAdapter.h"
#include "ProcessAdapter.h"

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
    InstanceMethod("find", &MemoryAdapter::find),
    InstanceMethod("createCache", &MemoryAdapter::createCache),
    InstanceMethod("clearCache", &MemoryAdapter::clearCache),
    InstanceMethod("deleteCache", &MemoryAdapter::deleteCache),
    InstanceMethod("isCaching", &MemoryAdapter::isCaching),
    InstanceMethod("getCacheSize", &MemoryAdapter::getCacheSize),
    InstanceMethod("readData", &MemoryAdapter::readData),
    InstanceMethod("writeData", &MemoryAdapter::writeData),
  });
}

MemoryAdapter::MemoryAdapter(const Napi::CallbackInfo& info) : ClassAdapter(info) {
  if(WrapAdaptee(info, adaptee)) return;
  if(IsInstance(info[0])) {
    adaptee = Unwrap(info[0])->adaptee;
    return;
  }
  if(info[0].IsUndefined()) {
    adaptee = Robot::Memory();
    return;
  }
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
  return MemoryAdapter::StatsAdapter::New(env, adaptee.GetStats(info[0].IsBoolean() ? info[0].As<Napi::Boolean>() : false));
}

Napi::Value MemoryAdapter::getRegion(const Napi::CallbackInfo& info) {
  return MemoryAdapter::RegionAdapter::New(env, adaptee.GetRegion(info[0].ToNumber().Int64Value()));
}

Napi::Value MemoryAdapter::getRegions(const Napi::CallbackInfo& info) {
  auto regions = adaptee.GetRegions(
    info[0].IsUndefined() ? 0 : info[0].ToNumber(),
    info[1].IsUndefined() ? -1 : info[1].ToNumber()
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
    info[1].IsUndefined() ? 0 : info[1].ToNumber(),
    info[2].IsUndefined() ? -1 : info[2].ToNumber(),
    info[3].IsUndefined() ? 0 : info[3].ToNumber(),
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
    info[0].ToNumber().Int64Value(),
    info[1].ToNumber().Int64Value(),
    info[2].ToNumber().Int64Value(),
    info[3].IsUndefined() ? 0 : info[3].ToNumber().Int64Value(),
    info[4].IsUndefined() ? 0 : info[4].ToNumber().Int64Value()
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
  uint64_t length = info[0].ToNumber().Int64Value();
  if(buffer.Length() < length) {
    throw Napi::RangeError::New(env, "Buffer is too small");
  }
  return Napi::Number::New(env, adaptee.ReadData(
    info[0].ToNumber().Int64Value(),
    buffer.Data(),
    length,
    info[3].IsUndefined() ? Robot::Memory::Default : (Robot::Memory::Flags)(info[3].ToNumber().Int32Value())
  ));
}

Napi::Value MemoryAdapter::writeData(const Napi::CallbackInfo& info) {
  return env.Null(); // FIXME
}

Napi::Value MemoryAdapter::readInt8(const Napi::CallbackInfo& info) {
  return env.Null(); // FIXME
}

Napi::Value MemoryAdapter::readInt16(const Napi::CallbackInfo& info) {
  return env.Null(); // FIXME
}

Napi::Value MemoryAdapter::readInt32(const Napi::CallbackInfo& info) {
  return env.Null(); // FIXME
}

Napi::Value MemoryAdapter::readInt64(const Napi::CallbackInfo& info) {
  return env.Null(); // FIXME
}

Napi::Value MemoryAdapter::readReal32(const Napi::CallbackInfo& info) {
  return env.Null(); // FIXME
}

Napi::Value MemoryAdapter::readReal64(const Napi::CallbackInfo& info) {
  return env.Null(); // FIXME
}

Napi::Value MemoryAdapter::readPtr(const Napi::CallbackInfo& info) {
  return env.Null(); // FIXME
}

Napi::Value MemoryAdapter::readBool(const Napi::CallbackInfo& info) {
  return env.Null(); // FIXME
}

Napi::Value MemoryAdapter::readString(const Napi::CallbackInfo& info) {
  return env.Null(); // FIXME
}

Napi::Value MemoryAdapter::writeInt8(const Napi::CallbackInfo& info) {
  return env.Null(); // FIXME
}

Napi::Value MemoryAdapter::writeInt16(const Napi::CallbackInfo& info) {
  return env.Null(); // FIXME
}

Napi::Value MemoryAdapter::writeInt32(const Napi::CallbackInfo& info) {
  return env.Null(); // FIXME
}

Napi::Value MemoryAdapter::writeInt64(const Napi::CallbackInfo& info) {
  return env.Null(); // FIXME
}

Napi::Value MemoryAdapter::writeReal32(const Napi::CallbackInfo& info) {
  return env.Null(); // FIXME
}

Napi::Value MemoryAdapter::writeReal64(const Napi::CallbackInfo& info) {
  return env.Null(); // FIXME
}

Napi::Value MemoryAdapter::writePtr(const Napi::CallbackInfo& info) {
  return env.Null(); // FIXME
}

Napi::Value MemoryAdapter::writeBool(const Napi::CallbackInfo& info) {
  return env.Null(); // FIXME
}

Napi::Value MemoryAdapter::writeString(const Napi::CallbackInfo& info) {
  return env.Null(); // FIXME
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
    InstanceAccessor("private_", &MemoryAdapter::RegionAdapter::private_, &MemoryAdapter::RegionAdapter::private_),
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

MemoryAdapter::RegionAdapter::RegionAdapter(const Napi::CallbackInfo& info) : ClassAdapterCmp(info) {
  if(WrapAdaptee(info, adaptee)) return;
  if(IsInstance(info[0])) {
    adaptee = Unwrap(info[0])->adaptee;
    return;
  }
  if(info[0].IsUndefined()) {
    adaptee = Robot::Memory::Region();
    return;
  }
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
  return Napi::Boolean::New(env, adaptee.Start);
}
void MemoryAdapter::RegionAdapter::start(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.Start = value.ToBoolean();
}

Napi::Value MemoryAdapter::RegionAdapter::stop(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.Stop);
}
void MemoryAdapter::RegionAdapter::stop(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.Stop = value.ToBoolean();
}

Napi::Value MemoryAdapter::RegionAdapter::size(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.Size);
}
void MemoryAdapter::RegionAdapter::size(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.Size = value.ToBoolean();
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
  adaptee.Access = value.ToNumber();
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
  return Napi::Boolean::New(env, adaptee.Contains(info[0].ToNumber().Int64Value()));
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

MemoryAdapter::StatsAdapter::StatsAdapter(const Napi::CallbackInfo& info) : ClassAdapterEq(info) {
  if(WrapAdaptee(info, adaptee)) return;
  if(IsInstance(info[0])) {
    adaptee = Unwrap(info[0])->adaptee;
    return;
  }
  if(info[0].IsUndefined()) {
    adaptee = Robot::Memory::Stats();
    return;
  }
  throw Napi::TypeError::New(env, "Invalid arguments");
}

Napi::Value MemoryAdapter::StatsAdapter::systemReads(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.SystemReads);
}
void MemoryAdapter::StatsAdapter::systemReads(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.SystemReads = value.ToNumber();
}

Napi::Value MemoryAdapter::StatsAdapter::cachedReads(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.CachedReads);
}
void MemoryAdapter::StatsAdapter::cachedReads(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.CachedReads = value.ToNumber();
}

Napi::Value MemoryAdapter::StatsAdapter::systemWrites(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.SystemWrites);
}
void MemoryAdapter::StatsAdapter::systemWrites(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.SystemWrites = value.ToNumber();
}

Napi::Value MemoryAdapter::StatsAdapter::accessWrites(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.AccessWrites);
}
void MemoryAdapter::StatsAdapter::accessWrites(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.AccessWrites = value.ToNumber();
}

Napi::Value MemoryAdapter::StatsAdapter::readErrors(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.ReadErrors);
}
void MemoryAdapter::StatsAdapter::readErrors(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.ReadErrors = value.ToNumber();
}

Napi::Value MemoryAdapter::StatsAdapter::writeErrors(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.WriteErrors);
}
void MemoryAdapter::StatsAdapter::writeErrors(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.WriteErrors = value.ToNumber();
}

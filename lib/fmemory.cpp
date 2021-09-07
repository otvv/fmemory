// fmemory - simple memory hacking library for linux
//
// by: otvv
// license: MIT
//

#include "fmemory.hpp"

using namespace FMEMORY::UTILITIES;

FMEMORY::MANAGER mngrMemory;

namespace FMEMORY
{
  //-------------------------------------------------------------------//
  std::int32_t MANAGER::GetProcessID(const std::string &_process_name)
  {
    if (_process_name.empty())
    {
      return 0;
    }

    for (const auto &process : std::filesystem::directory_iterator("/proc/"))
    {
      // check if the proc folder is valid
      if (!process.is_directory())
      {
        continue;
      }

      if (!IsOnlyDigits(process.path().string().erase(0, 6)))
      {
        continue;
      }

      std::string strLine;
      std::ifstream ccProcessName(process.path() / "comm");

      if (ccProcessName.is_open())
      {
        std::getline(ccProcessName, strLine);

        // if the process is equal to the process name
        if (strLine == _process_name)
        {
          m_iProcessID = std::stoi(process.path().string().erase(0, 6));

          // return process id
          return m_iProcessID;
        }
      }
    }

    return 0;
  }
  //-------------------------------------------------------------------//
  std::uintptr_t MANAGER::GetModule(const std::string &_module_name, std::int32_t _process_id)
  {
    FILE *fileMaps;
    std::uintptr_t ulResult;
    char cCommand[MIN_BUFFER_SIZE];

    // run command
    std::snprintf(cCommand, 256, "grep \"%s\" /proc/%i/maps | head -n 1 | cut -d \"-\" -f1", _module_name.c_str(), _process_id);

    // create new stream with our custom command
    fileMaps = popen(cCommand, "r");

    if (fileMaps)
    {
      // scan for modules
      if (fscanf(fileMaps, "%" SCNx64, &ulResult))
      {
        // return module address
        return ulResult;
      }
      else
      {
        return 0;
      }
    }

    // close pipe
    pclose(fileMaps);

    return 0;
  }
  //-------------------------------------------------------------------//
  bool MANAGER::ReadProcessMemory(void *_address, void *_buffer, std::size_t _size)
  {
    struct iovec local_iovec[1];
    struct iovec remote_iovec[1];

    local_iovec[0].iov_base = _buffer;
    local_iovec[0].iov_len = sizeof(_size);
    remote_iovec[0].iov_base = _address;
    remote_iovec[0].iov_len = sizeof(_size);

    return (process_vm_readv(m_iProcessID, local_iovec, 1, remote_iovec, 1, 0) == static_cast<ssize_t>(_size));
  }
  //-------------------------------------------------------------------//
  char MANAGER::ReadCharProcessMemory(void *_address)
  {
    struct iovec local_iovec[1];
    struct iovec remote_iovec[1];

    char cBuffer;

    local_iovec[0].iov_base = &cBuffer;
    local_iovec[0].iov_len = sizeof(char);
    remote_iovec[0].iov_base = _address;
    remote_iovec[0].iov_len = sizeof(char);

    process_vm_readv(m_iProcessID, local_iovec, 1, remote_iovec, 1, 0) == static_cast<ssize_t>(cBuffer);

    return cBuffer;
  }

  //-------------------------------------------------------------------//
  bool MANAGER::WriteProcessMemory(void *_address, void *_buffer, std::size_t _size)
  {
    struct iovec local_iovec[1];
    struct iovec remote_iovec[1];

    local_iovec[0].iov_base = _buffer;
    local_iovec[0].iov_len = sizeof(_size);
    remote_iovec[0].iov_base = _address;
    remote_iovec[0].iov_len = sizeof(_size);

    return (process_vm_writev(m_iProcessID, local_iovec, 1, remote_iovec, 1, 0) == static_cast<ssize_t>(_size));
  }
  //-------------------------------------------------------------------//
  std::uintptr_t MANAGER::GetCallAddress(std::uintptr_t _address)
  {
    std::size_t ulCode = 0;

    if (MANAGER::ReadProcessMemory(reinterpret_cast<char *>(_address + 0x1), &ulCode, sizeof(std::size_t)))
    {
      return ulCode + (_address + 0x5);
    }

    return 0;
  }
  //-------------------------------------------------------------------//
  std::uintptr_t MANAGER::GetAbsoluteAddress(std::uintptr_t _address, std::size_t _offset, std::int32_t _size)
  {
    std::size_t ulCode = 0;

    if (MANAGER::ReadProcessMemory(reinterpret_cast<char *>(_address + _offset), &ulCode, sizeof(std::size_t)))
    {
      return ulCode + (_address + _size);
    }

    return 0;
  }
} // namespace FMEMORY

Napi::Number getProcessID(const Napi::CallbackInfo &_info)
{
  // environment
  Napi::Env env = _info.Env();

  // check if the argument is valid
  if (_info.Length() < 1 || !_info[0].IsString())
  {
    Napi::TypeError::New(env, "Process name expected - (String)").ThrowAsJavaScriptException();
  }

  // process name
  Napi::String processName = _info[0].As<Napi::String>();

  // get pid of given process
  Napi::Number returnValue = Napi::Number::New(env, mngrMemory.GetProcessID(processName.Utf8Value()));

  return returnValue;
}
//-------------------------------------------------------------------//
Napi::Number getModuleBaseAddress(const Napi::CallbackInfo &_info)
{
  // environment
  Napi::Env env = _info.Env();

  // check if the argument is valid
  if (_info.Length() < 1 || !_info[0].IsString())
  {
    Napi::TypeError::New(env, "Module name expected - (String)").ThrowAsJavaScriptException();
  }
  else if (_info.Length() < 1 || !_info[1].IsNumber())
  {
    Napi::TypeError::New(env, "Process id expected - (Number)").ThrowAsJavaScriptException();
  }

  // module name
  Napi::String moduleName = _info[0].As<Napi::String>();

  // process id
  Napi::Number pID = _info[1].As<Napi::Number>();

  // get module address
  Napi::Number returnValue = Napi::Number::New(env, mngrMemory.GetModule(moduleName.Utf8Value(), pID.Int32Value()));

  return returnValue;
}
//-------------------------------------------------------------------//
Napi::Value readMemory(const Napi::CallbackInfo &_info)
{
  // environment
  Napi::Env env = _info.Env();

  // check if the argument is valid
  if (_info.Length() < 1 || !_info[0].IsNumber())
  {
    Napi::TypeError::New(env, "Base module address expected - (Number)").ThrowAsJavaScriptException();
  }
  else if (_info.Length() < 1 || !_info[1].IsString())
  {
    Napi::TypeError::New(env, "Data type expected - (String)").ThrowAsJavaScriptException();
  }

  // get base address
  Napi::Number baseAddress = _info[0].As<Napi::Number>();

  // get data type
  Napi::String dataType = _info[1].As<Napi::String>();

  // dummy
  Napi::Value returnValue = {};

  if (dataType.Utf8Value().compare("int") == 0)
  {
    int dummy = 0;
    mngrMemory.ReadProcessMemory(reinterpret_cast<void *>(baseAddress.Int64Value()), &dummy, sizeof(dummy));
    return Napi::Number::New(env, dummy);
  }

  else if (dataType.Utf8Value().compare("uint") == 0)
  {
    unsigned int dummy = 0;
    mngrMemory.ReadProcessMemory(reinterpret_cast<void *>(baseAddress.Int64Value()), &dummy, sizeof(dummy));
    return Napi::Number::New(env, dummy);
  }

  else if (dataType.Utf8Value().compare("long") == 0)
  {
    long dummy = 0;
    mngrMemory.ReadProcessMemory(reinterpret_cast<void *>(baseAddress.Int64Value()), &dummy, sizeof(dummy));
    return Napi::Number::New(env, dummy);
  }

  else if (dataType.Utf8Value().compare("ulong") == 0)
  {
    unsigned long dummy = 0;
    mngrMemory.ReadProcessMemory(reinterpret_cast<void *>(baseAddress.Int64Value()), &dummy, sizeof(dummy));
    return Napi::Number::New(env, dummy);
  }

  else if (dataType.Utf8Value().compare("short") == 0)
  {
    short dummy = 0;
    mngrMemory.ReadProcessMemory(reinterpret_cast<void *>(baseAddress.Int64Value()), &dummy, sizeof(dummy));
    return Napi::Number::New(env, dummy);
  }

  else if (dataType.Utf8Value().compare("float") == 0)
  {
    float dummy = 0.0f;
    mngrMemory.ReadProcessMemory(reinterpret_cast<void *>(baseAddress.Int64Value()), &dummy, sizeof(dummy));
    return Napi::Number::New(env, dummy);
  }

  else if (dataType.Utf8Value().compare("double") == 0)
  {
    double dummy = 0.0;
    mngrMemory.ReadProcessMemory(reinterpret_cast<void *>(baseAddress.Int64Value()), &dummy, sizeof(dummy));
    return Napi::Number::New(env, dummy);
  }

  else if (dataType.Utf8Value().compare("byte") == 0)
  {
    std::uint8_t dummy = 0;
    mngrMemory.ReadProcessMemory(reinterpret_cast<void *>(baseAddress.Int64Value()), &dummy, sizeof(dummy));
    return Napi::Number::New(env, dummy);
  }

  else if (dataType.Utf8Value().compare("bool") == 0)
  {
    bool dummy = false;
    mngrMemory.ReadProcessMemory(reinterpret_cast<void *>(baseAddress.Int64Value()), &dummy, sizeof(dummy));
    return Napi::Number::New(env, dummy);
  }

  else if (dataType.Utf8Value().compare("string") == 0)
  {
    // store characters here
    std::vector<char> characters = {};

    // char offset (count)
    std::size_t offset = 0x0;
    while (true)
    {
      char buffer = mngrMemory.ReadCharProcessMemory((reinterpret_cast<char *>(baseAddress.Int64Value()) + offset));
      characters.push_back(buffer);

      // break at 1 million chars
      if (offset == (sizeof(char) * 1000000))
      {
        characters.clear();
        break;
      }

      // break at terminator (end of string)
      if (buffer == '\0')
      {
        break;
      }

      // go to next char
      offset += sizeof(char);
    }

    // if the string is null or invalid
    if (characters.size() == 0)
    {
      Napi::TypeError::New(env, "Invalid string!").ThrowAsJavaScriptException(); // TODO: add more verbose error
    }
    else // if the string is valid
    {
      // assemble string with found chars
      std::string found_string(characters.begin(), characters.end());

      return Napi::String::New(env, found_string.c_str());
    }
  }

  return returnValue;
}
//-------------------------------------------------------------------//
Napi::Function writeMemory(const Napi::CallbackInfo &_info)
{
  // environment
  Napi::Env env = _info.Env();

  // check if the argument is valid
  if (_info.Length() < 1 || !_info[0].IsNumber())
  {
    Napi::TypeError::New(env, "Base module address expected - (Number)").ThrowAsJavaScriptException();
  }
  else if (_info.Length() < 1 || !_info[1].IsNumber())
  {
    Napi::TypeError::New(env, "Value expected - (Number) or (String)").ThrowAsJavaScriptException();
  }
  else if (_info.Length() < 1 || !_info[2].IsString())
  {
    Napi::TypeError::New(env, "Data type expected - (String)").ThrowAsJavaScriptException();
  }

  // get base address
  Napi::Number baseAddress = _info[0].As<Napi::Number>();

  // get value
  Napi::Number valueToWrite = _info[1].As<Napi::Number>();

  // get data type
  Napi::String dataType = _info[2].As<Napi::String>();

  // dummy return value
  Napi::Function returnValue = {};

  if (dataType.Utf8Value().compare("int") == 0)
  {
    int dummy = valueToWrite.Int32Value();
    mngrMemory.WriteProcessMemory(reinterpret_cast<void *>(baseAddress.Int64Value()), &dummy, sizeof(dummy));
  }

  else if (dataType.Utf8Value().compare("uint") == 0)
  {
    unsigned int dummy = valueToWrite.Uint32Value();
    mngrMemory.WriteProcessMemory(reinterpret_cast<void *>(baseAddress.Int64Value()), &dummy, sizeof(dummy));
  }

  else if (dataType.Utf8Value().compare("long") == 0)
  {
    long dummy = valueToWrite.Int64Value();
    mngrMemory.WriteProcessMemory(reinterpret_cast<void *>(baseAddress.Int64Value()), &dummy, sizeof(dummy));
  }

  else if (dataType.Utf8Value().compare("ulong") == 0)
  {
    unsigned long dummy = valueToWrite.Int64Value();
    mngrMemory.WriteProcessMemory(reinterpret_cast<void *>(baseAddress.Int64Value()), &dummy, sizeof(dummy));
  }

  else if (dataType.Utf8Value().compare("short") == 0)
  {
    short dummy = valueToWrite.Int32Value();
    mngrMemory.WriteProcessMemory(reinterpret_cast<void *>(baseAddress.Int64Value()), &dummy, sizeof(dummy));
  }

  else if (dataType.Utf8Value().compare("float") == 0)
  {
    float dummy = valueToWrite.FloatValue();
    mngrMemory.WriteProcessMemory(reinterpret_cast<void *>(baseAddress.Int64Value()), &dummy, sizeof(dummy));
  }

  else if (dataType.Utf8Value().compare("double") == 0)
  {
    double dummy = valueToWrite.DoubleValue();
    mngrMemory.WriteProcessMemory(reinterpret_cast<void *>(baseAddress.Int64Value()), &dummy, sizeof(dummy));
  }

  else if (dataType.Utf8Value().compare("byte") == 0)
  {
    unsigned char dummy = valueToWrite.Uint32Value();
    mngrMemory.WriteProcessMemory(reinterpret_cast<void *>(baseAddress.Int64Value()), &dummy, sizeof(dummy));
  }

  else if (dataType.Utf8Value().compare("bool") == 0)
  {
    bool dummy = valueToWrite.Int32Value();
    mngrMemory.WriteProcessMemory(reinterpret_cast<void *>(baseAddress.Int64Value()), &dummy, sizeof(dummy));
  }

  else if (dataType.Utf8Value().compare("string") == 0)
  {
    Napi::String valueToWrite = _info[1].As<Napi::String>();

    std::string dummy = valueToWrite.Utf8Value();
    mngrMemory.WriteProcessMemory(reinterpret_cast<void *>(baseAddress.Int64Value()), reinterpret_cast<char *>(dummy.data()), sizeof(dummy));
  }

  return returnValue;
}
//-------------------------------------------------------------------//
Napi::Number getCallAddress(const Napi::CallbackInfo &_info)
{
  // environment
  Napi::Env env = _info.Env();

  // check if the argument is valid
  if (_info.Length() < 1 || !_info[0].IsNumber())
  {
    Napi::TypeError::New(env, "Address expected - (Number)").ThrowAsJavaScriptException();
  }

  // address to call
  Napi::Number addressToCall = _info[0].As<Napi::Number>();

  // get call address
  Napi::Number returnValue = Napi::Number::New(env, mngrMemory.GetCallAddress(addressToCall.Int64Value()));

  return returnValue;
}
//-------------------------------------------------------------------//
Napi::Number getAbsoluteAddress(const Napi::CallbackInfo &_info)
{
  // environment
  Napi::Env env = _info.Env();

  // check if the argument is valid
  if (_info.Length() < 1 || !_info[0].IsNumber())
  {
    Napi::TypeError::New(env, "Address expected - (Number)").ThrowAsJavaScriptException();
  }
  else if (_info.Length() < 1 || !_info[1].IsNumber())
  {
    Napi::TypeError::New(env, "Offset expected - (Number)").ThrowAsJavaScriptException();
  }

  else if (_info.Length() < 1 || !_info[2].IsNumber())
  {
    Napi::TypeError::New(env, "Size expected - (Number)").ThrowAsJavaScriptException();
  }

  // address to call
  Napi::Number addressToCall = _info[0].As<Napi::Number>();

  // address offset
  Napi::Number addressOffset = _info[1].As<Napi::Number>();

  // address size
  Napi::Number addressSize = _info[2].As<Napi::Number>();

  // get call address
  Napi::Number returnValue = Napi::Number::New(env, mngrMemory.GetAbsoluteAddress(addressToCall.Int64Value(), addressOffset.Int32Value(), addressSize.Int32Value()));

  return returnValue;
}
//-------------------------------------------------------------------//
Napi::Object Initialize(Napi::Env _env, Napi::Object _exports)
{
  _exports.Set("getProcessID", Napi::Function::New(_env, getProcessID));
  _exports.Set("getModuleBaseAddress", Napi::Function::New(_env, getModuleBaseAddress));
  _exports.Set("readMemory", Napi::Function::New(_env, readMemory));
  _exports.Set("writeMemory", Napi::Function::New(_env, writeMemory));
  _exports.Set("getCallAddress", Napi::Function::New(_env, getCallAddress));
  _exports.Set("getAbsoluteAddress", Napi::Function::New(_env, getAbsoluteAddress));

  return _exports;
}

NODE_API_MODULE(fmemoryJS, Initialize);
// fmemory - simple node add-on for reading and writing memory on Linux.
// powered by gmem
//
// by: otvv
// license: MIT
//

#define MAX_CHAR_SIZE 1000000

// gmem
#include "dependencies/gmem/gmem.hpp"

// node
#include <napi.h>

Napi::Number getProcessID(const Napi::CallbackInfo &_info)
{
  Napi::Env env = _info.Env();

  // check if the argument is valid
  if (_info.Length() < 1 || !_info[0].IsString())
  {
    Napi::TypeError::New(env, "process name expected - (arg type: string)").ThrowAsJavaScriptException();
  }

  // get the content of the first argument (process name)
  Napi::String processName = _info[0].As<Napi::String>();

  // get pid of given process
  Napi::Number returnValue = Napi::Number::New(env, gmem::process::get_pid(processName.Utf8Value()));

  return returnValue;
}
//-------------------------------------------------------------------//
Napi::Number getModuleBaseAddr(const Napi::CallbackInfo &_info)
{
  Napi::Env env = _info.Env();

  // check if passed arguments are valid
  if (_info.Length() < 1 || !_info[0].IsString())
  {
    Napi::TypeError::New(env, "module name expected - (arg type: string)").ThrowAsJavaScriptException();
  }
  else if (_info.Length() < 1 || !_info[1].IsNumber())
  {
    Napi::TypeError::New(env, "process id expected - (arg type: number)").ThrowAsJavaScriptException();
  }

  // get the content of the first argument (module name)
  Napi::String moduleName = _info[0].As<Napi::String>();

  // get the content of the second argument (process id)
  Napi::Number processID = _info[1].As<Napi::Number>();

  // get base address of given module
  Napi::Number returnValue = Napi::Number::New(env, gmem::process::get_base_addr(moduleName.Utf8Value(), processID.Int32Value()));

  return returnValue;
}
//-------------------------------------------------------------------//
Napi::Value readMemory(const Napi::CallbackInfo &_info)
{
  Napi::Env env = _info.Env();

  // check if the argument is valid
  if (_info.Length() < 1 || !_info[0].IsNumber())
  {
    Napi::TypeError::New(env, "address expected - (arg type: number)").ThrowAsJavaScriptException();
  }
  else if (_info.Length() < 1 || !_info[1].IsString())
  {
    Napi::TypeError::New(env, "data type expected - (arg type: string)").ThrowAsJavaScriptException();
  }

  // get the content of the first argument (address location to read)
  Napi::Number address = _info[0].As<Napi::Number>();

  // get the content of the second argument (data type)
  Napi::String dataType = _info[1].As<Napi::String>();

  Napi::Value returnValue = {};

  if (dataType.Utf8Value().compare("int32") == 0)
  {
    auto result = gmem::process::read_mem<std::int32_t>(address.Int64Value());
    return Napi::Number::New(env, result);
  }

  else if (dataType.Utf8Value().compare("int64") == 0)
  {
    auto result = gmem::process::read_mem<std::int64_t>(address.Int64Value());
    return Napi::Number::New(env, result);
  }

  else if (dataType.Utf8Value().compare("uint32") == 0)
  {
    auto result = gmem::process::read_mem<std::uint32_t>(address.Int64Value());
    return Napi::Number::New(env, result);
  }

  else if (dataType.Utf8Value().compare("uint64") == 0)
  {
    auto result = gmem::process::read_mem<std::uint64_t>(address.Int64Value());
    return Napi::Number::New(env, result);
  }

  else if (dataType.Utf8Value().compare("long") == 0)
  {
    auto result = gmem::process::read_mem<long>(address.Int64Value());
    return Napi::Number::New(env, result);
  }

  else if (dataType.Utf8Value().compare("ulong") == 0)
  {
    auto result = gmem::process::read_mem<unsigned long>(address.Int64Value());
    return Napi::Number::New(env, result);
  }

  else if (dataType.Utf8Value().compare("short") == 0)
  {
    auto result = gmem::process::read_mem<short>(address.Int64Value());
    return Napi::Number::New(env, result);
  }

  else if (dataType.Utf8Value().compare("ushort") == 0)
  {
    auto result = gmem::process::read_mem<unsigned short>(address.Int64Value());
    return Napi::Number::New(env, result);
  }

  else if (dataType.Utf8Value().compare("float") == 0)
  {
    auto result = gmem::process::read_mem<float>(address.Int64Value());
    return Napi::Number::New(env, result);
  }

  else if (dataType.Utf8Value().compare("double") == 0)
  {
    auto result = gmem::process::read_mem<double>(address.Int64Value());
    return Napi::Number::New(env, result);
  }

  else if (dataType.Utf8Value().compare("byte") == 0)
  {
    auto result = gmem::process::read_mem<unsigned char>(address.Int64Value());
    return Napi::Number::New(env, result);
  }

  else if (dataType.Utf8Value().compare("char") == 0)
  {
    auto result = gmem::process::read_mem<char>(address.Int64Value());
    return Napi::Number::New(env, result);
  }

  else if (dataType.Utf8Value().compare("uchar") == 0)
  {
    auto result = gmem::process::read_mem<unsigned char>(address.Int64Value());
    return Napi::Number::New(env, result);
  }

  else if (dataType.Utf8Value().compare("bool") == 0)
  {
    auto result = gmem::process::read_mem<bool>(address.Int64Value());
    return Napi::Boolean::New(env, result);
  }

  else if (dataType.Utf8Value().compare("pointer") == 0)
  {
    auto result = gmem::process::read_mem<std::uintptr_t>(address.Int64Value());
    return Napi::Number::New(env, result);
  }

  else if (dataType.Utf8Value().compare("string") == 0)
  {
    // store characters here
    std::vector<char> characters = {};

    // char offset (count)
    std::size_t offset = 0x0;
    for (;;)
    {
      auto buffer = gmem::process::read_mem<char>(address.Int64Value() + offset);
      characters.push_back(buffer);

      // break at 1 million chars
      if (offset == (sizeof(char) * MAX_CHAR_SIZE))
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

    if (characters.size())
    {
      // assemble a new string with found characters
      std::string assembled_string(characters.begin(), characters.end());

      return Napi::String::New(env, assembled_string.c_str());
    }
    else // if the string is invalid
    {
      Napi::TypeError::New(env, "invalid string!").ThrowAsJavaScriptException(); // TODO: add a more verbose error
    }
  }

  return returnValue;
}
//-------------------------------------------------------------------//
Napi::Function writeMemory(const Napi::CallbackInfo &_info)
{
  Napi::Env env = _info.Env();

  // check if the argument is valid
  if (_info.Length() < 1 || !_info[0].IsNumber())
  {
    Napi::TypeError::New(env, "address expected - (arg type: number)").ThrowAsJavaScriptException();
  }
  else if (_info.Length() < 1 || !_info[1].IsNumber())
  {
    Napi::TypeError::New(env, "value to write expected - (arg type: any)").ThrowAsJavaScriptException();
  }
  else if (_info.Length() < 1 || !_info[2].IsString())
  {
    Napi::TypeError::New(env, "data type expected - (arg type: string)").ThrowAsJavaScriptException();
  }

  // get base address
  Napi::Number address = _info[0].As<Napi::Number>();

  // get value
  Napi::Number valueToWrite = _info[1].As<Napi::Number>();

  // get boolean value
  Napi::Boolean booleanToWrite = _info[1].As<Napi::Boolean>();

  // get data type
  Napi::String dataType = _info[2].As<Napi::String>();

  Napi::Function returnValue = {};

  if (dataType.Utf8Value().compare("int32") == 0)
  {
    std::int32_t dummy = valueToWrite.Int32Value();
    gmem::process::write_mem<std::int32_t>(address.Int64Value(), dummy);
  }

  else if (dataType.Utf8Value().compare("int64") == 0)
  {
    std::int64_t dummy = valueToWrite.Int64Value();
    gmem::process::write_mem<std::int64_t>(address.Int64Value(), dummy);
  }

  else if (dataType.Utf8Value().compare("uint32") == 0)
  {
    std::uint32_t dummy = valueToWrite.Uint32Value();
    gmem::process::write_mem<std::uint32_t>(address.Int64Value(), dummy);
  }

  else if (dataType.Utf8Value().compare("uint64") == 0)
  {
    std::uint64_t dummy = valueToWrite.Int64Value();
    gmem::process::write_mem<std::uint64_t>(address.Int64Value(), dummy);
  }

  else if (dataType.Utf8Value().compare("long") == 0)
  {
    long dummy = valueToWrite.Int64Value();
    gmem::process::write_mem<long>(address.Int64Value(), dummy);
  }

  else if (dataType.Utf8Value().compare("ulong") == 0)
  {
    unsigned long dummy = valueToWrite.Int64Value();
    gmem::process::write_mem<unsigned long>(address.Int64Value(), dummy);
  }

  else if (dataType.Utf8Value().compare("short") == 0)
  {
    short dummy = valueToWrite.Int32Value();
    gmem::process::write_mem<short>(address.Int64Value(), dummy);
  }

  else if (dataType.Utf8Value().compare("ushort") == 0)
  {
    unsigned short dummy = valueToWrite.Uint32Value();
    gmem::process::write_mem<unsigned short>(address.Int64Value(), dummy);
  }

  else if (dataType.Utf8Value().compare("float") == 0)
  {
    float dummy = valueToWrite.FloatValue();
    gmem::process::write_mem<float>(address.Int64Value(), dummy);
  }

  else if (dataType.Utf8Value().compare("double") == 0)
  {
    double dummy = valueToWrite.DoubleValue();
    gmem::process::write_mem<double>(address.Int64Value(), dummy);
  }

  else if (dataType.Utf8Value().compare("byte") == 0)
  {
    unsigned char dummy = valueToWrite.Uint32Value();
    gmem::process::write_mem<unsigned char>(address.Int64Value(), dummy);
  }

  else if (dataType.Utf8Value().compare("char") == 0)
  {
    char dummy = valueToWrite.Uint32Value();
    gmem::process::write_mem<char>(address.Int64Value(), dummy);
  }

    else if (dataType.Utf8Value().compare("uchar") == 0)
  {
    unsigned char dummy = valueToWrite.Uint32Value();
    gmem::process::write_mem<unsigned char>(address.Int64Value(), dummy);
  }

  else if (dataType.Utf8Value().compare("bool") == 0)
  {
    bool dummy = booleanToWrite.Value();
    gmem::process::write_mem<bool>(address.Int64Value(), dummy);
  }

  else if (dataType.Utf8Value().compare("string") == 0)
  {
    Napi::String stringToWrite = _info[1].As<Napi::String>();
    std::string dummy = stringToWrite.Utf8Value();
    gmem::process::write_mem<const char*>(address.Int64Value(), dummy.c_str());
  }

  return returnValue;
}
//-------------------------------------------------------------------//
Napi::Object Initialize(Napi::Env _env, Napi::Object _exports)
{
  _exports.Set("getProcessID", Napi::Function::New(_env, getProcessID));
  _exports.Set("getModuleBaseAddr", Napi::Function::New(_env, getModuleBaseAddr));
  _exports.Set("readMemory", Napi::Function::New(_env, readMemory));
  _exports.Set("writeMemory", Napi::Function::New(_env, writeMemory));

  return _exports;
}

NODE_API_MODULE(fmemory, Initialize);

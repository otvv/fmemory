// fmemory - simple memory hacking library for linux
//
// by: otvv
// license: MIT
//

#ifndef FMEMORY_HH
#define FMEMORY_HH

#define MIN_BUFFER_SIZE 256
#define MAX_BUFFER_SIZE 4096
#define MAX_CHAR_SIZE 1000000

// std
#include <algorithm>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <stdint.h>
#include <utility>
#include <vector>

// linux
#include <sys/uio.h>
#include <inttypes.h>

// node
#include <napi.h>

namespace FMEMORY
{
  // @NOTE: everything inside this namespace is not meant
  // to be used outside the library.
  namespace UTILITIES
  {
    // @BRIEF: return true if the string is composed of digits
    inline bool IsOnlyDigits(const std::string &_string)
    {
      return std::all_of(_string.begin(), _string.end(), ::isdigit);
    }

    inline std::int32_t m_iProcessID;

  } // namespace UTILITIES

  using MANAGER = struct SMemoryManager_t
  {
    // @BRIEF: get the process id by name
    // @ARGS: std::string _process_name
    std::int32_t GetProcessID(const std::string &_process_name);

    // @BREIF: this will find a specific module on a specific process
    // @ARGS: std::string _module_name, std::int32_t _process_id
    std::uintptr_t GetModule(const std::string &_module_name, std::int32_t _process_id);

    // @BRIEF: read the value of specific memory location
    // @ARGS: void* _address, std::size_t _buffer, std::size_t _size
    bool ReadProcessMemory(void *_address, void *_buffer, std::size_t _size);

    // @BREIF: write to a specific memory location
    // @ARGS: void* _address, std::size_t _buffer, std::size_t _size
    bool WriteProcessMemory(void *_address, void *_buffer, std::size_t _size);

    // @BRIEF: read the char of the value of a specific memory location
    // @ARGS: void* _address
    char ReadCharProcessMemory(void *_address);

    // @BRIEF: get the call address of a function
    // @ARGS: std::uintptr_t _address
    std::uintptr_t GetCallAddressOffset(std::uintptr_t _address);

    // @BRIEF: get the absolute address of a function
    // @ARGS: std::uintptr_t _address, std::size_t _offset, std::int32_t _size
    std::uintptr_t GetAbsoluteAddress(std::uintptr_t _address, std::size_t _offset, std::int32_t _size);
  };

} // namespace FMEMORY

#endif // FMEMORY_HH

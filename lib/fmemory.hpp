// fmemory - simple memory hacking library for linux
//
// by: otvv
// license: MIT
//

#ifndef FMEMORY_HH
#define FMEMORY_HH

#define MIN_BUFFER_SIZE 256
#define MAX_BUFFER_SIZE 4096

#include <algorithm>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <stdint.h>
#include <utility>
#include <vector>

#include <sys/uio.h>
#include <inttypes.h>

#include <napi.h>

namespace FMEMORY
{

  struct SMemoryManager_t;
  struct SMapModuleMemoryRegion_t;

  // @warn: you are not meant to use the stuff that are inside this namespace outside the library
  namespace UTILITIES
  {

    inline bool IsOnlyDigits(const std::string &_string)
    {
      return std::all_of(_string.begin(), _string.end(), ::isdigit);
    }

    inline std::int32_t m_iProcessID;

  } // namespace UTILITIES

  using MEMORY_MAP = struct SMapModuleMemoryRegion_t
  {
    std::uintptr_t m_ulStart;
    std::uintptr_t m_ulEnd;

    bool m_bReadable;
    bool m_bWritable;
    bool m_bExecutable;
    bool m_bShared;

    std::uintptr_t m_ulOffset;
    std::uint8_t m_ucDeviceMajor;
    std::uint8_t m_ucDeviceMinor;
    std::uintptr_t m_ulNodeFileNumber;
    std::string m_strPathName;
    std::string m_strFileName;

    // @brief: find signature
    void *FindSignature(SMemoryManager_t _handle, const std::string &_data, const std::string &_pattern);
  };

  using MANAGER = struct SMemoryManager_t
  {
    // @brief: get the process id by name
    // @args: std::string _process_name
    std::int32_t GetProcessID(const std::string &_process_name);

    // @brief: this will find a specific module on a specific process
    // @args: std::string _module_name, std::int32_t _process_id
    std::uintptr_t GetModule(const std::string &_module_name, std::int32_t _process_id);

    // @brief: read the value of specific memory location
    // @args: std::uintptr_t _address, std::size_t _buffer, std::size_t _size
    bool ReadProcessMemory(void *_address, void *_buffer, std::size_t _size);

    // @brief: write to a specific memory location
    // @args: std::uintptr_t _address, std::size_t _buffer, std::size_t _size
    bool WriteProcessMemory(void *_address, void *_buffer, std::size_t _size);

    // @brief: get the address of a callable pattern (function/pointer)
    // @args: std::uintptr_t _address
    std::uintptr_t GetCallAddress(std::uintptr_t _address);

    // @brief: get the absolute address of a callable pattern (function/pointer)
    // @args: std::uintptr_t _address, std::size_t _offset, std::int32_t _size
    std::uintptr_t GetAbsoluteAddress(std::uintptr_t _address, std::size_t _offset, std::int32_t _size);

    // @brief: map the memory region of a specific process
    // @args: std::int32_t _process_id
    void MapProcessMemoryRegions(std::int32_t _process_id);

    // @brief: get info about the memory region of a process
    const std::vector<MEMORY_MAP> &GetMemoryInfo();

  private:
    std::vector<MEMORY_MAP> m_prgpMemoryMap;
  };

} // namespace FMEMORY

#endif // FMEMORY_HH
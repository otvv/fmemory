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

  using MEMORY_MAP = struct SMapModuleMemoryRegion_t
  {
  public:
    // @BRIEF: find signature on scanned module
    void *FindSignature(SMemoryManager_t _handle, const std::string &_data, const std::string &_pattern);

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
  };

  using MANAGER = struct SMemoryManager_t
  {
    // @BRIEF: get the process id by name
    // @ARGS: std::string _process_name
    std::int32_t GetProcessID(const std::string &_process_name);

    // @BREIF: this will find a specific module on a specific process
    // @ARGS: std::string _module_name, std::int32_t _process_id
    std::uintptr_t GetModule(const std::string &_module_name, std::int32_t _process_id);

    // @BRIEF: read the value of specific memory location
    // @ARGS: std::uintptr_t _address, std::size_t _buffer, std::size_t _size
    bool ReadProcessMemory(void *_address, void *_buffer, std::size_t _size);

    // @BREIF: write to a specific memory location
    // @ARGS: std::uintptr_t _address, std::size_t _buffer, std::size_t _size
    bool WriteProcessMemory(void *_address, void *_buffer, std::size_t _size);

    // @BRIEF: get the address of a callable pattern (function/pointer)
    // @ARGS: std::uintptr_t _address
    std::uintptr_t GetCallAddress(std::uintptr_t _address);

    // @BRIEF: get the absolute address of a callable pattern (function/pointer)
    // @ARGS: std::uintptr_t _address, std::size_t _offset, std::int32_t _size
    std::uintptr_t GetAbsoluteAddress(std::uintptr_t _address, std::size_t _offset, std::int32_t _size);

    // @BRIEF: map the memory region of a specific process
    // @ARGS: std::int32_t _process_id
    void MapProcessMemoryRegions(std::int32_t _process_id);

    // @BRIEF: get info about the memory region of a process
    const std::vector<MEMORY_MAP> &GetMemoryInfo();

  private:
    std::vector<MEMORY_MAP> m_prgpMemoryMap;
  };

} // namespace FMEMORY

#endif // FMEMORY_HH

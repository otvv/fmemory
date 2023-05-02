// gmem - simple memory reading/writing library for linux in C++
//
// by: otvv
// license: MIT
//

#ifndef GMEM_H
#define GMEM_H

#define MAX_BUFFER_SIZE 256

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

namespace gmem
{
  // TODO: maybe move this to internal namespace
  namespace proc_info
  {
    inline std::int32_t process_id;
    inline std::uintptr_t base_address;
  }

  namespace internal
  {
    inline bool composed_of_digits(const std::string &string)
    {
      return std::all_of(string.begin(), string.end(), ::isdigit);
    }
  } // namespace gmem::internal

  namespace process
  {
    std::int32_t get_pid(const std::string &process_name)
    {
      // in case the user provides an invalid process name, return an invalid id as well :)
      if (process_name.empty())
      {
        return -1;
      }

      // iterate process list so we can try to look for the process supplied by the user
      for (const auto &process : std::filesystem::directory_iterator("/proc/"))
      {
        // don't do anything in case the process list is invalid
        if (!process.is_directory() || !internal::composed_of_digits(process.path().string().erase(0, 6)))
        {
          continue;
        }

        // this will store the name of each process found in the 'proc' directory
        std::string name_process_found;

        // open the file that contains a list of process names
        std::ifstream process_name_list(process.path() / "comm");

        if (process_name_list.is_open())
        {
          // read file line by line
          std::getline(process_name_list, name_process_found);

          // if the process found on the list is the same as the one the user supplied
          if (name_process_found == process_name)
          {
            proc_info::process_id = std::stoi(process.path().string().erase(0, 6));
            return proc_info::process_id;
          }
        }
      }

      return -1;
    }

    //
    //

    std::uintptr_t get_base_addr(const std::string &module_name, std::int32_t pid)
    {
      FILE* file_maps;
      char cmd[MAX_BUFFER_SIZE];

      // create cmd string
      std::snprintf(cmd, MAX_BUFFER_SIZE, "grep \"%s\" /proc/%i/maps | head -n 1 | cut -d \"-\" -f1", module_name.c_str(), pid);

      // create stream with the command that was just generated
      file_maps = popen(cmd, "r");

      if (!file_maps)
      {
        // close stream
        pclose(file_maps);

        return 0;
      }

      // if module is found store its base address in a variable
      if (fscanf(file_maps, "%" SCNx64, &proc_info::base_address))
      {
        // close stream
        pclose(file_maps);

        return proc_info::base_address;
      }

      return 0;
    }

    //
    //

    template <typename T>
    T read_mem(std::uintptr_t address)
    {
      static T buffer;
      struct iovec local[1];
      struct iovec remote[1];

      local[0].iov_base = std::addressof(buffer);
      local[0].iov_len = sizeof(buffer);

      remote[0].iov_base = reinterpret_cast<void *>(address);
      remote[0].iov_len = sizeof(buffer);

      process_vm_readv(proc_info::process_id, local, 1, remote, 1, 0);

      return buffer;
    }

    //
    //

    template <typename T>
    void write_mem(std::uintptr_t address, T value)
    {
      struct iovec local[1];
      struct iovec remote[1];

      local[0].iov_base = &value;
      local[0].iov_len = sizeof(value);

      remote[0].iov_base = reinterpret_cast<void *>(address);
      remote[0].iov_len = sizeof(value);

      process_vm_writev(proc_info::process_id, local, 1, remote, 1, 0);
    }

  } // namespace gmem::process

} // namespace gmem

#endif // GMEM_H

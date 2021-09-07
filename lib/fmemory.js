const fmemory = require("../build/Release/fmemory");

module.exports = {

  // data type constants
  INT: "int",
  UINT: "uint",
  LONG: "long",
  ULONG: "ulong",
  SHORT: "short",
  FLOAT: "float",
  DOUBLE: "double",
  BYTE: "byte",
  BOOL: "bool",
  STRING: "string",

  getProcessID(processName) {

    if (arguments.length === 1) {
      return fmemory.getProcessID(processName);
    }

    throw "[fmemory] - missing arguments!"; // TODO: add a proper message here.
  },

  getModuleBaseAddress(moduleName, processID) {

    if (arguments.length === 2) {
      return fmemory.getModuleBaseAddress(moduleName, processID);
    }

    throw "[fmemory] - missing arguments!"; // TODO: add a proper message here.
  },

  readMemory(baseAddress, type) {

    if (arguments.length === 2) {
      return fmemory.readMemory(baseAddress, type);
    }

    throw "[fmemory] - missing arguments!"; // TODO: add a proper message here.
  },

  writeMemory(baseAddress, value, type) {

    if (arguments.length === 3) {
      return fmemory.writeMemory(baseAddress, value, type);
    }

    throw "[fmemory] - missing arguments!"; // TODO: add a proper message here.
  },

  getCallAddress(address) {

    if (arguments.length === 1) {
      return fmemory.getCallAddress(address);
    }

    throw "[fmemory] - missing arguments!"; // TODO: add a proper message here.
  },

  getAbsoluteAddress(address, offset, size) {

    if (arguments.length === 3) {
      return fmemory.getAbsoluteAddress(address, offset, size);
    }

    throw "[fmemory] - missing arguments!"; // TODO: add a proper message here.
  }
};
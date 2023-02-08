const _fmemory = require("../build/Release/fmemory");

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

  // methods
  getProcessID(processName) {

    if (arguments.length === 1) {
      return _fmemory._getProcessID(processName);
    }

    throw "[fmemory] - missing arguments!"; // TODO: add a proper message here.
  },
  getModuleBaseAddress(moduleName, processID) {

    if (arguments.length === 2) {
      return _fmemory._getModuleBaseAddress(moduleName, processID);
    }

    throw "[fmemory] - missing arguments!";
  },
  readMemory(baseAddress, type) {

    if (arguments.length === 2) {
      return _fmemory._readMemory(baseAddress, type);
    }

    throw "[fmemory] - missing arguments!";
  },
  writeMemory(baseAddress, value, type) {

    if ( arguments.length === 3 ) {
      return _fmemory._writeMemory(baseAddress, value, type);
    }

    throw "[fmemory] - missing arguments!";
  },
  getCallAddress(address) {

    if (arguments.length === 1) {
      return _fmemory._getCallAddress(address);
    }

    throw "[fmemory] - missing arguments!";
  },
  getAbsoluteAddress(address, offset, size) {

    if (arguments.length === 3) {
      return _fmemory._getAbsoluteAddress(address, offset, size);
    }

    throw "[fmemory] - missing arguments!";
  }
};
const _fmemory = require("../build/Release/fmemory");

module.exports = {

  // data type constants
  INT: "int32",
  INT64: "int64",
  UINT: "uint32",
  UINT64: "uint64",
  LONG: "long",
  ULONG: "ulong",
  SHORT: "short",
  USHORT: "ushort",
  FLOAT: "float",
  DOUBLE: "double",
  BYTE: "byte",
  CHAR: "char",
  UCHAR: "uchar",
  BOOL: "bool",
  POINTER: "pointer",
  STRING: "string",

  // methods
  getProcessID(processName) {
    try {
      if (arguments.length === 1) {
        return _fmemory.getProcessID(processName);
      }
    } catch (e) {
      throw new Error(`[fmemory] - ${e.message}`);
    }
  },
  getModuleBaseAddr(moduleName, processID) {
    try {
      if (arguments.length === 2) {
        return _fmemory.getModuleBaseAddr(moduleName, processID);
      }
    } catch (e) {
      throw new Error(`[fmemory] - ${e.message}`);
    }
  },
  getModuleBaseAddr(moduleName, processName) {
    try {
      if (arguments.length === 2) {
        return _fmemory.getModuleBaseAddr(moduleName, _fmemory.getProcessID(processName));
      }
    } catch (e) {
      throw new Error(`[fmemory] - ${e.message}`);
    }
  },
  readMemory(address, type) {
    try {
      if (arguments.length === 2) {
        return _fmemory.readMemory(address, type);
      }
    } catch (e) {
      throw new Error(`[fmemory] - ${e.message}`);
    }
  },
  writeMemory(address, value, type) {
    try {
      if ( arguments.length === 3 ) {
        return _fmemory.writeMemory(address, value, type);
      }
    } catch (e) {
      throw new Error(`[fmemory] - ${e.message}`);
    }
  },
};
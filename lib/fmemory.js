const _fmemory = require("../build/Release/fmemory");

module.exports = {

  // data type constants
  INT: 'int',
  UINT: 'uint',
  LONG: 'long',
  ULONG: 'ulong',
  SHORT: 'short',
  USHORT: 'ushort',
  FLOAT: 'float',
  DOUBLE: 'double',
  BYTE: 'byte',
  BOOL: 'bool',
  STRING: 'string',

  // methods
  getProcessID(processName) {
    try {
      if (arguments.length === 1) {
        return _fmemory.getProcessID(processName);
      }
    } catch (e) {
      throw new Error(`[fmemory] - ${e}`);
    }
  },
  getModuleBaseAddress(moduleName, processID) {
    try {
      if (arguments.length === 2) {
        return _fmemory.getModuleBaseAddress(moduleName, processID);
      }
    } catch (e) {
      throw new Error(`[fmemory] - ${e}`);
    }
  },
  readMemory(baseAddress, type) {
    try {
      if (arguments.length === 2) {
        return _fmemory.readMemory(baseAddress, type);
      }
    } catch (e) {
      throw new Error(`[fmemory] - ${e}`);
    }
  },
  writeMemory(baseAddress, value, type) {
    try {
      if ( arguments.length === 3 ) {
        return _fmemory.writeMemory(baseAddress, value, type);
      }
    } catch (e) {
      throw new Error(`[fmemory] - ${e}`);
    }
  },
  getCallAddressOffset(address) {
    try {
      if (arguments.length === 1) {
        return _fmemory.getCallAddressOffset(address);
      }
    } catch (e) {
      throw new Error(`[fmemory] - ${e}`);
    }
  },
  getAbsoluteAddress(address, offset, size) {
    try {
      if (arguments.length === 3) {
        return _fmemory.getAbsoluteAddress(address, offset, size);
      }
    } catch (e) {
      throw new Error(`[fmemory] - ${e}`);
    }
  }
};
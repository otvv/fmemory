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

  getProcessID(processName) {

    if (arguments === 1) {
      return fmemory.getProcessID(processName);
    }

    return fmemory.getProcessID(processName);
  },

  getModuleBaseAddress(moduleName, processID) {

    if (arguments.length === 2) {
      return fmemory.getModuleBaseAddress(moduleName, processID);
    }

    return fmemory.getModuleBaseAddress(moduleName, processID);
  },

  readMem(baseAddress, type) {

    if (arguments.length === 2) {
      return fmemory.readMem(baseAddress, type);
    }

    return fmemory.readMem(baseAddress, type);
  },

  writeMem(baseAddress, value, type) {

    if (arguments.length === 3) {
      return fmemory.writeMem(baseAddress, value, type);
    }

    return fmemory.writeMem(baseAddress, value, type);
  },

  findSignature(moduleName, processID, signaturePattern, signatureMask) {

    if (arguments.length === 4) {
      return fmemory.findSignature(moduleName, processID, signaturePattern, signatureMask);
    }

    return fmemory.findSignature(moduleName, processID, signaturePattern, signatureMask);
  },

  getCallAddress(address) {

    if (arguments.length === 1) {
      return fmemory.getCallAddress(address);
    }

    return fmemory.getCallAddress(address);
  },

  getAbsoluteAddress(address, offset, size) {

    if (arguments.length === 3) {
      return fmemory.getAbsoluteAddress(address, offset, size);
    }

    return fmemory.getAbsoluteAddress(address, offset, size);
  }
}
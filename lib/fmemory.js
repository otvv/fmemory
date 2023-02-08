import {
  getProcessID as _getProcessID,
  getModuleBaseAddress as _getModuleBaseAddress,
  readMemory as _readMemory, writeMemory as _writeMemory,
  getCallAddress as _getCallAddress,
  getAbsoluteAddress as _getAbsoluteAddress
} from "../build/Release/fmemory";

export const INT = "int";
export const UINT = "uint";
export const LONG = "long";
export const ULONG = "ulong";
export const SHORT = "short";
export const FLOAT = "float";
export const DOUBLE = "double";
export const BYTE = "byte";
export const BOOL = "bool";
export const STRING = "string";

export const getProcessID = (processName) => {

  if (arguments.length === 1) {
    return _getProcessID(processName);
  }

  throw "[fmemory] - missing arguments!"; // TODO: add a proper message here.
}
export const getModuleBaseAddress = (moduleName, processID) => {

  if (arguments.length === 2) {
    return _getModuleBaseAddress(moduleName, processID);
  }

  throw "[fmemory] - missing arguments!"; // TODO: add a proper message here.
}
export const readMemory = (baseAddress, type) => {

  if (arguments.length === 2) {
    return _readMemory(baseAddress, type);
  }

  throw "[fmemory] - missing arguments!"; // TODO: add a proper message here.
}
export const writeMemory = (baseAddress, value, type) => {

  if (arguments.length === 3) {
    return _writeMemory(baseAddress, value, type);
  }

  throw "[fmemory] - missing arguments!"; // TODO: add a proper message here.
}
export const getCallAddress = (address) => {

  if (arguments.length === 1) {
    return _getCallAddress(address);
  }

  throw "[fmemory] - missing arguments!"; // TODO: add a proper message here.
}
export const getAbsoluteAddress = (address, offset, size) => {

  if (arguments.length === 3) {
    return _getAbsoluteAddress(address, offset, size);
  }

  throw "[fmemory] - missing arguments!"; // TODO: add a proper message here.
}
const fmemory = require('../build/Release/fmemory');

module.exports = {

  // data type constants
  INT: 'int',
  UINT: 'uint',
  LONG: 'long',
  ULONG: 'ulong',
  SHORT: 'short',
  FLOAT: 'float',
  DOUBLE: 'double',
  BYTE: 'byte',
  BOOL: 'bool',

  getProcessID(process_name) {

    if (arguments === 1) {
      return fmemory.getProcessID(process_name)
    }

    return fmemory.getProcessID(process_name)
  },

  getModuleBaseAddress(module_name, process_id) {

    if (arguments.length === 2) {
      return fmemory.getModuleBaseAddress(module_name, process_id)
    }

    return fmemory.getModuleBaseAddress(module_name, process_id)
  },

  readMem(base_address, type) {

    if (arguments.length === 2) {
      return fmemory.readMem(base_address, type)
    }

    return fmemory.readMem(base_address, type)
  },

  writeMem(base_address, value, type) {

    if (arguments.length === 3) {
      return fmemory.writeMem(base_address, value, type)
    }

    return fmemory.writeMem(base_address, value, type)
  },

  findSignature(module_name, process_id, signature_pattern, signature_mask) {

    if (arguments.length === 4) {
      return fmemory.findSignature(module_name, process_id, signature_pattern, signature_mask)
    }

    return fmemory.findSignature(module_name, process_id, signature_pattern, signature_mask)
  },

  getCallAddress(address) {

    if (arguments.length === 1) {
      return fmemory.getCallAddress(address)
    }

    return fmemory.getCallAddress(address)
  },

  getAbsoluteAddress(address, offset, size) {

    if (arguments.length === 3) {
      return fmemory.getAbsoluteAddress(address, offset, size)
    }

    return fmemory.getAbsoluteAddress(address, offset, size)
  }
}
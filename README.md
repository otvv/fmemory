# fmemory [![codacy badge](https://app.codacy.com/project/badge/Grade/ce5ae034b28843f08eb76d7103787946 "codacy badge")](https://www.codacy.com/gh/otvv/fmemory/dashboard?utm_source=github.com&utm_medium=referral&utm_content=otvv/fmemory&utm_campaign=Badge_Grade)

A simple node add-on powered by [gmem](https://github.com/otvv/gmem) for reading/writing memory in a given process on GNU/Linux.

---

<details>
<summary>more info:</summary>
<h4>features:</h4>
    
    - get process id through process name
    - get module base address through module name
    - memory reading
    - memory writing
  
<h4>todo:</h4>
    
    - get call address of a function
    - vector3d and vector2d support _(read, write)_
    - shellcode execution _(maybe)_
    - high privileges helpers
    - signature reader/scanner

🚧 _more to come.._
</details>

<h4>installing:</h4>

Since this is a Node addon, you'll need `cmake-js`, `node-addon-api`, and a C++ compiler (`GCC` or `clang`).

To install this addon simply type: `npm install fmemory`

_if you get an error related to cmake-js, make sure that node.js is updated to the latest version and also make sure that `cmake-js` is installed globally and `node-addon-api` is installed as well. (It doesnt need to be installed globally)_

<h4>usage:</h4>

The usage is pretty straight-forward.

Here's how:

```javascript
const fmemory = require("fmemory");
// or import fmemory from "fmemory";

const localPlayerControllerAddress = 0xDEADBEEF;
const hPawn = 0x123;
const flFlashAlpha = 0x321;

const main = (() => {
  // get process id
  const processID = fmemory.getProcessID("cs2")

  // get engine module base address
  const clientModuleBaseAddress = fmemory.getModuleBaseAddr("libclient.so", processID)
  
  // another way to get the module base address
  // const clientModuleBaseAddress = fmemory.getModuleBaseAddrProcName("libclient.so", "cs2") 

  // local player pawn pointer
  const pLocalPLayer = fmemory.readMemory(localPlayerControllerAddress + hPawn, fmemory.POINTER)

  // disable flashbang
  if (pLocalPLayer) {
    fmemory.writeMemory(pLocalPLayer + flFlashAlpha, 0.0 /* <- value */, fmemory.FLOAT)
  }
})()
```

_NOTE_: This works with Wine/Proton as well.
_(fmemory still needs improvements on this area, but it works.)_

<h4>documentation:</h4>

When using `writeMemory` or `readMemory` functions, you will need to tell the data type parameter. 

It can either be a `string`:
```javascript
"int32", "uint32", "int64",
"uint64", "long", "ulong", 
"short", "float", "double", 
"byte", "char", "uchar",
"bool" "string", "pointer"
```

Or you can use one of the `constants` provided by the library:
```javascript
fmemory.INT, fmemory.UINT, fmemory.INT64,
fmemory.UINT64, fmemory.LONG, fmemory.ULONG, 
fmemory.SHORT, fmemory.FLOAT, fmemory.DOUBLE,
fmemory.BYTE, fmemory.CHAR, fmemory.UCHAR,
fmemory.BOOL, fmemory.STRING, fmemory.POINTER
```

---

**fmemory** is available under the [MIT License](https://github.com/otvv/fmemory/blob/master/LICENSE)

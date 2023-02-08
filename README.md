# fmemory [![Codacy Badge](https://app.codacy.com/project/badge/Grade/ce5ae034b28843f08eb76d7103787946)](https://www.codacy.com/gh/otvv/fmemory/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=otvv/fmemory&amp;utm_campaign=Badge_Grade)
A simple node add-on for reading and writing memory on GNU/Linux.

***

<details>
<summary>features:</summary>

-   memory reading
-   memory writing
-   get process id through process name
-   get module base address through module name
-   get call address
-   get the absolute location of an address (get absolute address)

**todo:**

-   vector3d and vector2d support _(read, write)_
-   ability to call virtual functions
-   shellcode execution _(maybe)_

:construction: _more to come.._

</details>

<h4>installing:</h4>

Since this is a Node addon, you'll need `cmake-js`, `node-addon-api`, and a C++ compiler (`GCC`).

To install this addon simply type: `npm install fmemory` (if you get an error related to cmake-js, make sure your node.js is updated and try installing `node-addon-api` and `cmake-js` globally first.)

<h4>usage:</h4>

The usage is pretty straight-forward.

Here's how:

```javascript
const fmemory = require("fmemory");

const clientStateAddress = 0xDEADBEEF
const deltaTickOffset = 0x123

(function main() {

  // get process id
  const processID = fmemory.getProcessID("csgo_linux64")
  
  // get engine module base address
  const engineModuleBaseAddress = fmemory.getModuleBaseAddress("engine_client.so", processID)
  
  // get client state address
  const pClientState = fmemory.readMemory(engineModuleBaseAddress + clientStateAddress, fmemory.INT)

  if (pClientState) {
    
    // force update
    fmemory.writeMemory(pClientState + deltaTickOffset, -1 /* <- value */, fmemory.BYTE)
  } 
  
})()
```
_NOTE_: This works with Wine/Proton as well.

<h4>documentation:</h4>

When using the `writeMem` or `readMem` you will need to tell the data type parameter, it can either be a `string`:

```javascript
"int", "uint", "long",
"ulong", "short", "float",
"double", "byte", "bool"
"string"
```
Or you can use one of the constants provided by the library:

```javascript
fmemory.INT, fmemory.UINT, fmemory.LONG,
fmemory.ULONG, fmemory.SHORT, fmemory.FLOAT,
fmemory.DOUBLE, fmemory.BYTE, fmemory.BOOL,
fmemory.STRING
```

***

**fmemory** is available under the [MIT License](https://github.com/otvv/fmemory/blob/master/LICENSE)

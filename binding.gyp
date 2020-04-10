{ 
  "targets": [ 
    { 
      "target_name": "fmemory", 
      "cflags_cc": [ "-std=c++17" ],         
      "sources": [ "lib/fmemory.cpp" ],
      'include_dirs': [ "<!@(node -p \"require('node-addon-api').include\")" ],
      'defines': [ "NAPI_DISABLE_CPP_EXCEPTIONS" ]
    }
  ]
}
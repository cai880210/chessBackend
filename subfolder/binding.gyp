{
  "targets": [
    {
      "target_name": "rileyChess",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-Wall -MMD -g -O2" ],
      "sources": [ "rileyChess.cc", "magicmoves.cc", "main.cc" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
    }
  ]
}

# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\SAPR_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\SAPR_autogen.dir\\ParseCache.txt"
  "SAPR_autogen"
  )
endif()

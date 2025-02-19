# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/loggerlib_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/loggerlib_autogen.dir/ParseCache.txt"
  "loggerlib_autogen"
  )
endif()

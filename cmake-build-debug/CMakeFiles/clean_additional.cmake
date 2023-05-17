# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/SeedRipper_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/SeedRipper_autogen.dir/ParseCache.txt"
  "SeedRipper_autogen"
  )
endif()

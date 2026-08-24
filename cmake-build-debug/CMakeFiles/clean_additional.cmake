# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\JournalAppTests_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\JournalAppTests_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\JournalApp_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\JournalApp_autogen.dir\\ParseCache.txt"
  "JournalAppTests_autogen"
  "JournalApp_autogen"
  )
endif()

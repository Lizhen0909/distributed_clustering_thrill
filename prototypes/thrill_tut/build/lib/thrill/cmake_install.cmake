# Install script for directory: /home/eagle/dev/thrill_tut/lib/thrill

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/eagle/dev/thrill_tut/build/lib/thrill/extlib/googletest/googletest/cmake_install.cmake")
  include("/home/eagle/dev/thrill_tut/build/lib/thrill/extlib/libs3/cmake_install.cmake")
  include("/home/eagle/dev/thrill_tut/build/lib/thrill/thrill/cmake_install.cmake")
  include("/home/eagle/dev/thrill_tut/build/lib/thrill/tests/cmake_install.cmake")
  include("/home/eagle/dev/thrill_tut/build/lib/thrill/benchmarks/cmake_install.cmake")
  include("/home/eagle/dev/thrill_tut/build/lib/thrill/examples/cmake_install.cmake")
  include("/home/eagle/dev/thrill_tut/build/lib/thrill/frontends/cmake_install.cmake")
  include("/home/eagle/dev/thrill_tut/build/lib/thrill/misc/cmake_install.cmake")

endif()


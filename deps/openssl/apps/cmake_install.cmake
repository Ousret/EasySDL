# Install script for directory: /Users/Ahmed/Documents/L2 SPI/C/Personnels/ESDL/deps/openssl/apps

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

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Runtime")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/Ahmed/Documents/L2 SPI/C/Personnels/ESDL/deps/openssl/apps/openssl")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/openssl" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/openssl")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/Ahmed/Documents/L2 SPI/C/Personnels/ESDL/deps/openssl/crypto/bin/libcrypto.dylib" "@executable_path/../lib/libcrypto.dylib"
      -change "/Users/Ahmed/Documents/L2 SPI/C/Personnels/ESDL/deps/openssl/ssl/bin/libssl.dylib" "@executable_path/../lib/libssl.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/openssl")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "$ORIGIN/../lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/openssl")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Data")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/openssl" TYPE FILE FILES "/Users/Ahmed/Documents/L2 SPI/C/Personnels/ESDL/deps/openssl/apps/CA.sh")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Data")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/openssl" TYPE FILE FILES "/Users/Ahmed/Documents/L2 SPI/C/Personnels/ESDL/deps/openssl/apps/CA.pl")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Data")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/openssl" TYPE FILE FILES "/Users/Ahmed/Documents/L2 SPI/C/Personnels/ESDL/deps/openssl/apps/tsget")
endif()


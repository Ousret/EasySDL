# Install script for directory: /Users/Ahmed/Documents/L2 SPI/C/Personnels/ESDL/deps/openssl

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

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Header")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/openssl" TYPE FILE FILES "/Users/Ahmed/Documents/L2 SPI/C/Personnels/ESDL/deps/openssl/e_os2.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Data")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/openssl" TYPE FILE FILES "/Users/Ahmed/Documents/L2 SPI/C/Personnels/ESDL/deps/openssl/tools/c_hash")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Data")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/openssl" TYPE FILE FILES "/Users/Ahmed/Documents/L2 SPI/C/Personnels/ESDL/deps/openssl/tools/c_info")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Data")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/openssl" TYPE FILE FILES "/Users/Ahmed/Documents/L2 SPI/C/Personnels/ESDL/deps/openssl/tools/c_issuer")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Data")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/openssl" TYPE FILE FILES "/Users/Ahmed/Documents/L2 SPI/C/Personnels/ESDL/deps/openssl/tools/c_name")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Other")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/openssl/etc" TYPE FILE FILES "/Users/Ahmed/Documents/L2 SPI/C/Personnels/ESDL/deps/openssl/tools/c_rehash")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Documentation")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/openssl/doc/" TYPE DIRECTORY FILES "/Users/Ahmed/Documents/L2 SPI/C/Personnels/ESDL/deps/openssl/doc/")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Data")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/openssl" TYPE FILE FILES "/Users/Ahmed/Documents/L2 SPI/C/Personnels/ESDL/deps/openssl/FAQ")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Data")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/openssl" TYPE FILE FILES "/Users/Ahmed/Documents/L2 SPI/C/Personnels/ESDL/deps/openssl/LICENSE")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Data")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/openssl" TYPE FILE FILES "/Users/Ahmed/Documents/L2 SPI/C/Personnels/ESDL/deps/openssl/PROBLEMS")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Data")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/openssl" TYPE FILE FILES "/Users/Ahmed/Documents/L2 SPI/C/Personnels/ESDL/deps/openssl/README")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Data")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/openssl" TYPE FILE FILES "/Users/Ahmed/Documents/L2 SPI/C/Personnels/ESDL/deps/openssl/README.ASN1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Data")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/openssl" TYPE FILE FILES "/Users/Ahmed/Documents/L2 SPI/C/Personnels/ESDL/deps/openssl/README.ENGINE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/Ahmed/Documents/L2 SPI/C/Personnels/ESDL/deps/openssl/crypto/cmake_install.cmake")

endif()


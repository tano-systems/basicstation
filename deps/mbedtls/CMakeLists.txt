# Download and unpack mbedtls at configure time
# See: http://crascit.com/2015/07/25/cmake-gtest/
configure_file(CMakeLists.txt.in mbedtls-download/CMakeLists.txt)
# Call CMake to download and mbedtls
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/mbedtls-download )
if(result)
  message(FATAL_ERROR "CMake step for mbedtls failed: ${result}")
endif()
# Build the downloaded mbedtls
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/mbedtls-download )
if(result)
  message(FATAL_ERROR "Build step for mbedtls failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
#set(mbedtls_force_shared_crt ON CACHE BOOL "" FORCE)
# Prevent installation of mbedtls with your project
#set(INSTALL_MBEDTLS OFF CACHE BOOL "" FORCE)

# Add mbedtls directly to our build.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/mbedtls-src
                 ${CMAKE_CURRENT_BINARY_DIR}/mbedtls-build)

set(MBEDTLS_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/mbedtls-src/include" CACHE INTERNAL "MBEDTLS_INCLUDE_DIR")
include_directories(${MBEDTLS_INCLUDE_DIR})
link_directories(${MBEDTLS_LIBRARIES})

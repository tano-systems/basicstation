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

# This is a bit of a hack that can be used to get gtest libraries to build in C++11 if you aren't using CMAKE_CXX_STANDARD
#
#set(CXX11_FEATURES
#    cxx_nullptr
#    cxx_auto_type
#    cxx_delegating_constructors
#)
#target_compile_features(gtest
#    PRIVATE
#        ${CXX11_FEATURES}
#)
#
#target_compile_features(gmock_main
#    PRIVATE
#        ${CXX11_FEATURES}
#)
#
#target_compile_features(gmock
#    PRIVATE
#        ${CXX11_FEATURES}
#)
#
#target_compile_features(gmock_main
#    PRIVATE
#        ${CXX11_FEATURES}
#)

# Add aliases for mbedtls libraries
#if(NOT TARGET mbedtls)
#    add_library(mbedtls ALIAS mbedtls)
#    add_library(mbedcrypto ALIAS mbedcrypto)
#endif()

#if(NOT TARGET GTest::GMock)
#    add_library(GMock::GMock ALIAS gmock)
 #   add_library(GMock::Main ALIAS gmock_main)
#endif()


set(MBEDTLS_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/mbedtls-src/include" CACHE INTERNAL "MBEDTLS_INCLUDE_DIR")
include_directories(${MBEDTLS_INCLUDE_DIR})
link_directories(${MBEDTLS_LIBRARIES})

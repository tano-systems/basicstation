# Download and unpack pico_gateway at configure time
# See: http://crascit.com/2015/07/25/cmake-gtest/
configure_file(CMakeLists.txt.in pico_gateway-download/CMakeLists.txt)
# Call CMake to download pico_gateway
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/pico_gateway-download )
if(result)
  message(FATAL_ERROR "CMake step for pico_gateway failed: ${result}")
endif()
# Build the downloaded pico_gateway
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/pico_gateway-download )
if(result)
  message(FATAL_ERROR "Build step for pico_gateway failed: ${result}")
endif()

# Add pico_gateway directly to our build.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/pico_gateway-src
                 ${CMAKE_CURRENT_BINARY_DIR}/pico_gateway-build)

# Add aliases for picogw
if(NOT TARGET Semtech::::picogw)
    add_library(Semtech::picogw ALIAS picogw)
endif()

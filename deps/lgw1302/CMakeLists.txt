# Download and unpack sx1302_hal at configure time
# See: http://crascit.com/2015/07/25/cmake-gtest/
configure_file(CMakeLists.txt.in sx1302_hal-download/CMakeLists.txt)
# Call CMake to download lora_gateway
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/sx1302_hal-download )
if(result)
  message(FATAL_ERROR "CMake step for sx1302_hal failed: ${result}")
endif()
# Build the downloaded lora_gateway
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/sx1302_hal-download )
if(result)
  message(FATAL_ERROR "Build step for sx1302_hal failed: ${result}")
endif()

# Add lora_gateway directly to our build.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/sx1302_hal-src
                 ${CMAKE_CURRENT_BINARY_DIR}/sx1302_hal-build)

# Add aliases for loragw
if(NOT TARGET Semtech::1302gw)
    add_library(Semtech::1302gw ALIAS 1302gw)
endif()

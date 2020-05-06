# Download and unpack lora_gateway at configure time
# See: http://crascit.com/2015/07/25/cmake-gtest/
configure_file(CMakeLists.txt.in lora_gateway-download/CMakeLists.txt)
# Call CMake to download lora_gateway
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lora_gateway-download )
if(result)
  message(FATAL_ERROR "CMake step for lora_gateway failed: ${result}")
endif()
# Build the downloaded lora_gateway
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lora_gateway-download )
if(result)
  message(FATAL_ERROR "Build step for lora_gateway failed: ${result}")
endif()

# Add lora_gateway directly to our build.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/lora_gateway-src
                 ${CMAKE_CURRENT_BINARY_DIR}/lora_gateway-build)

# Add aliases for loragw
if(NOT TARGET Semtech::::loragw)
    add_library(Semtech::loragw ALIAS loragw)
endif()

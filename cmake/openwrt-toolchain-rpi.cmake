set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)

set(ENV{STAGING_DIR} "/home/lx/workspace/openwrt-rpi/staging_dir")

set(OPENWRT_STAGING_DIR $ENV{STAGING_DIR})
set(OPENWRT_TARGET target-arm_cortex-a7+neon-vfpv4_musl_eabi)
set(OPENWRT_TOOLCHAIN toolchain-arm_cortex-a7+neon-vfpv4_gcc-8.3.0_musl_eabi)
set(OPENWRT_CROSS ${OPENWRT_STAGING_DIR}/${OPENWRT_TOOLCHAIN}/bin/arm-openwrt-linux-)

set(CMAKE_C_COMPILER ${OPENWRT_CROSS}gcc)
set(CMAKE_CXX_COMPILER ${OPENWRT_CROSS}g++)

# where is the target environment
set(CMAKE_FIND_ROOT_PATH ${OPENWRT_STAGING_DIR}/${OPENWRT_TARGET})

set(CMAKE_EXE_LINKER_FLAGS		"${CMAKE_EXE_LINKER_FLAGS}		--sysroot=${CMAKE_FIND_ROOT_PATH}")
set(CMAKE_SHARED_LINKER_FLAGS	"${CMAKE_SHARED_LINKER_FLAGS}	--sysroot=${CMAKE_FIND_ROOT_PATH}")
set(CMAKE_MODULE_LINKER_FLAGS	"${CMAKE_MODULE_LINKER_FLAGS}	--sysroot=${CMAKE_FIND_ROOT_PATH}")


# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


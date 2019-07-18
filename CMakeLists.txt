# -- Minimum required version
cmake_minimum_required (VERSION 3.2)

# -- Project name
project (basicstation)

# -- Various includes
include (CMakePackageConfigHelpers)
include (GNUInstallDirs)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")

# -- set c99 standard default
set(CMAKE_C_STANDARD 11)

# -- Required to build
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
find_package(mbedTLS REQUIRED)

# -- Versioning with git tag
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.git)
	execute_process(
		COMMAND git describe --tags --always
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
		OUTPUT_VARIABLE "basicstation_VERSION"
		ERROR_QUIET
		OUTPUT_STRIP_TRAILING_WHITESPACE)
		if(basicstation_VERSION STREQUAL "")
			set(basicstation_VERSION 0)
		endif(basicstation_VERSION STREQUAL "")
		message( STATUS "Git full version: ${basicstation_VERSION}" )
	execute_process(
		COMMAND /bin/bash -c "git describe --tags --abbrev=0 | cut --delimiter='v' --fields=2"
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
		OUTPUT_VARIABLE "basicstation_VERSION_SHORT"
		ERROR_QUIET
		OUTPUT_STRIP_TRAILING_WHITESPACE)
		if(basicstation_VERSION_SHORT STREQUAL "")
			set(basicstation_VERSION_SHORT 0)
		endif(basicstation_VERSION_SHORT STREQUAL "")
	message( STATUS "Git version: ${basicstation_VERSION_SHORT}" )
else(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.git)
	set(basicstation_VERSION_SHORT 0)
	set(basicstation_VERSION 0)
endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.git)

# -- Current date
execute_process(
	COMMAND date -u "+%Y-%m-%d %H:%M:%S"
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	OUTPUT_VARIABLE DATE
	ERROR_QUIET
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

# -- Current commit
execute_process(
	COMMAND git rev-parse --short HEAD
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	OUTPUT_VARIABLE COMMIT
	ERROR_QUIET
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

# -- find packages
find_package(loragw QUIET)
if(NOT loragw_FOUND)
	find_package(PkgConfig)
	pkg_search_module(LORAGW loragw)

	if(LORAGW_FOUND)
		message("-- pkg_config: libloragw is found")
		message("-- libloragw include: ${LORAGW_INCLUDE_DIRS}")
		message("-- libloragw library: ${LORAGW_LINK_LIBRARIES}")
	endif()
else()
	message("-- CMake: libloragw is found")
endif()

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
if (NOT (${CMAKE_INSTALL_PREFIX} STREQUAL "/usr" ) )
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
endif()

# -- Options
set(PLATFORM "linux" CACHE STRING "target platform")
set_property(CACHE PLATFORM PROPERTY STRINGS linux rpi kerlink)
message("-- Platform: " ${PLATFORM})

add_executable(basicstation "")

# --  add the compile options
target_compile_options(
	basicstation
	PRIVATE
	-Wall
#	-Wextra
	-DCFG_version="${COMMIT}"
	-DCFG_bdate="${DATE}"
	-DCFG_platform="linux"
	-DCFG_linux
	-DCFG_no_leds
	-DCFG_lgw1
	-DCFG_logini_lvl=INFO
	-DCFG_tlsdebug
	-DCFG_ral_lgw
)

target_compile_definitions(
	basicstation
	PRIVATE
	VERSION_STRING="${basicstation_VERSION_SHORT}"
)

target_sources(
	basicstation
	PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/src/aio.c
		${CMAKE_CURRENT_LIST_DIR}/src/argp.c
		${CMAKE_CURRENT_LIST_DIR}/src/cups.c
		${CMAKE_CURRENT_LIST_DIR}/src/flashsim.c
		${CMAKE_CURRENT_LIST_DIR}/src/fs.c
		${CMAKE_CURRENT_LIST_DIR}/src/lgwsim.c
		${CMAKE_CURRENT_LIST_DIR}/src/log.c
		${CMAKE_CURRENT_LIST_DIR}/src/lora.c
		${CMAKE_CURRENT_LIST_DIR}/src/net.c
		${CMAKE_CURRENT_LIST_DIR}/src/ral_lgw.c
		${CMAKE_CURRENT_LIST_DIR}/src/ral_lgw2.c
		${CMAKE_CURRENT_LIST_DIR}/src/rt.c
		${CMAKE_CURRENT_LIST_DIR}/src/s2conf.c
		${CMAKE_CURRENT_LIST_DIR}/src/s2e.c
		${CMAKE_CURRENT_LIST_DIR}/src/selftests.c
		${CMAKE_CURRENT_LIST_DIR}/src/sx1301conf.c
		${CMAKE_CURRENT_LIST_DIR}/src/sx1301v2conf.c
		${CMAKE_CURRENT_LIST_DIR}/src/sys.c
		${CMAKE_CURRENT_LIST_DIR}/src/tc.c
		${CMAKE_CURRENT_LIST_DIR}/src/timesync.c
		${CMAKE_CURRENT_LIST_DIR}/src/tls.c
		${CMAKE_CURRENT_LIST_DIR}/src/uj.c
		${CMAKE_CURRENT_LIST_DIR}/src/web.c
		${CMAKE_CURRENT_LIST_DIR}/src/xq.c
		${CMAKE_CURRENT_LIST_DIR}/src-linux/cmdfifo.c
		${CMAKE_CURRENT_LIST_DIR}/src-linux/commands.c
		${CMAKE_CURRENT_LIST_DIR}/src-linux/gps.c
		${CMAKE_CURRENT_LIST_DIR}/src-linux/leds.c
		${CMAKE_CURRENT_LIST_DIR}/src-linux/ral_master.c
		${CMAKE_CURRENT_LIST_DIR}/src-linux/ral_slave.c
		${CMAKE_CURRENT_LIST_DIR}/src-linux/rmtsh.c
		${CMAKE_CURRENT_LIST_DIR}/src-linux/station_main.c
		${CMAKE_CURRENT_LIST_DIR}/src-linux/sys_linux.c
		${CMAKE_CURRENT_LIST_DIR}/src-linux/sys_log.c
		${CMAKE_CURRENT_LIST_DIR}/src-linux/web_linux.c
)

target_include_directories(
	basicstation
	PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/src
		${CMAKE_CURRENT_LIST_DIR}/src-linux
		${MBEDTLS_INCLUDE_DIR}
)

target_link_libraries(
	basicstation
	PUBLIC
		Threads::Threads
		m
	PRIVATE
		Semtech::loragw
		${MBEDTLS_LIBRARIES}
)

set_target_properties(basicstation PROPERTIES VERSION ${basicstation_VERSION})
set_target_properties(basicstation PROPERTIES SOVERSION ${basicstation_VERSION_SHORT})

# add the install targets
install (
	TARGETS basicstation
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT shlib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_executable(crc32 "")
target_compile_options(
	crc32
	PRIVATE
		-DCFG_prog_crc32
)

target_sources(
	crc32
	PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/src/crc32.c
)

# add the install targets
install (
	TARGETS crc32
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT shlib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
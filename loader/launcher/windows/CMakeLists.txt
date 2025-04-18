cmake_minimum_required(VERSION 3.21)

add_library(ProxyLoader SHARED proxyLoader.cpp)
set_target_properties(ProxyLoader PROPERTIES
	PREFIX ""
	OUTPUT_NAME "XInput1_4"
	LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${FREOD_BIN_PATH}/nightly"
	RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${FREOD_BIN_PATH}/nightly"
	ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${FREOD_BIN_PATH}/nightly"
	LIBRARY_OUTPUT_DIRECTORY_RELEASE "${FREOD_BIN_PATH}/nightly"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${FREOD_BIN_PATH}/nightly"
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${FREOD_BIN_PATH}/nightly"
	LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${FREOD_BIN_PATH}/nightly"
	RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${FREOD_BIN_PATH}/nightly"
	ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${FREOD_BIN_PATH}/nightly"
	LIBRARY_OUTPUT_DIRECTORY "${FREOD_BIN_PATH}/nightly"
	RUNTIME_OUTPUT_DIRECTORY "${FREOD_BIN_PATH}/nightly"
	ARCHIVE_OUTPUT_DIRECTORY "${FREOD_BIN_PATH}/nightly"
)

string(REPLACE "." "_" FREOD_GD_VERSION_IDENTIFIER "_${FREOD_GD_VERSION}")
target_compile_definitions(ProxyLoader PRIVATE FREOD_GD_VERSION_IDENTIFIER=${FREOD_GD_VERSION_IDENTIFIER})

add_executable(Updater Updater.cpp)
target_compile_features(Updater PUBLIC cxx_std_17)
set_target_properties(Updater PROPERTIES
	PREFIX ""
	OUTPUT_NAME "FreodUpdater"
	RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${FREOD_BIN_PATH}/nightly"
	ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${FREOD_BIN_PATH}/nightly"
	LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${FREOD_BIN_PATH}/nightly"
	LIBRARY_OUTPUT_DIRECTORY_RELEASE "${FREOD_BIN_PATH}/nightly"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${FREOD_BIN_PATH}/nightly"
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${FREOD_BIN_PATH}/nightly"
	LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${FREOD_BIN_PATH}/nightly"
	RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${FREOD_BIN_PATH}/nightly"
	ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${FREOD_BIN_PATH}/nightly"
	LIBRARY_OUTPUT_DIRECTORY "${FREOD_BIN_PATH}/nightly"
	RUNTIME_OUTPUT_DIRECTORY "${FREOD_BIN_PATH}/nightly"
	ARCHIVE_OUTPUT_DIRECTORY "${FREOD_BIN_PATH}/nightly"
)

if (MSVC)
	include(CheckLinkerFlag)
	check_linker_flag(CXX /NOEXP SUPPORTS_NOEXP)
	if (SUPPORTS_NOEXP)
		target_link_options(ProxyLoader PRIVATE /NOEXP)
	endif()

	target_link_options(ProxyLoader PRIVATE /NOIMPLIB /DEBUG:NONE)
	target_link_options(Updater PRIVATE /DEBUG:NONE)
endif()

cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT Embedded)
cmake_policy(SET CMP0141 NEW)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build libraries static" FORCE)

# Docs flags
if (FREOD_BUILDING_DOCS)
	set(FREOD_DISABLE_CLI_CALLS ON)
	set(FREOD_DISABLE_PRECOMPILED_HEADERS ON)
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
	set(FREOD_DONT_BUILD_TEST_MODS ON)
endif()

option(FREOD_USE_BREAKPAD "Enables the use of the Breakpad library for crash dumps." ON)

# Check if git is installed, raise a fatal error if not
find_program(GIT_EXECUTABLE git)
if (NOT GIT_EXECUTABLE)
	message(FATAL_ERROR "Git not found! Please install Git and try again.\nhttps://git-scm.com/")
endif()

# Read version
file(READ VERSION FREOD_VERSION)
string(STRIP "${FREOD_VERSION}" FREOD_VERSION)
set(FREOD_VERSION "${FREOD_VERSION}" CACHE INTERNAL "Freod version")
set(FREOD_VERSION_FULL "${FREOD_VERSION}" CACHE INTERNAL "Freod version full")

# Check if version has a tag like v1.0.0-alpha
string(FIND ${FREOD_VERSION} "-" FREOD_VERSION_HAS_TAG)
if (NOT ${FREOD_VERSION_HAS_TAG} EQUAL "-1")
	string(REGEX MATCH "[a-z]+(\.[0-9]+)?$" FREOD_VERSION_TAG ${FREOD_VERSION})
	string(SUBSTRING "${FREOD_VERSION}" 0 ${FREOD_VERSION_HAS_TAG} FREOD_VERSION)
	string(FIND ${FREOD_VERSION_TAG} "." FREOD_VERSION_TAG_HAS_NUMBER)

	# Extract tag type and number from tag
	if (NOT ${FREOD_VERSION_TAG_HAS_NUMBER} EQUAL "-1")
		string(SUBSTRING "${FREOD_VERSION_TAG}" 0 ${FREOD_VERSION_TAG_HAS_NUMBER} FREOD_VERSION_TAG_TYPE)
		math(EXPR FREOD_VERSION_TAG_HAS_NUMBER "${FREOD_VERSION_TAG_HAS_NUMBER} + 1")
		string(SUBSTRING "${FREOD_VERSION_TAG}" ${FREOD_VERSION_TAG_HAS_NUMBER} -1 FREOD_VERSION_TAG_NUMBER)
	else()
		set(FREOD_VERSION_TAG_TYPE "${FREOD_VERSION_TAG}")
		set(FREOD_VERSION_TAG_NUMBER "")
	endif()

	# Capitalize first letter of tag type
	string(SUBSTRING ${FREOD_VERSION_TAG_TYPE} 0 1 FIRST_LETTER)
	string(TOUPPER ${FIRST_LETTER} FIRST_LETTER)
	string(REGEX REPLACE "^.(.*)" "${FIRST_LETTER}\\1" FREOD_VERSION_TAG_TYPE "${FREOD_VERSION_TAG_TYPE}")

	message(STATUS "Version: ${FREOD_VERSION}, tag: ${FREOD_VERSION_TAG} (type: ${FREOD_VERSION_TAG_TYPE}, number: ${FREOD_VERSION_TAG_NUMBER})")
else()
	set(FREOD_VERSION_TAG "")
	set(FREOD_VERSION_TAG_TYPE "")
	set(FREOD_VERSION_TAG_NUMBER "")

	message(STATUS "Version: ${FREOD_VERSION}")
endif()

include(cmake/CPM.cmake)

if (NOT DEFINED CPM_SOURCE_CACHE)
	message(NOTICE "Not using CPM cache (CPM_SOURCE_CACHE). "
		"It is recommended to use it to improve configure times.")
endif()

# Allow users to have their own copy of bindings that can be overwritten with a CMake option.
# If the option is not provided, by default just clone bindings with CPM and use that
if (DEFINED ENV{FREOD_BINDINGS_REPO_PATH})
	set(temp $ENV{FREOD_BINDINGS_REPO_PATH})
	# this is so stupid i hate windows paths
	string(REPLACE "\\" "/" FREOD_BINDINGS_REPO_PATH ${temp})
endif()

if (NOT FREOD_BINDINGS_REPO_PATH)
	message(STATUS
		"No override path for bindings provided, using CPM to clone default. "
		"If you would like to use a separate clone of the bindings repo "
		"(for example in order to be able to efficiently change and "
		"contribute new bindings) then set FREOD_BINDINGS_REPO_PATH to where you have "
		"cloned the repository (system environment variables are supported)."
	)
	CPMAddPackage(NAME "bindings"
		GITHUB_REPOSITORY "freod-sdk/bindings"
		GIT_TAG "main"
		DOWNLOAD_ONLY YES
		NO_CACHE YES
	)
	set(FREOD_BINDINGS_REPO_PATH ${bindings_SOURCE_DIR})
else()
	message(STATUS "Using ${FREOD_BINDINGS_REPO_PATH} for bindings repo")
endif()

project(freod-sdk VERSION ${FREOD_VERSION} LANGUAGES CXX C)

if (NOT FREOD_DONT_USE_CCACHE)
	message(STATUS "Looking for ccache/sccache")
	if (DEFINED CMAKE_C_COMPILER_LAUNCHER OR DEFINED CMAKE_CXX_COMPILER_LAUNCHER)
		message(STATUS "Looking for ccache/sccache - detecting variant")
		if (DEFINED CMAKE_C_COMPILER_LAUNCHER AND DEFINED CMAKE_CXX_COMPILER_LAUNCHER AND
			CMAKE_C_COMPILER_LAUNCHER STREQUAL CMAKE_CXX_COMPILER_LAUNCHER)
			if (CMAKE_CXX_COMPILER_LAUNCHER MATCHES "sccache(|.exe)$")
				message(STATUS "Looking for ccache/sccache - detected sccache")
				set(FREOD_CCACHE_VARIANT "sccache")
			elseif(CMAKE_CXX_COMPILER_LAUNCHER MATCHES "ccache(|.exe)$")
				message(STATUS "Looking for ccache/sccache - detected ccache")
				set(FREOD_CCACHE_VARIANT "ccache")
			else()
				message(STATUS "Looking for ccache/sccache - none (custom compiler launcher: {CMAKE_C_COMPILER_LAUNCHER}, {CMAKE_CXX_COMPILER_LAUNCHER})")
			endif()
		else()
			message(STATUS "Looking for ccache/sccache - skipped (custom compiler launcher: {CMAKE_C_COMPILER_LAUNCHER}, {CMAKE_CXX_COMPILER_LAUNCHER})")
		endif()
	elseif (CMAKE_GENERATOR MATCHES "Visual Studio")
		message(STATUS "Looking for ccache/sccache - skipped (Visual Studio generator)")
	else()
		find_program(FREOD_CCACHE NAMES sccache ccache)
		if (FREOD_CCACHE)
			if (${FREOD_CCACHE} MATCHES "sccache(|.exe)$")
				set(FREOD_CCACHE_VARIANT "sccache")
			else()
				set(FREOD_CCACHE_VARIANT "ccache")
			endif()
			message(STATUS "Looking for ccache/sccache - found ${FREOD_CCACHE_VARIANT}")
			message(NOTICE "Compiler launcher not set but ccache/sccache was found. "
				"Setting compiler launcher to that")
			set(CMAKE_C_COMPILER_LAUNCHER ${FREOD_CCACHE})
			set(CMAKE_CXX_COMPILER_LAUNCHER ${FREOD_CCACHE})
		else()
			message(STATUS "Looking for ccache/sccache - not found")
		endif()
		unset(FREOD_CCACHE)
	endif()
else()
	message(STATUS "Not looking for ccache because FREOD_DONT_USE_CCACHE was ON")
endif()

if (DEFINED FREOD_CCACHE_VARIANT)
	if (NOT DEFINED FREOD_DISABLE_PRECOMPILED_HEADERS)
		if (${FREOD_CCACHE_VARIANT} STREQUAL "sccache" AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
			if (APPLE AND DEFINED CMAKE_OSX_ARCHITECTURES AND (CMAKE_OSX_ARCHITECTURES STREQUAL "arm64;x86_64" OR CMAKE_OSX_ARCHITECTURES STREQUAL "x86_64;arm64"))
				message(NOTICE "Using ${FREOD_CCACHE_VARIANT} with ${CMAKE_CXX_COMPILER_ID} while building multiple architectures, PCH will be disabled due to issues with sccache.")
				message("(You can override this by setting FREOD_DISABLE_PRECOMPILED_HEADERS to ON).")

				set(FREOD_DISABLE_PRECOMPILED_HEADERS ON)
			else()
				message(NOTICE "Using ${FREOD_CCACHE_VARIANT} with ${CMAKE_CXX_COMPILER_ID}, PCH will be enabled.")
				set(FREOD_DISABLE_PRECOMPILED_HEADERS OFF)
			endif()
		else()
			message(NOTICE "Using ${FREOD_CCACHE_VARIANT} with ${CMAKE_CXX_COMPILER_ID}, PCH will be disabled.")
			set(FREOD_DISABLE_PRECOMPILED_HEADERS ON)
		endif()
	endif()
else()
	if (APPLE)
		# when building for multiple architectures, a caching compiler is not recommended
		message(NOTICE "Not using a caching compiler (ccache/sccache).")
	else()
		message(NOTICE "Not using a caching compiler (ccache/sccache). "
			"It is recommended to install one to improve build times.")
		message(NOTICE "We recommend sccache, check its README for installation instructions, "
			"normally you can just use your usual package manager (e.g. 'scoop install sccache').")
	endif()

	if (NOT DEFINED FREOD_DISABLE_PRECOMPILED_HEADERS)
		message(NOTICE "Because of this, PCH will be enabled.")
		set(FREOD_DISABLE_PRECOMPILED_HEADERS OFF)
	endif()
endif()

if (NOT DEFINED FREOD_DISABLE_PRECOMPILED_HEADERS)
	set(FREOD_DISABLE_PRECOMPILED_HEADERS OFF)
endif()

if (FREOD_DISABLE_PRECOMPILED_HEADERS)
	message(STATUS "Pre-compiled headers - OFF")
else()
	message(STATUS "Pre-compiled headers - ON")
endif()

add_library(${PROJECT_NAME} INTERFACE)

# Rerun CMake on VERSION file change
set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_CONFIGURE_DEPENDS VERSION)

target_compile_definitions(${PROJECT_NAME} INTERFACE -DPROJECT_NAME=${CMAKE_PROJECT_NAME})

set(FREOD_BIN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(FREOD_LOADER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/loader)
set(FREOD_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

include(cmake/Platform.cmake)
include(cmake/FreodFile.cmake)

if (NOT DEFINED FREOD_GD_VERSION)
	set(FREOD_GD_VERSION 2.2074)
	set(FREOD_COMP_GD_VERSION 22074)
endif()

target_compile_definitions(
	${PROJECT_NAME} INTERFACE
	FREOD_GD_VERSION=${FREOD_GD_VERSION}
	FREOD_COMP_GD_VERSION=${FREOD_COMP_GD_VERSION}
	FREOD_GD_VERSION_STRING="${FREOD_GD_VERSION}"
)

if (WIN32)
	# This allows you to compile in debug mode
	# add_compile_definitions(_HAS_ITERATOR_DEBUGGING=0)
	add_definitions(-D_HAS_ITERATOR_DEBUGGING=0)
	target_compile_definitions(${PROJECT_NAME} INTERFACE _HAS_ITERATOR_DEBUGGING=0)
	
	target_link_libraries(${PROJECT_NAME} INTERFACE delayimp ws2_32)
	if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND
		CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
		# target x86 on windows with clang
		target_compile_options(${PROJECT_NAME} INTERFACE --target=x86_64-pc-windows-msvc)
		target_link_options(${PROJECT_NAME} INTERFACE --target=x86_64-pc-windows-msvc)
		add_compile_options(--target=x86_64-pc-windows-msvc)
		add_link_options(--target=x86_64-pc-windows-msvc)
	endif()
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	# force an error on a missing return type, as this is unfortunately a common mistake people make
	target_compile_options(${PROJECT_NAME} INTERFACE -Werror=return-type)
endif()

# if (APPLE AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
# 	set_property(TARGET ${PROJECT_NAME} PROPERTY LINKER_TYPE LLD)
# 	target_link_options(${PROJECT_NAME} INTERFACE -fuse-ld=lld)
# endif ()

# We do not support anything below SDK 23
if (ANDROID)
	string(REGEX MATCH "[0-9]+" ANDROID_PLATFORM_NUMBER "${ANDROID_PLATFORM}")
	if (ANDROID_PLATFORM_NUMBER LESS 23)
		message(FATAL_ERROR "Specified target Android SDK version is too low (${ANDROID_PLATFORM_NUMBER}), must be 23 or higher")
	endif()
endif()

set(MAT_JSON_AS_INTERFACE ON)
CPMAddPackage("gh:freod-sdk/result@1.3.3")
CPMAddPackage("gh:freod-sdk/json@3.2.1")
CPMAddPackage("gh:fmtlib/fmt#11.1.4")

target_compile_definitions(${PROJECT_NAME} INTERFACE MAT_JSON_DYNAMIC=1)

# this is needed for cross compilation on linux,
# since fmtlib will fail to compile otherwise
if (FREOD_DISABLE_FMT_CONSTEVAL)
	message(VERBOSE "Disabling FMT_CONSTEVAL")
	target_compile_definitions(fmt PUBLIC -DFMT_CONSTEVAL=)
endif()

# Tulip hook (hooking)
if (PROJECT_IS_TOP_LEVEL AND NOT FREOD_BUILDING_DOCS)
	set(TULIP_LINK_SOURCE ON)
endif()
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

# Allow users to have their own copy of TulipHook that can be overwritten with a CMake option.
# If the option is not provided, by default just clone TulipHook with CPM and use that
if (DEFINED ENV{FREOD_TULIPHOOK_REPO_PATH})
	set(temp $ENV{FREOD_TULIPHOOK_REPO_PATH})
	# this is so stupid i hate windows paths
	string(REPLACE "\\" "/" FREOD_TULIPHOOK_REPO_PATH ${temp})
endif()

if (DEFINED FREOD_TULIPHOOK_REPO_PATH)
	message(STATUS "Using ${FREOD_TULIPHOOK_REPO_PATH} for TulipHook")
	add_subdirectory(${FREOD_TULIPHOOK_REPO_PATH} ${FREOD_TULIPHOOK_REPO_PATH}/build)
else()
	CPMAddPackage("gh:freod-sdk/TulipHook@2.4.3")
endif()
set(CMAKE_WARN_DEPRECATED ON CACHE BOOL "" FORCE)

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-Wno-everything SUPPORTS_W_NO_EVERYTHING)
if (SUPPORTS_W_NO_EVERYTHING)
	# Silence warnings from dependencies
	if (TARGET capstone)
		target_compile_options(capstone PRIVATE -Wno-everything)
	endif()
endif()

target_sources(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/entry.cpp)

add_subdirectory(${FREOD_BINDINGS_REPO_PATH} ${CMAKE_BINARY_DIR}/bindings)

if (NOT FREOD_DISABLE_PRECOMPILED_HEADERS)
	target_precompile_headers(FreodBindings INTERFACE
		"$<$<COMPILE_LANGUAGE:CXX>:${FREOD_LOADER_PATH}/include/Freod/Bindings.hpp>"
	)
endif()

target_include_directories(FreodBindings PUBLIC
	${FREOD_LOADER_PATH}/include
	${FREOD_LOADER_PATH}/include/Freod/cocos/include
	${FREOD_LOADER_PATH}/include/Freod/cocos/extensions
	${FREOD_LOADER_PATH}/include/Freod/fmod
)
target_link_directories(FreodBindings PUBLIC ${FREOD_LOADER_PATH}/include/link)
target_link_libraries(FreodBindings PUBLIC fmt TulipHookInclude mat-json FreodResult)
target_link_libraries(${PROJECT_NAME} INTERFACE FreodBindings)

if (NOT EXISTS ${FREOD_BIN_PATH})
	file(MAKE_DIRECTORY ${FREOD_BIN_PATH})
endif()

if (NOT EXISTS ${FREOD_BIN_PATH}/${PROJECT_VERSION} AND EXISTS ${FREOD_BIN_PATH}/nightly/)
	set(FREOD_LINK_NIGHTLY 1)
endif()

if (${FREOD_LINK_NIGHTLY})
	set(FREOD_PLATFORM_BIN_PATH ${FREOD_BIN_PATH}/nightly/${FREOD_PLATFORM_BINARY})
else()
	set(FREOD_PLATFORM_BIN_PATH ${FREOD_BIN_PATH}/${PROJECT_VERSION}/${FREOD_PLATFORM_BINARY})
endif()

if (PROJECT_IS_TOP_LEVEL)
	add_subdirectory(loader)
	target_link_libraries(${PROJECT_NAME} INTERFACE freod-loader)
	if (SUPPORTS_W_NO_EVERYTHING)
		target_compile_options(freod-loader PRIVATE -Wno-inconsistent-missing-override)
	endif()
elseif (EXISTS ${FREOD_PLATFORM_BIN_PATH})
	target_link_libraries(${PROJECT_NAME} INTERFACE "${FREOD_PLATFORM_BIN_PATH}")
	if (NOT FREOD_DISABLE_PRECOMPILED_HEADERS)
		target_precompile_headers(${PROJECT_NAME} INTERFACE
			"$<$<COMPILE_LANGUAGE:CXX>:${FREOD_LOADER_PATH}/include/Freod/DefaultInclude.hpp>"
			"$<$<COMPILE_LANGUAGE:CXX>:${FREOD_LOADER_PATH}/include/Freod/Freod.hpp>"
			# please stop adding modify here its not here because it makes windows compilation take longer than freod 1.0 release date
		)
	endif()
else()
	message(FATAL_ERROR
		"No valid loader binary to link to! Install pre-built binaries with `freod sdk install-binaries`, "
		"or build Freod from source."
	)
endif()

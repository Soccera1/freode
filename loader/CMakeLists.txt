cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
cmake_policy(SET CMP0097 NEW)

project(freod-loader VERSION ${FREOD_VERSION} LANGUAGES C CXX)
if (FREOD_VERSION_TAG_TYPE)
	if (FREOD_VERSION_TAG_NUMBER)
		set(PROJECT_VERSION_TAG_CONSTR "freod::VersionTag(freod::VersionTag::${FREOD_VERSION_TAG_TYPE}, ${FREOD_VERSION_TAG_NUMBER})")
	else()
		set(PROJECT_VERSION_TAG_CONSTR "freod::VersionTag::${FREOD_VERSION_TAG_TYPE}")
	endif()
else()
	set(PROJECT_VERSION_TAG_CONSTR "std::nullopt")
endif()

if (FREOD_VERSION_TAG)
	set(PROJECT_VERSION_SUFFIX "-${FREOD_VERSION_TAG}" CACHE INTERNAL "" )
else()
	set(PROJECT_VERSION_SUFFIX "" CACHE INTERNAL "" )
endif()

# I think CMake is trying to link zlib from the host system?
# This prevents it from doing that.
if (WIN32 AND CMAKE_CROSSCOMPILING)
	set(MZ_FORCE_FETCH_LIBS ON)
endif()

# https://stackoverflow.com/a/63924044/9124836
# https://stackoverflow.com/a/72396471
execute_process(
	COMMAND git describe --always --dirty
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	OUTPUT_VARIABLE FREOD_COMMIT_HASH
	OUTPUT_STRIP_TRAILING_WHITESPACE
	ERROR_QUIET
)

if (NOT FREOD_COMMIT_HASH)
	set(FREOD_COMMIT_HASH "(?)")
endif()

execute_process(
	COMMAND git describe --always --dirty
	WORKING_DIRECTORY ${FREOD_BINDINGS_REPO_PATH}
	OUTPUT_VARIABLE FREOD_BINDINGS_COMMIT_HASH
	OUTPUT_STRIP_TRAILING_WHITESPACE
	ERROR_QUIET
)

if (NOT FREOD_BINDINGS_COMMIT_HASH)
	set(FREOD_BINDINGS_COMMIT_HASH "(?)")
endif()

# Package info file for internal representation
set(FREOD_RESOURCES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/resources)
configure_file(resources/mod.json.in ${CMAKE_CURRENT_SOURCE_DIR}/resources/mod.json)
file(READ resources/mod.json LOADER_MOD_JSON)
configure_file(${FREOD_ROOT_PATH}/VERSION ${FREOD_RESOURCES_PATH}/version COPYONLY)
configure_file(${FREOD_RESOURCES_PATH}/about.md.in ${FREOD_RESOURCES_PATH}/about.md NEWLINE_STYLE LF)
configure_file(${FREOD_ROOT_PATH}/CHANGELOG.md ${FREOD_RESOURCES_PATH}/changelog.md NEWLINE_STYLE LF)
configure_file(${FREOD_RESOURCES_PATH}/support.md.in ${FREOD_RESOURCES_PATH}/support.md NEWLINE_STYLE LF)
configure_file(src/internal/about.cpp.in ${CMAKE_CURRENT_SOURCE_DIR}/src/internal/about.cpp)

# Source files
file(GLOB SOURCES CONFIGURE_DEPENDS 
	src/cocos2d-ext/*.cpp
	src/cocos2d-ext/zip/*.cpp
	src/core/*.cpp
	src/hooks/*.cpp
	src/ids/*.cpp
	src/internal/*.cpp
	src/server/*.cpp
	src/loader/*.cpp
	src/load.cpp
	src/utils/*.cpp
	src/ui/*.cpp
	src/ui/nodes/*.cpp
	src/ui/other/*.cpp
	src/ui/mods/*.cpp
	src/ui/mods/list/*.cpp
	src/ui/mods/popups/*.cpp
	src/ui/mods/events/*.cpp
	src/ui/mods/sources/*.cpp
	src/ui/mods/settings/*.cpp
	src/ui/mods/test/*.cpp
	src/ui/*.cpp
	src/c++stl/*.cpp
	hash/hash.cpp
)

# Obj-c sources
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
	file(GLOB OBJC_SOURCES CONFIGURE_DEPENDS
		src/platform/ios/*.mm
		src/load.mm
		src/hooks/AddExtraKeys.mm
	)
else()
	file(GLOB OBJC_SOURCES CONFIGURE_DEPENDS
		src/platform/mac/*.mm
		src/load.mm
		src/hooks/AddExtraKeys.mm
	)
endif()
set_source_files_properties(${OBJC_SOURCES} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

if (WIN32)
	# This allows you to compile in debug mode
	add_compile_definitions(_HAS_ITERATOR_DEBUGGING=0)
	add_definitions(-D_HAS_ITERATOR_DEBUGGING=0)
endif()


# Add platform sources
if (WIN32)

	file(GLOB WIN_SOURCES CONFIGURE_DEPENDS 
		src/platform/windows/*.cpp
	)
	list(APPEND SOURCES ${WIN_SOURCES})

elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
	file(GLOB IOS_SOURCES CONFIGURE_DEPENDS
		src/platform/ios/*.cpp
		src/platform/mac/Cocos2d.cpp # identical on ios, so we just use the mac one
	)
	list(APPEND SOURCES ${IOS_SOURCES})
	list(APPEND SOURCES ${OBJC_SOURCES})

elseif(APPLE)
	file(GLOB MAC_SOURCES CONFIGURE_DEPENDS
		src/platform/mac/*.cpp
	)
	list(APPEND SOURCES ${MAC_SOURCES})
	list(APPEND SOURCES ${OBJC_SOURCES})

elseif(ANDROID)

	file(GLOB ANDROID_SOURCES CONFIGURE_DEPENDS
		src/platform/android/*.cpp
	)
	list(APPEND SOURCES ${ANDROID_SOURCES})

endif()

# Embed version info in binary
if (WIN32)
	configure_file(src/platform/windows/info.rc.in info.rc)
	set(SOURCES ${SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/info.rc)
endif()


if (ANDROID AND FREOD_USE_BREAKPAD)
	CPMAddPackage("gh:qimiko/breakpad#169df6a")
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

file(MAKE_DIRECTORY "${FREOD_BIN_PATH}/nightly")

# Prevent SDK from activating a loader build
set_target_properties(${PROJECT_NAME} PROPERTIES
	PREFIX ""
	OUTPUT_NAME ${FREOD_OUTPUT_NAME}
	# i'm going to say a slur that not a single soul has uttered in the last 200 years
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

if (FREOD_NO_UNDEFINED_VIRTUALS)
	target_compile_definitions(${PROJECT_NAME} PUBLIC FREOD_NO_UNDEFINED_VIRTUALS)
endif()

# CA Bundle
CPMAddPackage(
	GITHUB_REPOSITORY freod-sdk/net_libs
	GIT_TAG 1f5ffb3
# CPM is bugged and will not allow an empty string.
# https://github.com/cpm-cmake/CPM.cmake/issues/467
# https://github.com/cpm-cmake/CPM.cmake/issues/461
	GIT_SUBMODULES "nghttp3"
	GIT_SUBMODULES_RECURSE OFF
)
target_link_libraries(${PROJECT_NAME} ca-bundle)

# Package resources for UI
package_freod_resources_now(
	${PROJECT_NAME}
	${CMAKE_CURRENT_SOURCE_DIR}/resources
	${FREOD_BIN_PATH}/nightly/resources
	${CMAKE_CURRENT_SOURCE_DIR}/src/internal/resources.hpp
)

if (APPLE) 
	# I don't care a single bit where to put this
	file(COPY ${FREOD_LOADER_PATH}/include/link/macos/libfmod.dylib DESTINATION ${FREOD_BIN_PATH}/nightly)
endif()

if (ANDROID)
	# needed to define some opengl functions
	target_link_libraries(${PROJECT_NAME} EGL)

	if (FREOD_USE_BREAKPAD)
		target_compile_definitions(${PROJECT_NAME} PRIVATE -DFREOD_USE_BREAKPAD)
		target_link_libraries(${PROJECT_NAME} Breakpad)
	endif()
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
	src/
	src/loader/
	src/internal/
	src/platform/
	src/gui/
	hash/
	./
)

if (APPLE)
	# For profiling
	target_compile_options(${PROJECT_NAME} PUBLIC "-ftime-trace")
	# target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-e,_dynamicInit")
	#set_property(TARGET ${PROJECT_NAME} PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC FREOD_EXPORTING MAT_JSON_EXPORTING)

target_compile_definitions(${PROJECT_NAME} PRIVATE
	FREOD_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE
	_CRT_SECURE_NO_WARNINGS
)

# set FREOD_MOD_ID for loader itself
target_compile_definitions(${PROJECT_NAME} PRIVATE FREOD_MOD_ID="freod.loader")

# These are only needed for building source :-)
if (NOT FREOD_BUILDING_DOCS)
	# Markdown support
	CPMAddPackage("gh:mity/md4c#release-0.5.2")

	# Zip support (needed for in-memory streams, which zlib's minizip doesn't support)
	set(MZ_LZMA OFF CACHE INTERNAL "")
	set(MZ_ZSTD OFF CACHE INTERNAL "")
	set(MZ_BZIP2 OFF CACHE INTERNAL "")
	set(MZ_OPENSSL OFF CACHE INTERNAL "")
	set(MZ_LIBBSD OFF CACHE INTERNAL "")
	set(MZ_FETCH_LIBS ON CACHE INTERNAL "" FORCE)
	set(MZ_FORCE_FETCH_LIBS ${WIN32} CACHE INTERNAL "")
	set(SKIP_INSTALL_ALL ON CACHE INTERNAL "")
	CPMAddPackage("gh:freod-sdk/minizip-ng#43d9254")
	if (WIN32)
		set_target_properties(zlib PROPERTIES SUFFIX "1.dll")
	endif()

	# Silence warnings from dependencies
	include(CheckCXXCompilerFlag)
	check_cxx_compiler_flag(-Wno-everything SUPPORTS_W_NO_EVERYTHING)
	if (SUPPORTS_W_NO_EVERYTHING)
		if (TARGET md4c)
			target_compile_options(md4c PRIVATE -Wno-everything)
		endif()
		if (TARGET zlib)
			target_compile_options(zlib PRIVATE -Wno-everything)
		endif()
		if (TARGET zlibstatic)
			get_property(zlibstatic_ALIASED TARGET zlibstatic PROPERTY ALIASED_TARGET)
			if ("${zlibstatic_ALIASED}" STREQUAL "")
				target_compile_options(zlibstatic PRIVATE -Wno-everything)
			endif()
		endif()
	endif()

	# Cool date library
	CPMAddPackage("gh:HowardHinnant/date#1ead671")

	target_include_directories(${PROJECT_NAME} PRIVATE ${md4c_SOURCE_DIR}/src)

	target_link_libraries(${PROJECT_NAME} md4c minizip date)
endif()

target_link_libraries(${PROJECT_NAME} TulipHook freod-sdk mat-json-impl)

# Required by curl for macos
if (APPLE)
	target_link_libraries(${PROJECT_NAME} z)
endif()

if (MSVC)
	# Disable outputting .exp file
	include(CheckLinkerFlag)
	check_linker_flag(CXX /NOEXP SUPPORTS_NOEXP)
	if (SUPPORTS_NOEXP)
		target_link_options(${PROJECT_NAME} PRIVATE /NOEXP)
	endif()
endif()

if (WIN32)
	# allows forward compat on windows
	if (MSVC)
		target_link_options(${PROJECT_NAME} PRIVATE /DELAYLOAD:libcocos2d.dll /DELAYLOAD:libExtensions.dll)
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		target_link_options(${PROJECT_NAME} PRIVATE "-Wl,/delayload:libcocos2d.dll" "-Wl,/delayload:libExtensions.dll")
	else()
		message(FATAL_ERROR "Unable to set DELAYLOAD! This is essential for Freod forward compatibility to work.")
	endif()
endif()

# Use precompiled headers for faster builds
if (NOT FREOD_DISABLE_PRECOMPILED_HEADERS)
	target_precompile_headers(${PROJECT_NAME} PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}/include/Freod/Freod.hpp"
	)
endif()

# Create launcher
if (APPLE)

	if("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
		# Used for File Picker API
		find_library(UNIFORM_TYPE_IDENTIFIERS_FRAMEWORK UniformTypeIdentifiers)
		target_link_libraries(${PROJECT_NAME} ${UNIFORM_TYPE_IDENTIFIERS_FRAMEWORK})
	else()
		set_target_properties(freod-loader PROPERTIES 
			SYSTEM_NAME MacOS
			OSX_DEPLOYMENT_TARGET 10.15
			APPLE_SILICON_PROCESSOR x86_64
		)

		# freodbootstrapper is unused on ios
		add_subdirectory(launcher/mac)
		set(LAUNCHER_TARGET FreodBootstrapper)
	endif()
elseif (WIN32)
	add_subdirectory(launcher/windows)

	target_link_libraries(${PROJECT_NAME} dbghelp winmm)

	target_compile_definitions(${PROJECT_NAME} PUBLIC ISOLATION_AWARE_ENABLED=1)

	if (MSVC)
		# disable warnings about CCNode::setID
		target_link_options(${PROJECT_NAME} PUBLIC /ignore:4217)
	endif()

	set(LAUNCHER_TARGET ProxyLoader Updater)
endif()

add_subdirectory(test)

# Add install target on CLI >= 2.10.0 (which adds `freod profile path`)
if (NOT FREOD_BUILDING_DOCS)
	# nest this because when building docs FREOD_CLI_VERSION is not defined
	if (${FREOD_CLI_VERSION} VERSION_GREATER_EQUAL "2.10.0")
		execute_process(
			COMMAND ${FREOD_CLI} profile path -d
			OUTPUT_VARIABLE FREOD_PROFILE_GD_PATH
			RESULT_VARIABLE FREOD_PROFILE_GD_PATH_RESULT
		)
		if (${FREOD_PROFILE_GD_PATH_RESULT} EQUAL 0)
			string(STRIP ${FREOD_PROFILE_GD_PATH} FREOD_PROFILE_GD_PATH)
			install(
				TARGETS freod-loader ${LAUNCHER_TARGET}
				RUNTIME
				DESTINATION "${FREOD_PROFILE_GD_PATH}"
			)
			file(GLOB RESOURCE_FILES ${FREOD_BIN_PATH}/nightly/resources/*.*)
			install(
				FILES ${RESOURCE_FILES}
				DESTINATION "${FREOD_PROFILE_GD_PATH}/freod/resources/freod.loader"
			)
		endif()
	endif()
endif()

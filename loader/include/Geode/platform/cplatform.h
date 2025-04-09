#pragma once

#ifdef _MSC_VER
    #pragma warning(disable : 4099) // type first seen as class
    #pragma warning(default : 4067)
    #pragma warning(disable : 4251) // dll-interface
    #pragma warning(disable : 4244) // narrowing conversion
#endif

// Set dllexport/dllimport to freod classes & functions

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__) || defined(WIN64) || defined(_WIN64) || defined(__WIN64) && !defined(__CYGWIN__)
    #define FREOD_WINDOWS(...) __VA_ARGS__
    #define FREOD_DESKTOP(...) __VA_ARGS__
    #define FREOD_MOBILE(...)
    #define FREOD_IS_WINDOWS
    #define FREOD_IS_DESKTOP
    #define FREOD_PLATFORM_NAME "Windows"
    #define FREOD_PLATFORM_EXTENSION ".dll"
    #define FREOD_PLATFORM_SHORT_IDENTIFIER "win"
    #define FREOD_PLATFORM_SHORT_IDENTIFIER_NOARCH "win"
    #define CC_TARGET_OS_WIN32

    #if defined(WIN64) || defined(_WIN64) || defined(__WIN64) && !defined(__CYGWIN__)
        #define FREOD_IS_WINDOWS64
        #define FREOD_WINDOWS64(...) __VA_ARGS__
        #define FREOD_WINDOWS32(...)
        #define FREOD_CALL
    #else
        #define FREOD_IS_WINDOWS32
        #define FREOD_WINDOWS32(...) __VA_ARGS__
        #define FREOD_WINDOWS64(...)
        #define FREOD_CALL __stdcall
    #endif
#else
    #define FREOD_WINDOWS(...)
    #define FREOD_WINDOWS32(...)
    #define FREOD_WINDOWS64(...)
#endif

#if defined(__APPLE__)
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE
        #define FREOD_MACOS(...)
        #define FREOD_INTEL_MAC(...)
        #define FREOD_ARM_MAC(...)
        #define FREOD_IOS(...) __VA_ARGS__
        #define FREOD_DESKTOP(...)
        #define FREOD_MOBILE(...) __VA_ARGS__
        #define FREOD_IS_IOS
        #define FREOD_IS_MOBILE
        #define FREOD_PLATFORM_NAME "iOS"
        #define FREOD_PLATFORM_EXTENSION ".ios.dylib"
        #define FREOD_PLATFORM_SHORT_IDENTIFIER "ios"
        #define FREOD_PLATFORM_SHORT_IDENTIFIER_NOARCH "ios"
        #define CC_TARGET_OS_IPHONE
    #else
        #define FREOD_IOS(...)
        #define FREOD_MACOS(...) __VA_ARGS__
        #define FREOD_DESKTOP(...) __VA_ARGS__
        #define FREOD_MOBILE(...)
        #define FREOD_IS_MACOS
        #define FREOD_IS_DESKTOP
        #define FREOD_PLATFORM_EXTENSION ".dylib"
        #define FREOD_PLATFORM_SHORT_IDENTIFIER_NOARCH "mac"
        #define CC_TARGET_OS_MAC

        #if TARGET_CPU_ARM64
            #define FREOD_PLATFORM_NAME "MacArm"
            #define FREOD_IS_ARM_MAC
            #define FREOD_PLATFORM_SHORT_IDENTIFIER "mac-arm"
            #define FREOD_ARM_MAC(...) __VA_ARGS__
            #define FREOD_INTEL_MAC(...)
        #else
            #define FREOD_PLATFORM_NAME "MacIntel"
            #define FREOD_IS_INTEL_MAC
            #define FREOD_PLATFORM_SHORT_IDENTIFIER "mac-intel"
            #define FREOD_ARM_MAC(...)
            #define FREOD_INTEL_MAC(...) __VA_ARGS__
        #endif
    #endif
    #define FREOD_CALL
#else
    #define FREOD_MACOS(...)
    #define FREOD_IOS(...)
    #define FREOD_INTEL_MAC(...)
    #define FREOD_ARM_MAC(...)
#endif

// Android
#if defined(__ANDROID__)
    #define FREOD_ANDROID(...) __VA_ARGS__
    #define FREOD_MOBILE(...) __VA_ARGS__
    #define FREOD_DESKTOP(...)
    #define FREOD_IS_ANDROID
    #define FREOD_IS_MOBILE
    #define FREOD_CALL
    #define CC_TARGET_OS_ANDROID
    #define FREOD_PLATFORM_SHORT_IDENTIFIER_NOARCH "android"

    #if defined(__arm__)
        #define FREOD_ANDROID32(...) __VA_ARGS__
		#define FREOD_ANDROID64(...) 
        #define FREOD_IS_ANDROID32
		#define FREOD_PLATFORM_NAME "Android32"
        #define FREOD_PLATFORM_EXTENSION ".android32.so"
        #define FREOD_PLATFORM_SHORT_IDENTIFIER "android32"
	#elif defined(__aarch64__)
        #define FREOD_ANDROID32(...)
		#define FREOD_ANDROID64(...) __VA_ARGS__
        #define FREOD_IS_ANDROID64
        #define FREOD_PLATFORM_NAME "Android64"
        #define FREOD_PLATFORM_EXTENSION ".android64.so"
        #define FREOD_PLATFORM_SHORT_IDENTIFIER "android64"
    #else
        #define FREOD_ANDROID32(...)
        #define FREOD_ANDROID64(...)
	#endif
#else
    #define FREOD_ANDROID(...)
    #define FREOD_ANDROID32(...)
    #define FREOD_ANDROID64(...)
#endif

#ifndef FREOD_PLATFORM_NAME
    #error "Unsupported PlatformID!"
#endif

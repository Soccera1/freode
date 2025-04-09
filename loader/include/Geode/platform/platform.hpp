#pragma once

#include "cplatform.h"
#include <string>
#include <functional>
#include <memory>

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
    #define FREOD_PRETTY_FUNCTION std::string(__FUNCSIG__)
#else
    #define FREOD_PRETTY_FUNCTION std::string(__PRETTY_FUNCTION__)
#endif

// Windows
#ifdef FREOD_IS_WINDOWS

    #define FREOD_HIDDEN
    #define FREOD_INLINE __forceinline
    #define FREOD_VIRTUAL_CONSTEXPR
    #define FREOD_NOINLINE __declspec(noinline)

    #ifdef FREOD_EXPORTING
        #define FREOD_DLL __declspec(dllexport)
    #else
        #define FREOD_DLL __declspec(dllimport)
    #endif

    #define FREOD_API extern "C" __declspec(dllexport)
    #define FREOD_EXPORT __declspec(dllexport)

    #if defined(FREOD_IS_WINDOWS64)
        #define FREOD_IS_X64
        #define FREOD_CDECL_CALL
    #else 
        #define FREOD_IS_X86
        #define FREOD_CDECL_CALL __cdecl
	#endif

    #include "windows.hpp"

#elif defined(FREOD_IS_MACOS)

    #define FREOD_HIDDEN __attribute__((visibility("hidden")))
    #define FREOD_INLINE inline __attribute__((always_inline))
    #define FREOD_VIRTUAL_CONSTEXPR constexpr
    #define FREOD_NOINLINE __attribute__((noinline))

    #ifdef FREOD_EXPORTING
        #define FREOD_DLL __attribute__((visibility("default")))
    #else
        #define FREOD_DLL
    #endif

    #define FREOD_API extern "C" __attribute__((visibility("default")))
    #define FREOD_EXPORT __attribute__((visibility("default")))

    #define FREOD_IS_X64
    #define FREOD_CDECL_CALL

    #include "macos.hpp"

#elif defined(FREOD_IS_IOS)

    #define FREOD_HIDDEN __attribute__((visibility("hidden")))
    #define FREOD_INLINE inline __attribute__((always_inline))
    #define FREOD_VIRTUAL_CONSTEXPR constexpr
    #define FREOD_NOINLINE __attribute__((noinline))

    #ifdef FREOD_EXPORTING
        #define FREOD_DLL __attribute__((visibility("default")))
    #else
        #define FREOD_DLL
    #endif

    #define FREOD_API extern "C" __attribute__((visibility("default")))
    #define FREOD_EXPORT __attribute__((visibility("default")))

    #define FREOD_IS_X64
    #define FREOD_CDECL_CALL

    #include "ios.hpp"

#elif defined(FREOD_IS_ANDROID)

    #define FREOD_HIDDEN __attribute__((visibility("hidden")))
    #define FREOD_INLINE inline __attribute__((always_inline))
    #define FREOD_VIRTUAL_CONSTEXPR constexpr
    #define FREOD_NOINLINE __attribute__((noinline))

    #ifdef FREOD_EXPORTING
        #define FREOD_DLL __attribute__((visibility("default")))
    #else
        #define FREOD_DLL
    #endif

    #define FREOD_API extern "C" __attribute__((visibility("default")))
    #define FREOD_EXPORT __attribute__((visibility("default")))

    #if defined(FREOD_IS_ANDROID64)
        #define FREOD_IS_X64
    #else 
        #define FREOD_IS_X86
    #endif
    #define FREOD_CDECL_CALL

    #include "android.hpp"

#else

    #error "Unsupported Platform!"

#endif

namespace freod {
    class PlatformID {
    public:
        enum {
            Unknown    = 0b000000,
            Windows    = 0b000001,
            Android32  = 0b000010,
            Android64  = 0b000100,
            MacIntel   = 0b001000,
            MacArm     = 0b010000,
            iOS        = 0b100000,
            Android    = Android32 | Android64,
            Mac        = MacIntel | MacArm,
            Apple      = Mac | iOS,
            X64        = MacIntel | Windows,
            X86        = Unknown,
            ArmV7      = Android32,
            ArmV8      = Android64 | MacArm | iOS,
            Desktop    = Windows | Mac,
            Mobile     = Android | iOS,
        };

        using Type = decltype(Unknown);

        Type m_value;

        constexpr PlatformID(Type t) {
            m_value = t;
        }

        constexpr PlatformID& operator=(Type t) {
            m_value = t;
            return *this;
        }

        constexpr bool operator==(int other) const {
            return m_value == other;
        }

        constexpr bool operator==(PlatformID const& other) const {
            return m_value == other.m_value;
        }

        constexpr bool operator<(PlatformID const& other) const {
            return m_value < other.m_value;
        }

        constexpr bool operator>(PlatformID const& other) const {
            return m_value > other.m_value;
        }

        constexpr operator int() const {
            return m_value;
        }

        /**
         * Parse string into PlatformID. String should be all-lowercase, for 
         * example "windows" or "linux"
         */
        static FREOD_DLL PlatformID from(const char* str);
        static FREOD_DLL PlatformID from(std::string const& str);

        /**
         * Determines if a given platform string "covers" the given platform.
         * For example, "android" is covered by Platform::Android32 and Platform::Android64.
         * Input string must follow the format in PlatformID::toShortString.
         */
        static FREOD_DLL bool coveredBy(const char* str, PlatformID t);
        static FREOD_DLL bool coveredBy(std::string const& str, PlatformID t);
        /**
         * Returns the list of platforms covered by this string name. For 
         * example, "android" would return both Android32 and Android64
         * todo in v5: deprecate this as the flagged version deals with this
         */
        static FREOD_DLL std::vector<PlatformID> getCovered(std::string_view str);

        // todo in v5: this does not need to be constexpr in the header. dllexport it
        static constexpr char const* toString(Type lp) {
            switch (lp) {
                case Unknown: return "Unknown";
                case Windows: return "Windows";
                case MacIntel: return "MacIntel";
                case MacArm: return "MacArm";
                case iOS: return "iOS";
                case Android32: return "Android32";
                case Android64: return "Android64";
                default: break;
            }
            return "Undefined";
        }

        // todo in v5: this does not need to be constexpr in the header. dllexport it
        static constexpr char const* toShortString(Type lp, bool ignoreArch = false) {
            switch (lp) {
                case Unknown: return "unknown";
                case Windows: return "win";
                case MacIntel: return ignoreArch ? "mac" : "mac-intel";
                case MacArm: return ignoreArch ? "mac" : "mac-arm";
                case iOS: return "ios";
                case Android32: return ignoreArch ? "android" : "android32";
                case Android64: return ignoreArch ? "android" : "android64";
                default: break;
            }
            return "undefined";
        }

        template <class T>
            requires requires(T t) {
                static_cast<Type>(t);
            }
        constexpr static PlatformID from(T t) {
            return static_cast<Type>(t);
        }

        template <class T>
            requires requires(Type t) {
                static_cast<T>(t);
            }
        constexpr T to() const {
            return static_cast<T>(m_value);
        }
    };
}

namespace std {
    template <>
    struct hash<freod::PlatformID> {
        inline std::size_t operator()(freod::PlatformID const& id) const {
            return std::hash<freod::PlatformID::Type>()(id.m_value);
        }
    };
}

#ifdef FREOD_IS_WINDOWS
    #define FREOD_PLATFORM_TARGET PlatformID::Windows
#elif defined(FREOD_IS_ARM_MAC)
    #define FREOD_PLATFORM_TARGET PlatformID::MacArm
#elif defined(FREOD_IS_INTEL_MAC)
    #define FREOD_PLATFORM_TARGET PlatformID::MacIntel
#elif defined(FREOD_IS_IOS)
    #define FREOD_PLATFORM_TARGET PlatformID::iOS
#elif defined(FREOD_IS_ANDROID32)
    #define FREOD_PLATFORM_TARGET PlatformID::Android32
#elif defined(FREOD_IS_ANDROID64)
    #define FREOD_PLATFORM_TARGET PlatformID::Android64
#endif

// this is cross-platform so not duplicating it across the typeinfo_cast definitions
namespace freod::cast {
    template<class T, class U>
    std::shared_ptr<T> typeinfo_pointer_cast(std::shared_ptr<U> const& r) noexcept {
        // https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast
        auto p = typeinfo_cast<typename std::shared_ptr<T>::element_type*>(r.get());
        return std::shared_ptr<T>(r, p);
    }
}

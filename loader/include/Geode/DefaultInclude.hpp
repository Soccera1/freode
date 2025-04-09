#pragma once

#include <Freod/Prelude.hpp>
#include <Freod/c++stl/gdstdlib.hpp>
#include <Freod/platform/platform.hpp>
#include <variant>

#if defined(FREOD_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE)
    #if !defined(__clang__)
        #error Freod Loader only compiles with Clang.
    #endif 
#endif

#if !defined(FREOD_CONCAT)
    #define FREOD_WRAPPER_CONCAT(x, y) x##y
    #define FREOD_CONCAT(x, y) FREOD_WRAPPER_CONCAT(x, y)
#endif

#define FREOD_WRAPPER_STR(...) #__VA_ARGS__
#define FREOD_STR(...) FREOD_WRAPPER_STR(__VA_ARGS__)

#define FREOD_PAD(size) uint8_t FREOD_CONCAT(__pad, __LINE__)[size]
#define FREOD_UNIMPLEMENTED_PAD private:

#define FREOD_NONINHERITED_MEMBERS private:

#define FREOD_EXPAND(x) x
#define FREOD_INVOKE(macro, ...) FREOD_EXPAND(macro(__VA_ARGS__))

namespace freod {
    struct ZeroConstructorType {};

    static constexpr auto ZeroConstructor = ZeroConstructorType();

    struct CutoffConstructorType {};

    static constexpr auto CutoffConstructor = CutoffConstructorType();
}

#define FREOD_CUSTOM_CONSTRUCTOR_BEGIN(Class_) \
    FREOD_ZERO_CONSTRUCTOR_BEGIN(Class_)       \
    FREOD_CUTOFF_CONSTRUCTOR_BEGIN(Class_)

#define FREOD_CUSTOM_CONSTRUCTOR_COCOS(Class_, Base_) \
    FREOD_ZERO_CONSTRUCTOR(Class_, Base_)             \
    FREOD_CUTOFF_CONSTRUCTOR_COCOS(Class_, Base_)

#define FREOD_CUSTOM_CONSTRUCTOR_GD(Class_, Base_) \
    FREOD_ZERO_CONSTRUCTOR(Class_, Base_)          \
    FREOD_CUTOFF_CONSTRUCTOR_GD(Class_, Base_)

#define FREOD_CUSTOM_CONSTRUCTOR_CUTOFF(Class_, Base_) \
    FREOD_ZERO_CONSTRUCTOR(Class_, Base_)              \
    FREOD_CUTOFF_CONSTRUCTOR_CUTOFF(Class_, Base_)

#define FREOD_ZERO_CONSTRUCTOR_BEGIN(Class_)                                              \
    Class_(freod::ZeroConstructorType, void*) {}                                          \
    Class_(freod::ZeroConstructorType, size_t fill) :                                     \
        Class_(freod::ZeroConstructor, std::memset(static_cast<void*>(this), 0, fill)) {} \
    Class_(freod::ZeroConstructorType) : Class_(freod::ZeroConstructor, nullptr) {}

#define FREOD_ZERO_CONSTRUCTOR(Class_, Base_)                                                \
    Class_(freod::ZeroConstructorType, size_t fill) : Base_(freod::ZeroConstructor, fill) {} \
    Class_(freod::ZeroConstructorType) : Base_(freod::ZeroConstructor, sizeof(Class_)) {}

#define FREOD_FILL_CONSTRUCTOR(Class_, Offset_)                                          \
    Class_(freod::CutoffConstructorType, size_t fill) :                                  \
        Class_(                                                                          \
            freod::CutoffConstructor,                                                    \
            std::memset(reinterpret_cast<std::byte*>(this) + Offset_, 0, fill - Offset_) \
        ) {}                                                                             \
    Class_(freod::CutoffConstructorType, void*)

#define FREOD_CUTOFF_CONSTRUCTOR_BEGIN(Class_)                      \
    FREOD_MACOS(FREOD_FILL_CONSTRUCTOR(Class_, 0){})                \
    FREOD_IOS(FREOD_FILL_CONSTRUCTOR(Class_, 0){})                  \
    FREOD_WINDOWS(Class_(freod::CutoffConstructorType, size_t fill) \
                  : Class_() {})                                    \
    FREOD_ANDROID(FREOD_FILL_CONSTRUCTOR(Class_, 0){})

#define FREOD_CUTOFF_CONSTRUCTOR_COCOS(Class_, Base_)               \
    FREOD_MACOS(Class_(freod::CutoffConstructorType, size_t fill)   \
                : Base_(freod::CutoffConstructor, fill){})          \
    FREOD_IOS(Class_(freod::CutoffConstructorType, size_t fill)     \
              : Base_(freod::CutoffConstructor, fill){})            \
    FREOD_WINDOWS(Class_(freod::CutoffConstructorType, size_t fill) \
                  : Class_() {})                                    \
    FREOD_ANDROID(Class_(freod::CutoffConstructorType, size_t fill)   \
                : Base_(freod::CutoffConstructor, fill){})

#define FREOD_CUTOFF_CONSTRUCTOR_GD(Class_, Base_)                  \
    FREOD_WINDOWS(Class_(freod::CutoffConstructorType, size_t fill) \
                  : Base_(freod::CutoffConstructor, fill){})        \
    FREOD_MACOS(Class_(freod::CutoffConstructorType, size_t fill)   \
                : Base_(freod::CutoffConstructor, fill){})          \
    FREOD_IOS(Class_(freod::CutoffConstructorType, size_t fill)     \
              : Base_(freod::CutoffConstructor, fill){})            \
    FREOD_ANDROID(Class_(freod::CutoffConstructorType, size_t fill) \
              : Base_(freod::CutoffConstructor, fill){})

#define FREOD_CUTOFF_CONSTRUCTOR_CUTOFF(Class_, Base_)                       \
    FREOD_WINDOWS(FREOD_FILL_CONSTRUCTOR(Class_, sizeof(Base_)) : Base_(){}) \
    FREOD_ANDROID(Class_(freod::CutoffConstructorType, size_t fill)          \
                : Base_(freod::CutoffConstructor, fill){})                   \
    FREOD_MACOS(Class_(freod::CutoffConstructorType, size_t fill)            \
                : Base_(freod::CutoffConstructor, fill){})                   \
    FREOD_IOS(Class_(freod::CutoffConstructorType, size_t fill)              \
              : Base_(freod::CutoffConstructor, fill){})

#define FREOD_NUMBER_OF_ARGS(...) \
    FREOD_EXPAND(FREOD_NUMBER_OF_ARGS_(__VA_ARGS__, FREOD_NUMBER_SEQUENCE(), ))
#define FREOD_NUMBER_OF_ARGS_(...) FREOD_EXPAND(FREOD_NUMBER_OF_ARGS_N(__VA_ARGS__))
#define FREOD_NUMBER_OF_ARGS_N( \
    _1,                         \
    _2,                         \
    _3,                         \
    _4,                         \
    _5,                         \
    _6,                         \
    _7,                         \
    _8,                         \
    _9,                         \
    _10,                        \
    _11,                        \
    _12,                        \
    _13,                        \
    _14,                        \
    _15,                        \
    _16,                        \
    _17,                        \
    _18,                        \
    _19,                        \
    _20,                        \
    _21,                        \
    _22,                        \
    _23,                        \
    _24,                        \
    _25,                        \
    _26,                        \
    _27,                        \
    _28,                        \
    _29,                        \
    _30,                        \
    _31,                        \
    _32,                        \
    _33,                        \
    _34,                        \
    _35,                        \
    _36,                        \
    _37,                        \
    _38,                        \
    _39,                        \
    _40,                        \
    _41,                        \
    _42,                        \
    _43,                        \
    _44,                        \
    _45,                        \
    _46,                        \
    _47,                        \
    _48,                        \
    _49,                        \
    _50,                        \
    _51,                        \
    _52,                        \
    _53,                        \
    _54,                        \
    _55,                        \
    _56,                        \
    _57,                        \
    _58,                        \
    _59,                        \
    _60,                        \
    _61,                        \
    _62,                        \
    _63,                        \
    N,                          \
    ...                         \
)                               \
    N
#define FREOD_NUMBER_SEQUENCE()                                                                 \
    63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, \
        40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, \
        18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define $execute                                                                                  \
    template <class>                                                                              \
    void FREOD_CONCAT(freodExecFunction, __LINE__)();                                             \
    namespace {                                                                                   \
        struct FREOD_CONCAT(ExecFuncUnique, __LINE__) {};                                         \
    }                                                                                             \
    static inline auto FREOD_CONCAT(Exec, __LINE__) =                                             \
        (FREOD_CONCAT(freodExecFunction, __LINE__) < FREOD_CONCAT(ExecFuncUnique, __LINE__) > (), \
         0);                                                                                      \
    template <class>                                                                              \
    void FREOD_CONCAT(freodExecFunction, __LINE__)()

#define FREOD_FORWARD_COMPAT_DISABLE_HOOKS_INNER(message) \
    if (Loader::get()->isForwardCompatMode()) {           \
        if (strlen(message)) {                            \
            log::warn("[Forward Compat] " message);       \
        }                                                 \
        for (const auto& [_, hook] : self.m_hooks) {      \
            hook->setAutoEnable(false);                   \
        }                                                 \
    }
#define FREOD_FORWARD_COMPAT_ENABLE_HOOKS_INNER(message)  \
    if (!Loader::get()->isForwardCompatMode()) {          \
        if (strlen(message)) {                            \
            log::warn("[Forward Compat] " message);       \
        }                                                 \
        for (const auto& [_, hook] : self.m_hooks) {      \
            hook->setAutoEnable(false);                   \
        }                                                 \
    }
#define FREOD_FORWARD_COMPAT_DISABLE_HOOKS(message)       \
    static void onModify(const auto& self) {              \
        FREOD_FORWARD_COMPAT_DISABLE_HOOKS_INNER(message) \
    }
#define FREOD_FORWARD_COMPAT_ENABLE_HOOKS(message)        \
    static void onModify(const auto& self) {              \
        FREOD_FORWARD_COMPAT_ENABLE_HOOKS_INNER(message)  \
    }

// #define FREOD_NEST1(macro, begin)           \
// macro(FREOD_CONCAT(begin, 0)),                        \
// macro(FREOD_CONCAT(begin, 1)),                        \
// macro(FREOD_CONCAT(begin, 2)),                        \
// macro(FREOD_CONCAT(begin, 3)),                        \
// macro(FREOD_CONCAT(begin, 4)),                        \
// macro(FREOD_CONCAT(begin, 5)),                        \
// macro(FREOD_CONCAT(begin, 6)),                        \
// macro(FREOD_CONCAT(begin, 7)),                        \
// macro(FREOD_CONCAT(begin, 8)),                        \
// macro(FREOD_CONCAT(begin, 9)),                        \
// macro(FREOD_CONCAT(begin, a)),                        \
// macro(FREOD_CONCAT(begin, b)),                        \
// macro(FREOD_CONCAT(begin, c)),                        \
// macro(FREOD_CONCAT(begin, d)),                        \
// macro(FREOD_CONCAT(begin, e)),                        \
// macro(FREOD_CONCAT(begin, f))

// #define FREOD_NEST2(macro, begin)           \
// FREOD_NEST1(macro, FREOD_CONCAT(begin, 0)), \
// FREOD_NEST1(macro, FREOD_CONCAT(begin, 1)), \
// FREOD_NEST1(macro, FREOD_CONCAT(begin, 2)), \
// FREOD_NEST1(macro, FREOD_CONCAT(begin, 3)), \
// FREOD_NEST1(macro, FREOD_CONCAT(begin, 4)), \
// FREOD_NEST1(macro, FREOD_CONCAT(begin, 5)), \
// FREOD_NEST1(macro, FREOD_CONCAT(begin, 6)), \
// FREOD_NEST1(macro, FREOD_CONCAT(begin, 7)), \
// FREOD_NEST1(macro, FREOD_CONCAT(begin, 8)), \
// FREOD_NEST1(macro, FREOD_CONCAT(begin, 9)), \
// FREOD_NEST1(macro, FREOD_CONCAT(begin, a)), \
// FREOD_NEST1(macro, FREOD_CONCAT(begin, b)), \
// FREOD_NEST1(macro, FREOD_CONCAT(begin, c)), \
// FREOD_NEST1(macro, FREOD_CONCAT(begin, d)), \
// FREOD_NEST1(macro, FREOD_CONCAT(begin, e)), \
// FREOD_NEST1(macro, FREOD_CONCAT(begin, f))

// #define FREOD_NEST3(macro, begin)           \
// FREOD_NEST2(macro, FREOD_CONCAT(begin, 0)), \
// FREOD_NEST2(macro, FREOD_CONCAT(begin, 1)), \
// FREOD_NEST2(macro, FREOD_CONCAT(begin, 2)), \
// FREOD_NEST2(macro, FREOD_CONCAT(begin, 3)), \
// FREOD_NEST2(macro, FREOD_CONCAT(begin, 4)), \
// FREOD_NEST2(macro, FREOD_CONCAT(begin, 5)), \
// FREOD_NEST2(macro, FREOD_CONCAT(begin, 6)), \
// FREOD_NEST2(macro, FREOD_CONCAT(begin, 7)), \
// FREOD_NEST2(macro, FREOD_CONCAT(begin, 8)), \
// FREOD_NEST2(macro, FREOD_CONCAT(begin, 9)), \
// FREOD_NEST2(macro, FREOD_CONCAT(begin, a)), \
// FREOD_NEST2(macro, FREOD_CONCAT(begin, b)), \
// FREOD_NEST2(macro, FREOD_CONCAT(begin, c)), \
// FREOD_NEST2(macro, FREOD_CONCAT(begin, d)), \
// FREOD_NEST2(macro, FREOD_CONCAT(begin, e)), \
// FREOD_NEST2(macro, FREOD_CONCAT(begin, f))

// #define FREOD_NEST4(macro, begin)           \
// FREOD_NEST3(macro, FREOD_CONCAT(begin, 0)), \
// FREOD_NEST3(macro, FREOD_CONCAT(begin, 1)), \
// FREOD_NEST3(macro, FREOD_CONCAT(begin, 2)), \
// FREOD_NEST3(macro, FREOD_CONCAT(begin, 3)), \
// FREOD_NEST3(macro, FREOD_CONCAT(begin, 4)), \
// FREOD_NEST3(macro, FREOD_CONCAT(begin, 5)), \
// FREOD_NEST3(macro, FREOD_CONCAT(begin, 6)), \
// FREOD_NEST3(macro, FREOD_CONCAT(begin, 7)), \
// FREOD_NEST3(macro, FREOD_CONCAT(begin, 8)), \
// FREOD_NEST3(macro, FREOD_CONCAT(begin, 9)), \
// FREOD_NEST3(macro, FREOD_CONCAT(begin, a)), \
// FREOD_NEST3(macro, FREOD_CONCAT(begin, b)), \
// FREOD_NEST3(macro, FREOD_CONCAT(begin, c)), \
// FREOD_NEST3(macro, FREOD_CONCAT(begin, d)), \
// FREOD_NEST3(macro, FREOD_CONCAT(begin, e)), \
// FREOD_NEST3(macro, FREOD_CONCAT(begin, f))

// #define FREOD_ENUM_OFFSETS_DEFINE(hex) FREOD_CONCAT($, hex)
// #define FREOD_ENUM_OFFSETS_SET() FREOD_NEST4(FREOD_ENUM_OFFSETS_DEFINE, 0x)

// enum class PrinterOffsets {
//    FREOD_ENUM_OFFSETS_SET()
// };

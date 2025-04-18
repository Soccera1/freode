#pragma once
#include "../utils/addresser.hpp"
#include "Traits.hpp"
#include "../loader/Log.hpp"

namespace freod::modifier {
/**
 * A helper struct that generates a static function that calls the given function.
 */
#define FREOD_AS_STATIC_FUNCTION(FunctionName_)                                                   \
    template <class Class2, class FunctionType>                                                   \
    struct AsStaticFunction_##FunctionName_ {                                                     \
        template <class FunctionType2>                                                            \
        struct Impl {};                                                                           \
        template <class Return, class... Params>                                                  \
        struct Impl<Return (*)(Params...)> {                                                      \
            static Return FREOD_CDECL_CALL function(Params... params) {                           \
                return Class2::FunctionName_(params...);                                          \
            }                                                                                     \
        };                                                                                        \
        template <class Return, class Class, class... Params>                                     \
        struct Impl<Return (Class::*)(Params...)> {                                               \
            static Return FREOD_CDECL_CALL function(Class* self, Params... params) {              \
                auto self2 = addresser::rthunkAdjust(                                             \
                    Resolve<Params...>::func(&Class2::FunctionName_), self                        \
                );                                                                                \
                return self2->Class2::FunctionName_(params...);                                   \
            }                                                                                     \
        };                                                                                        \
        template <class Return, class Class, class... Params>                                     \
        struct Impl<Return (Class::*)(Params...) const> {                                         \
            static Return FREOD_CDECL_CALL function(Class const* self, Params... params) {        \
                auto self2 = addresser::rthunkAdjust(                                             \
                    Resolve<Params...>::func(&Class2::FunctionName_), self                        \
                );                                                                                \
                return self2->Class2::FunctionName_(params...);                                   \
            }                                                                                     \
        };                                                                                        \
        static constexpr auto value = &Impl<FunctionType>::function;                              \
    };

    FREOD_AS_STATIC_FUNCTION(constructor)
    FREOD_AS_STATIC_FUNCTION(destructor)


#define FREOD_CONCEPT_FUNCTION_CHECK(FunctionName_)                                              \
    template <class Class, class... Args>                                                        \
    concept FunctionExists_##FunctionName_ = requires(Class* self, Args... args) {               \
        self->FunctionName_(args...);                                                            \
    };
}

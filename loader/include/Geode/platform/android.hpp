#pragma once

#include <dlfcn.h>
#include <type_traits>
#include "ItaniumCast.hpp"

namespace freod {
    struct PlatformInfo {
        void* m_so;
    };
}

namespace freod::base {
    /*FREOD_NOINLINE inline*/ uintptr_t get();
}


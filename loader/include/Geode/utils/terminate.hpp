#pragma once

#include "../DefaultInclude.hpp"
#include <exception>

namespace freod {
    class Mod;
    Mod* getMod();
}

namespace freod::utils {
#ifdef FREOD_IS_WINDOWS
    static constexpr size_t FREOD_TERMINATE_EXCEPTION_CODE   = 0x4000;
    static constexpr size_t FREOD_UNREACHABLE_EXCEPTION_CODE = 0x4001;

    static constexpr bool isFreodExceptionCode(size_t code) {
        return FREOD_TERMINATE_EXCEPTION_CODE <= code && code <= FREOD_UNREACHABLE_EXCEPTION_CODE;
    }
#else
    static constexpr size_t FREOD_TERMINATE_EXCEPTION_CODE = 0;
    static constexpr size_t FREOD_UNREACHABLE_EXCEPTION_CODE = 0;

    static constexpr bool isFreodExceptionCode(size_t code) {
        return false;
    }
#endif

    namespace detail {
        // This needs to do stuff with `Mod*` which is not included in the file
        FREOD_DLL void logTerminationError(const char* reason, Mod* mod);
    }

    template <class = void>
    [[noreturn]]
    void terminate(std::string const& reason, Mod* mod = getMod(), size_t platformCode = FREOD_TERMINATE_EXCEPTION_CODE) {
        // Add the error to the logfile
        detail::logTerminationError(reason.c_str(), mod);

    #ifdef FREOD_IS_WINDOWS
        // If a debugger is attached, start debugging
        if (IsDebuggerPresent()) {
            OutputDebugStringA(reason.c_str());
            DebugBreak();
        }
        // Otherwise terminate by raising an exception (which is caught by the crashlog handler)
        else {
            std::array<const void*, 2> errorList { static_cast<const void*>(reason.c_str()), mod };
            RaiseException(
                platformCode,
                EXCEPTION_NONCONTINUABLE,
                2, reinterpret_cast<ULONG_PTR*>(errorList.data())
            );
        }
    #endif
        
        std::terminate();
    }
    
    template <class = void>
    [[noreturn]]
    void unreachable(std::string const& reason = "Unspecified", Mod* mod = getMod()) {
        terminate(reason, mod, FREOD_UNREACHABLE_EXCEPTION_CODE);
    }
}

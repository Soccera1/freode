#pragma once

#include <Freod/platform/platform.hpp>
#include <tulip/TulipHook.hpp>
#include "../Prelude.hpp"

namespace freod::hook {
    /**
     * Create a calling convention wrapper for a function.
     */
    FREOD_DLL Result<void*> createWrapper(
        void* address,
        tulip::hook::WrapperMetadata const& metadata
    ) noexcept;

    /**
     * Create an abstract calling convention handler for TulipHook
     */
    FREOD_DLL std::shared_ptr<tulip::hook::CallingConvention> createConvention(
        tulip::hook::TulipConvention convention
    ) noexcept;
}

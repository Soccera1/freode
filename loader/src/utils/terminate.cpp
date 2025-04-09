#include <Freod/utils/terminate.hpp>
#include <Freod/loader/Mod.hpp>
#include "../loader/Log.hpp"

using namespace freod::prelude;

void freod::utils::detail::logTerminationError(const char* reason, Mod* mod) {
    // invoke logImpl directly to indicate what mod it came from
    log::logImpl(
        Severity::Error, mod,
        "The mod '{}' by {} has deliberately asked the game to crash.\nReason: {}",
        mod->getID(), fmt::format("{}", fmt::join(mod->getDevelopers(), ", ")), reason
    );
}


#include <Freod/loader/Tulip.hpp>

using namespace freod::prelude;

Result<void*> freod::hook::createWrapper(
    void* address,
    tulip::hook::WrapperMetadata const& metadata
) noexcept {
    return tulip::hook::createWrapper(address, metadata);
}

std::shared_ptr<tulip::hook::CallingConvention> freod::hook::createConvention(
    tulip::hook::TulipConvention convention
) noexcept {
    return tulip::hook::createConvention(convention);
}

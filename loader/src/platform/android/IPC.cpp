#include <Freod/DefaultInclude.hpp>
#include <loader/IPC.hpp>
#include <Freod/loader/Log.hpp>

using namespace freod::prelude;

void ipc::setup() {
    log::debug("IPC is not supported on this platform!");
}

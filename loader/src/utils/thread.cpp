#include <Freod/DefaultInclude.hpp>

using namespace freod::prelude;

#include <Freod/loader/Loader.hpp> // i don't think i have to label these anymore
#include <Freod/Utils.hpp>
#include "thread.hpp"

static thread_local std::string s_threadName;

std::string freod::utils::thread::getName() {
    // only use the thread-local variable here, no need for platform get methods
    if (s_threadName.empty())
        return getDefaultName();
    return s_threadName;
}

void freod::utils::thread::setName(std::string const& name) {
    s_threadName = name;
    platformSetName(name);
}

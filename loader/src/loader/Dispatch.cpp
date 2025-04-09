#include <Freod/loader/Dispatch.hpp>

using namespace freod::prelude;

std::unordered_map<std::string, EventListenerPool*>& freod::dispatchPools() {
    static std::unordered_map<std::string, EventListenerPool*> pools;
    return pools;
}
#include <Freod/modify/IDManager.hpp>

using namespace freod;

NodeIDs* NodeIDs::get() {
    static auto inst = new NodeIDs;
    return inst;
}

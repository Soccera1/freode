#include <loader/LoaderImpl.hpp>

using namespace freod::prelude;

#include <Freod/modify/CCScheduler.hpp>

struct FunctionQueue : Modify<FunctionQueue, CCScheduler> {
    void update(float dt) {
        LoaderImpl::get()->executeMainThreadQueue();
        return CCScheduler::update(dt);
    }
};

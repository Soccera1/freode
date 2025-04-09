#include <Freod/Freod.hpp>
#include <Freod/modify/CCScene.hpp>

using namespace freod::prelude;

class $modify(CCScene) {
    int getHighestChildZ() {
        if(this->getChildrenCount() == 0) return 0;

        return CCScene::getHighestChildZ();      
    }
};
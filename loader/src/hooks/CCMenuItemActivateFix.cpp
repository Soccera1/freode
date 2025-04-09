#include <Freod/Freod.hpp>
#include <Freod/modify/CCMenuItem.hpp>
#include <Freod/modify/CCMenu.hpp>

using namespace freod::prelude;

class $modify(CCMenuItem) {
    void activate() {
        this->retain();
        CCMenuItem::activate();
        this->release();
    }
};

class $modify(CCMenu) {
    void ccTouchEnded(CCTouch* touch, CCEvent* event) {
        this->retain();
        CCMenu::ccTouchEnded(touch, event);
        this->release();
    }
};
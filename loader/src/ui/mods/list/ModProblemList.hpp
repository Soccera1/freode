#pragma once

#include <Freod/cocos/cocoa/CCGeometry.h>
#include <Freod/cocos/base_nodes/CCNode.h>
#include <Freod/loader/Loader.hpp>
#include <Freod/loader/Mod.hpp>
#include <Freod/ui/ScrollLayer.hpp>
#include <GUI/CCControlExtension/CCScale9Sprite.h>

using namespace freod::prelude;

class ModProblemList : public CCNode {
protected:
    Mod* m_mod;
    ScrollLayer* m_list;
    CCScale9Sprite* m_bg;
public:
    static ModProblemList* create(
        Mod* mod,
        CCSize const& size
    );
protected:
    bool init(
        Mod* mod,
        CCSize const& size
    );
};

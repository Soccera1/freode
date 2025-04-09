#pragma once

#include <Freod/cocos/cocoa/CCGeometry.h>
#include <Freod/cocos/base_nodes/CCNode.h>
#include <Freod/loader/Loader.hpp>
#include <Freod/loader/Mod.hpp>
#include <Freod/ui/ScrollLayer.hpp>
#include <GUI/CCControlExtension/CCScale9Sprite.h>
#include "ui/mods/popups/DevPopup.hpp"
#include "ui/mods/sources/ModSource.hpp"

using namespace freod::prelude;

class ModDeveloperList : public CCNode {
protected:
    ModSource m_source;
    ScrollLayer* m_list;
    CCScale9Sprite* m_bg;
public:
    static ModDeveloperList* create(
        DevListPopup* popup, 
        ModSource const& source,
        CCSize const& size
    );
protected:
    bool init(
        DevListPopup* popup, 
        ModSource const& source,
        CCSize const& size
    );
};

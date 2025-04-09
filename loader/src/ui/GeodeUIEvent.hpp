#pragma once

#include <Freod/ui/FreodUI.hpp>
#include "mods/popups/ModPopup.hpp"
#include "mods/list/ModItem.hpp"

using namespace freod::prelude;

class ModPopupUIEvent::Impl {
public:
    ModPopup* popup;

    Impl(ModPopup* popup)
      : popup(popup) {}
};

class ModItemUIEvent::Impl {
public:
    ModItem* item;

    Impl(ModItem* item)
      : item(item) {}
};

class ModLogoUIEvent::Impl {
public:
    CCNode* sprite;
    std::string modID;

    Impl(CCNode* sprite, std::string const& modID)
      : sprite(sprite), modID(modID) {}
};

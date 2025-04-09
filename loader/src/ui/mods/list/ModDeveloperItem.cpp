#include "ModDeveloperItem.hpp"

#include <Freod/cocos/base_nodes/CCNode.h>
#include <Freod/ui/Layout.hpp>
#include <Freod/cocos/cocoa/CCGeometry.h>
#include <Freod/cocos/label_nodes/CCLabelBMFont.h>
#include <Freod/cocos/platform/CCPlatformMacros.h>
#include <Freod/cocos/sprite_nodes/CCSprite.h>
#include <Freod/DefaultInclude.hpp>
#include <Freod/loader/Loader.hpp>
#include <Freod/loader/Log.hpp>
#include <Freod/loader/Mod.hpp>
#include <Freod/ui/TextArea.hpp>
#include <Freod/utils/cocos.hpp>
#include <Freod/utils/ColorProvider.hpp>
#include <GUI/CCControlExtension/CCScale9Sprite.h>
#include <ccTypes.h>
#include <fmt/core.h>
#include "ui/mods/list/ModDeveloperItem.hpp"
#include "../UpdateModListState.hpp"

bool ModDeveloperItem::init(
    DevListPopup* popup,
    std::string developer,
    CCSize const& size,
    std::optional<std::string> displayName,
    bool addMoreButton
) {
    if (!CCNode::init()) {
        return false;
    }
    m_popup = popup;

    this->setContentSize(size);
    this->setAnchorPoint({ 0.5f, 0.5f });

    ccColor4B bgColor = ColorProvider::get()->color("mod-developer-item-bg"_spr);

    m_bg = CCScale9Sprite::create("square02b_001.png");
    m_bg->setColor(to3B(bgColor));
    m_bg->setOpacity(bgColor.a);
    m_bg->setScale(.3f);
    m_bg->setContentSize(CCSize {
        size.width,
        size.height
    } / m_bg->getScale());
    this->addChildAtPosition(
        m_bg,
        Anchor::Center
    );

    auto label = CCLabelBMFont::create(
        displayName.has_value() ? displayName->c_str() : developer.c_str(),
        "bigFont.fnt"
    );
    
    // Left + Right + Space between
    constexpr float paddings = 30.0f;
    float calc = size.width - paddings;
    label->setWidth(calc);
    label->setScale(0.4f);
    label->setAnchorPoint({0.0f, 0.5f});

    this->addChildAtPosition(
        label,
        Anchor::Left,
        {5, 0}
    );

    if (addMoreButton) {
        auto menu = CCMenu::create();
        menu->setAnchorPoint({1.0f, 0.5f});

        auto more = createFreodButton("More");

        auto btn = CCMenuItemSpriteExtra::create(
            more, this, menu_selector(ModDeveloperItem::onMoreByThisDev)
        );
        btn->setUserObject(CCString::create(developer));
        menu->addChild(btn);
        menu->setContentSize({size.width/2, size.height});
        menu->setScale(0.6f);

        auto layout = RowLayout::create();
        layout->setDefaultScaleLimits(0.5f, 0.7f);
        layout->setAxisAlignment(AxisAlignment::End);
        layout->setAxisReverse(true);
        menu->setLayout(layout);

        this->addChildAtPosition(
            menu,
            Anchor::Right,
            {-3, 0}
        );
    }

    return true;
}

void ModDeveloperItem::onMoreByThisDev(CCObject* sender) {
    auto str = static_cast<CCString*>(static_cast<CCNode*>(sender)->getUserObject());
    UpdateModListStateEvent(UpdateWholeState(str->getCString())).post();
    m_popup->onClose(nullptr);
}

ModDeveloperItem* ModDeveloperItem::create(
    DevListPopup* popup,
    std::string developer,
    CCSize const& size,
    std::optional<std::string> displayName,
    bool addMoreButton
) {
    auto ret = new ModDeveloperItem();
    if (ret->init(popup, developer, size, displayName, addMoreButton)) {
        ret->autorelease();
        return ret;
    }
    
    delete ret;
    return nullptr;
}
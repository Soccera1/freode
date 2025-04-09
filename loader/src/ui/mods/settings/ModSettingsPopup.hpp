#pragma once

#include <Freod/loader/Setting.hpp>
#include <Freod/ui/Popup.hpp>
#include <Freod/utils/cocos.hpp>
#include <Freod/ui/ScrollLayer.hpp>
#include <Freod/ui/TextInput.hpp>

#include "../FreodStyle.hpp"

using namespace freod::prelude;

class ModSettingsPopup : public FreodPopup<Mod*> {
protected:
    Mod* m_mod;
    ScrollLayer* m_list;
    std::vector<Ref<SettingNode>> m_settings;
    CCMenu* m_applyMenu;
    CCMenuItemSpriteExtra* m_applyBtn;
    CCMenuItemSpriteExtra* m_restartBtn;
    ButtonSprite* m_applyBtnSpr;
    TextInput* m_searchInput;
    CCMenuItemSpriteExtra* m_searchClearBtn;
    EventListener<EventFilter<SettingNodeValueChangeEvent>> m_changeListener;

    bool setup(Mod* mod) override;
    void updateState(SettingNode* invoker = nullptr);
    bool hasUncommitted() const;
    void onClose(CCObject*) override;
    void onApply(CCObject*);
    void onRestart(CCObject*);
    void onResetAll(CCObject*);
    void onOpenSaveDirectory(CCObject*);
    void onOpenConfigDirectory(CCObject*);
    void onOpenPersistentDirectory(CCObject*);
    void onClearSearch(CCObject*);

public:
    static ModSettingsPopup* create(Mod* mod, bool forceDisableTheme = false);
};

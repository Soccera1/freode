#pragma once

#include <Freod/ui/Popup.hpp>
#include <Freod/ui/TextInput.hpp>
#include "../sources/ModListSource.hpp"
#include "../FreodStyle.hpp"
#include <server/Server.hpp>

using namespace freod::prelude;

class FiltersPopup : public FreodPopup<ModListSource*> {
protected:
    ModListSource* m_source;
    CCMenu* m_tagsMenu;
    std::unordered_set<std::string> m_selectedTags;
    EventListener<server::ServerRequest<std::vector<server::ServerTag>>> m_tagsListener;
    CCMenuItemToggler* m_enabledModsOnly = nullptr;
    TextInput* m_developerNameInput = nullptr;

    bool setup(ModListSource* src) override;
    void updateTags();

    void onLoadTags(typename server::ServerRequest<std::vector<server::ServerTag>>::Event* event);
    void onResetTags(CCObject*);
    void onResetDevName(CCObject*);
    void onSelectTag(CCObject* sender);
    void onClose(CCObject* sender) override;

public:
    static FiltersPopup* create(ModListSource* src);
};

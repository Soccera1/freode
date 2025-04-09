#pragma once

#include <Freod/ui/Popup.hpp>
#include "../sources/ModListSource.hpp"
#include "../FreodStyle.hpp"
#include <server/Server.hpp>

using namespace freod::prelude;

class SortPopup : public FreodPopup<ModListSource*> {
protected:
    ModListSource* m_source;
    std::vector<CCMenuItemToggler*> m_options;
    size_t m_selected;

    bool setup(ModListSource* src) override;

    void onClose(CCObject* sender) override;
    void onSelect(CCObject*);

public:
    static SortPopup* create(ModListSource* src);
};

#pragma once

#include "../FreodStyle.hpp"
#include "../sources/ModSource.hpp"
#include <server/Server.hpp>

class DevListPopup : public FreodPopup<ModSource const&> {
protected:
    ModSource m_source;
public:
    static DevListPopup* create(ModSource const&);
    void onClose(cocos2d::CCObject*);
private:
    bool setup(ModSource const&);
    void createList();
    void onMoreByThisDev(CCObject* sender);
};

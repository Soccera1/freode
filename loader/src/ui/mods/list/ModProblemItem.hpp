#pragma once

#include <Freod/cocos/base_nodes/CCNode.h>
#include <Freod/cocos/cocoa/CCGeometry.h>
#include <Freod/cocos/sprite_nodes/CCSprite.h>
#include <Freod/loader/Loader.hpp>
#include <GUI/CCControlExtension/CCScale9Sprite.h>

using namespace freod::prelude;

class ModProblemItem : public CCNode {
protected:
    enum class Severity {
        Error,
        Info
    };
    LoadProblem m_problem;
    Mod* m_source;
    CCScale9Sprite* m_bg;
    ModProblemItem::Severity m_severity;
public:
    static ModProblemItem* create(Mod* source, LoadProblem problem, CCSize const& size);
protected:
    bool init(Mod* source, LoadProblem problem, CCSize const& size);
    ModProblemItem::Severity computeSeverity();
    bool showFixButton();
    bool showInfoButton();
    void onInfo(CCObject*);
    CCSprite* createSeverityIcon();
    std::string createProblemMessage();
};

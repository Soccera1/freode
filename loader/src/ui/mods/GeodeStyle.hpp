#pragma once

#include <Freod/DefaultInclude.hpp>
#include <Freod/binding/ButtonSprite.hpp>
#include <Freod/ui/General.hpp>
#include <Freod/ui/IconButtonSprite.hpp>
#include <Freod/ui/BasedButtonSprite.hpp>
#include <Freod/ui/Popup.hpp>
#include <Freod/loader/Mod.hpp>
#include <server/Server.hpp>

using namespace freod::prelude;

enum class FreodPopupStyle {
    Default,
    Alt,
    Alt2,
};

bool isFreodTheme(bool forceDisableTheme = false);

template <class... Args>
class FreodPopup : public Popup<Args...> {
protected:
    bool m_forceDisableTheme = false;

    bool init(float width, float height, Args... args, FreodPopupStyle style = FreodPopupStyle::Default, bool forceDisableTheme = false) {
        m_forceDisableTheme = forceDisableTheme;
        const bool freodTheme = isFreodTheme(forceDisableTheme);
        const char* bg;
        switch (style) {
            default:
            case FreodPopupStyle::Default: bg = freodTheme ? "GE_square01.png"_spr : "GJ_square01.png"; break;
            case FreodPopupStyle::Alt:     bg = freodTheme ? "GE_square02.png"_spr : "GJ_square02.png"; break;
            case FreodPopupStyle::Alt2:    bg = freodTheme ? "GE_square03.png"_spr : "GJ_square02.png"; break;
        }
        if (!Popup<Args...>::initAnchored(width, height, std::forward<Args>(args)..., bg))
            return false;
        
        this->setCloseButtonSpr(
            CircleButtonSprite::createWithSpriteFrameName(
                "close.png"_spr, .85f,
                (freodTheme ? 
                    (style == FreodPopupStyle::Default ? CircleBaseColor::DarkPurple : CircleBaseColor::DarkAqua) : 
                    CircleBaseColor::Green
                )
            )
        );

        return true;
    }
};

class FreodSquareSprite : public CCSprite {
protected:
    bool* m_stateSrc = nullptr;
    bool m_state = false;
    bool m_forceDisableTheme = false; 
    CCSprite* m_topSprite;

    bool init(CCSprite* top, bool* state, bool forceDisableTheme = false);

    void update(float dt) override;
    void updateImage();

public:
    static FreodSquareSprite* create(const char* top, bool* state = nullptr, bool forceDisableTheme = false);
    static FreodSquareSprite* createWithSpriteFrameName(const char* top, bool* state = nullptr, bool forceDisableTheme = false);

    CCSprite* getTopSprite() const;
    void setState(bool state);
};

CCNode* createLoadingCircle(float sideLength, const char* id = "loading-spinner");

enum class FreodButtonSprite {
    Default,
    Install,
    Delete,
    Enable,
    Gray,
};
const char* getFreodButtonSpriteName(FreodButtonSprite spr, bool forceDisableTheme = false);
IconButtonSprite* createFreodButton(CCNode* icon, std::string const& text, FreodButtonSprite bg = FreodButtonSprite::Default, bool forceDisableTheme = false);
ButtonSprite* createFreodButton(std::string const& text, int width, bool absolute = false, bool gold = false, FreodButtonSprite bg = FreodButtonSprite::Default, bool forceDisableTheme = false);
ButtonSprite* createFreodButton(std::string const& text, bool gold = false, FreodButtonSprite bg = FreodButtonSprite::Default, bool forceDisableTheme = false);

CircleButtonSprite* createFreodCircleButton(CCSprite* top, float scale = 1.f, CircleBaseSize size = CircleBaseSize::Medium, bool altColor = false, bool forceDisableTheme = false);

ButtonSprite* createTagLabel(std::string const& text, std::pair<ccColor3B, ccColor3B> const& color);
ButtonSprite* createFreodTagLabel(server::ServerTag const& tag);
std::pair<ccColor3B, ccColor3B> freodTagColors(server::ServerTag const& tag);

ListBorders* createFreodListBorders(CCSize const& size, bool forceDisableTheme = false);

class FreodTabSprite : public CCNode {
protected:
    CCScale9Sprite* m_deselectedBG;
    CCScale9Sprite* m_selectedBG;
    CCSprite* m_icon;
    CCLabelBMFont* m_label;

    bool init(const char* iconFrame, const char* text, float width, bool altColor);

public:
    static FreodTabSprite* create(const char* iconFrame, const char* text, float width, bool altColor = false);

    void select(bool selected);
    void disable(bool disabled);
};

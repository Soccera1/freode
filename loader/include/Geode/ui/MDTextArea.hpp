#pragma once

#include "ScrollLayer.hpp"
#include "TextRenderer.hpp"

#include <Freod/binding/FLAlertLayerProtocol.hpp>

struct MDParser;
class CCScrollLayerExt;

namespace freod {
    /**
     * TextArea for static markdown content. Supports the
     * following features:
     *  - Links
     *  - Images (sprites & spritesheets)
     *  - Headings
     *  - Paragraphs
     *  - Code blocks
     *  - Code spans
     *  - TextArea color tags (&lt;cr&gt;, &lt;cy&gt;, etc.)
     *  - Strikethrough
     *  - Underline
     *  - Bold & italic
     *  - Horizontal rules
     *  - Lists
     * 
     * Note that links also have some special protocols.
     * Use `user:<accountID>` to link to a GD
     * account; `level:<id>` to link to a GD level and
     * `mod:<id>` to link to another Freod mod.
     */
    class FREOD_DLL MDTextArea :
        public cocos2d::CCLayer,
        public cocos2d::CCLabelProtocol,
        public FLAlertLayerProtocol {
    protected:
        std::string m_text;
        cocos2d::CCSize m_size;
        cocos2d::extension::CCScale9Sprite* m_bgSprite = nullptr;
        cocos2d::CCMenu* m_content = nullptr;
        CCScrollLayerExt* m_scrollLayer = nullptr;
        TextRenderer* m_renderer = nullptr;

        bool init(std::string const& str, cocos2d::CCSize const& size);

        virtual ~MDTextArea();

        void onLink(CCObject*);
        void onGDProfile(CCObject*);
        void onGDLevel(CCObject*);
        void onFreodMod(CCObject*);
        void FLAlert_Clicked(FLAlertLayer*, bool btn) override;

        friend struct ::MDParser;

    public:
        /**
         * Create a markdown text area. See class
         * documentation for details on supported
         * features & notes.
         * @param str String to render
         * @param size Size of the textarea
         */
        static MDTextArea* create(std::string const& str, cocos2d::CCSize const& size);

        /**
         * Update the label's content; call
         * sparingly as rendering may be slow
         */
        void updateLabel();

        void setString(char const* text) override;
        char const* getString() override;

        CCScrollLayerExt* getScrollLayer() const;
    };
}

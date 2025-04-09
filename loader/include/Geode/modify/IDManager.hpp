#pragma once

#include "../DefaultInclude.hpp"
#include "../ui/EnterLayerEvent.hpp"

namespace cocos2d {
    class CCNode;
}

namespace freod {
    template<class T>
    concept IDProvidable = std::is_base_of_v<cocos2d::CCNode, T> && requires {
        { T::CLASS_NAME } -> std::convertible_to<const char*>;
    };

    class FREOD_DLL NodeIDs final {
    public:
        template<class T>
        using Provider = void(FREOD_CALL*)(T*);

    protected:
        std::unordered_map<std::string, Provider<cocos2d::CCNode>> m_providers;

    public:
        static NodeIDs* get();

        template<IDProvidable T>
        void registerProvider(void(FREOD_CALL* fun)(T*)) {
            m_providers.insert({
                T::CLASS_NAME,
                reinterpret_cast<Provider<cocos2d::CCNode>>(fun)
            });
        }

        template<IDProvidable T>
        bool provide(T* layer) const {
            if (m_providers.count(T::CLASS_NAME)) {
                m_providers.at(T::CLASS_NAME)(layer);
                return true;
            }
            return false;
        }

        // @note Because NodeIDs::provideFor(this) looks really neat
        template<IDProvidable T>
        static bool provideFor(T* layer) {
            return NodeIDs::get()->provide(layer);
        }
    };

    template<IDProvidable For>
    void FREOD_CALL freodInternalProvideIDsFor(For* cls) {
        if (cls->CCNode::getID() != For::CLASS_NAME) {
            cls->CCNode::setID(For::CLASS_NAME);
            cls->provide();
            EnterLayerEvent(For::CLASS_NAME, cls).post();
        }
    }
}

#define $register_ids(Layer_) \
    struct FREOD_CONCAT(ProvideIDsFor, Layer_) : public Layer_ {\
        void provide();\
    };\
	$execute {\
		NodeIDs::get()->registerProvider(\
            &freodInternalProvideIDsFor<FREOD_CONCAT(ProvideIDsFor, Layer_)>\
        );\
	};\
    void FREOD_CONCAT(ProvideIDsFor, Layer_)::provide() 

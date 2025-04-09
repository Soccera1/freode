#include <Freod/ui/SceneManager.hpp>
#include <Freod/modify/CCDirector.hpp>

using namespace freod::prelude;

#ifdef FREOD_IS_WINDOWS
#include <Freod/modify/AppDelegate.hpp>
#else
#include <Freod/modify/AchievementNotifier.hpp>
#endif

namespace freod {

#ifdef FREOD_IS_WINDOWS
struct SceneSwitch : Modify<SceneSwitch, AppDelegate> {
    FREOD_FORWARD_COMPAT_DISABLE_HOOKS("persist disabled")
    void willSwitchToScene(CCScene* scene) {
        AppDelegate::willSwitchToScene(scene);
        SceneManager::get()->willSwitchToScene(scene);
    }
};

#else
struct SceneSwitch : Modify<SceneSwitch, AchievementNotifier> {
    FREOD_FORWARD_COMPAT_DISABLE_HOOKS("persist disabled")
    void willSwitchToScene(CCScene* scene) {
        AchievementNotifier::willSwitchToScene(scene);
        SceneManager::get()->willSwitchToScene(scene);
    }
};

#endif

struct SceneSwitch2 : Modify<SceneSwitch2, CCDirector> {
    FREOD_FORWARD_COMPAT_DISABLE_HOOKS("persist disabled")
    // CCDirector does not call willSwitchToScene in these 2 instances,
    // so we have to do it ourselves to make everything behave as expected
    void popScene() {
        CCDirector::popScene();
    #ifdef FREOD_IS_WINDOWS
        AppDelegate::get()->willSwitchToScene(m_pNextScene);
    #else
        AchievementNotifier::sharedState()->willSwitchToScene(m_pNextScene);
    #endif
    }

    void popToSceneStackLevel(int level) {
        CCDirector::popToSceneStackLevel(level);
    #ifdef FREOD_IS_WINDOWS
        AppDelegate::get()->willSwitchToScene(m_pNextScene);
    #else
        AchievementNotifier::sharedState()->willSwitchToScene(m_pNextScene);
    #endif
    }
};

}
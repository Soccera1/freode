#include <Freod/modify/IDManager.hpp>
#include <Freod/utils/NodeIDs.hpp>
#include <Freod/binding/LoadingLayer.hpp>

using namespace freod::prelude;
using namespace freod::node_ids;

$register_ids(LoadingLayer) {
    setIDs(
        this, 0,
        "bg-texture",
        "gd-logo",
        "robtop-logo",
        "cocos2d-logo",
        "fmod-logo",
        "loading-text",
        "text-area",
        "progress-slider"
    );
}

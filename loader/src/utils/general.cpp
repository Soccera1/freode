#include <Freod/utils/general.hpp>

#ifndef FREOD_IS_MACOS
// feel free to properly implement this for other platforms
float freod::utils::getDisplayFactor() {
    return 1.0f;
}
#endif
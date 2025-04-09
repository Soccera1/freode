#pragma once

#include <Freod/platform/platform.hpp>

#include "string.hpp"

#if defined(FREOD_IS_ANDROID)
#include "gnustl.hpp"
#else
#include "aliastl.hpp"
#endif

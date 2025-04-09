#pragma once

#include <Freod/utils/VersionInfo.hpp>

namespace about {
    FREOD_DLL freod::VersionInfo getLoaderVersion();
    FREOD_DLL const char* getLoaderVersionStr();
    FREOD_DLL const char* getLoaderCommitHash();
    FREOD_DLL const char* getBindingsCommitHash();
    FREOD_DLL const char* getLoaderModJson();
};

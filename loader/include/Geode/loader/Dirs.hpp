#pragma once

#include <filesystem>
#include "../DefaultInclude.hpp"

namespace freod::dirs {
    /**
     * Directory where Geometry Dash is
     */
    FREOD_DLL std::filesystem::path getGameDir();
    /**
     * Directory where GD saves its files
     */
    FREOD_DLL std::filesystem::path getSaveDir();
    /**
     * Directory where Freod is
     */
    FREOD_DLL std::filesystem::path getFreodDir();
    /**
     * Directory where Freod saves its files
     */
    FREOD_DLL std::filesystem::path getFreodSaveDir();
    /**
     * Directory where Freod's resources are stored
     */
    FREOD_DLL std::filesystem::path getFreodResourcesDir();
    /**
     * Directory where Freod's resources are stored
     */
    FREOD_DLL std::filesystem::path getFreodLogDir();
    /**
     * Directory to store temporary files
     */
    FREOD_DLL std::filesystem::path getTempDir();
    /**
     * Directory where mods are stored by default
     */
    FREOD_DLL std::filesystem::path getModsDir();
    /**
     * Directory where mods' save data is stored
     */
    FREOD_DLL std::filesystem::path getModsSaveDir();
    /**
     * Directory where mods' unzipped packages are stored at runtime
     */
    FREOD_DLL std::filesystem::path getModRuntimeDir();
    /**
     * Directory where mods' config files lie
     */
    FREOD_DLL std::filesystem::path getModConfigDir();
    /**
     * Directory where Freod stores the cached index
     */
    FREOD_DLL std::filesystem::path getIndexDir();
    /**
     * Directory where crashlogs are stored
     */
    FREOD_DLL std::filesystem::path getCrashlogsDir();
    /**
     * Directory where mods' persistent files lie
     * This directory is not deleted even when Freod is uninstalled
     */
    FREOD_DLL std::filesystem::path getModPersistentDir();
}

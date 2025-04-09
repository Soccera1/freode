#pragma once

#include <Freod/DefaultInclude.hpp>
#include <string_view>

namespace freod::utils::permission {
    enum class Permission {
        ReadAllFiles = 0x3, // maintain compatibility with some older mods
        RecordAudio
    };

    /**
     * Request whether the given permission is granted to Freod by the operating system.
     * @param permission The permission
    */
    bool FREOD_DLL getPermissionStatus(Permission permission);

    /**
     * Request a permission to be granted by the operating system.
     * @param permission The permission
     * @param callback The callback, passed value is 'true' if permission was granted and 'false' otherwise.
     */
    void FREOD_DLL requestPermission(Permission permission, std::function<void(bool)> callback);
}
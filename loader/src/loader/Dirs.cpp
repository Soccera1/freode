
#include <Freod/loader/Dirs.hpp>
#include <cocos2d.h>
#include <crashlog.hpp>
#include <filesystem>
#include "LoaderImpl.hpp"

using namespace freod::prelude;

std::filesystem::path dirs::getFreodDir() {
    return dirs::getGameDir() / "freod";
}

std::filesystem::path dirs::getFreodSaveDir() {
    return dirs::getSaveDir() / "freod";
}

std::filesystem::path dirs::getFreodResourcesDir() {
    return dirs::getFreodDir() / "resources";
}

std::filesystem::path dirs::getFreodLogDir() {
    return dirs::getFreodDir() / "logs";
}

std::filesystem::path dirs::getTempDir() {
    return dirs::getFreodDir() / "temp";
}

std::filesystem::path dirs::getModsDir() {
    return dirs::getFreodDir() / "mods";
}

std::filesystem::path dirs::getModsSaveDir() {
    return dirs::getFreodSaveDir() / "mods";
}

std::filesystem::path dirs::getModConfigDir() {
    return dirs::getFreodDir() / "config";
}

std::filesystem::path dirs::getIndexDir() {
    return dirs::getFreodDir() / "index";
}

std::filesystem::path dirs::getCrashlogsDir() {
    return crashlog::getCrashLogDirectory();
}

std::filesystem::path dirs::getModPersistentDir() {
    return dirs::getSaveDir() / "freod-persistent";
}
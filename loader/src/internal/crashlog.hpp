#pragma once

#include <Freod/Loader.hpp>
#include <filesystem>
#include <string>

/**
 * Platform-specific crashlog functions. Used by the loader
 */
namespace crashlog {
    /**
     * Setup platform-specific crashlog handler
     * @returns True if the handler was successfully installed, false otherwise
     */
    bool FREOD_DLL setupPlatformHandler();

    /**
     * Setup platform-specific crashlog handler for post-launch
     */
    void FREOD_DLL setupPlatformHandlerPost();

    /**
     * Check if previous launch of GD crashed unexpectedly
     * @returns True if the launch crashed, false otherwise or if indeterminate
     */
    bool FREOD_DLL didLastLaunchCrash();
    /**
     * Get the directory where crashlogs are saved on the current platform
     * @returns Path to the directory, or an empty string if the platform does
     * not support crash logs
     */
    std::filesystem::path FREOD_DLL getCrashLogDirectory();

    std::string FREOD_DLL writeCrashlog(freod::Mod* faultyMod, std::string const& info, std::string const& stacktrace, std::string const& registers);

    std::string writeCrashlog(freod::Mod* faultyMod, std::string const& info, std::string const& stacktrace, std::string const& registers, std::filesystem::path& outCrashlogPath);

    std::string getDateString(bool filesafe);

    void FREOD_DLL printFreodInfo(std::stringstream& stream);
    void FREOD_DLL printMods(std::stringstream& stream);


}

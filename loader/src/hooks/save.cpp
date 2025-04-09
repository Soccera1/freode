#include <Freod/loader/Loader.hpp>

using namespace freod::prelude;

#include <Freod/modify/AppDelegate.hpp>
#include <Freod/modify/CCApplication.hpp>

namespace {
    void saveModData() {
        log::info("Saving mod data...");
        log::NestScope nest;

        auto begin = std::chrono::high_resolution_clock::now();

        (void)Loader::get()->saveData();

        auto end = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        log::info("Took {}s", static_cast<float>(time) / 1000.f);
    }
}

struct SaveLoader : Modify<SaveLoader, AppDelegate> {
    FREOD_FORWARD_COMPAT_DISABLE_HOOKS("save moved to CCApplication::gameDidSave()")
    void trySaveGame(bool p0) {
        saveModData();
        return AppDelegate::trySaveGame(p0);
    }
};

#ifdef FREOD_IS_WINDOWS

struct FallbackSaveLoader : Modify<FallbackSaveLoader, CCApplication> {
    FREOD_FORWARD_COMPAT_ENABLE_HOOKS("")
    void gameDidSave() {
        saveModData();
        return CCApplication::gameDidSave();
    }
};

#endif

#ifdef FREOD_IS_ANDROID

#include <Freod/modify/FileOperation.hpp>
#include <Freod/loader/Dirs.hpp>

// redirects the save path to what freod knows, in case launcher's fopen hook fails
struct FileOperationOverride : Modify<FileOperationOverride, FileOperation> {
    static gd::string getFilePath() {
        return dirs::getSaveDir().string() + "/";
    }
};

#endif

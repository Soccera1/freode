#include <Freod/DefaultInclude.hpp>

#include <Freod/loader/Mod.hpp>
#include <loader/ModImpl.hpp>
#include <dlfcn.h>

using namespace freod::prelude;

template <typename T>
T findSymbolOrMangled(void* dylib, char const* name, char const* mangled) {
    auto res = reinterpret_cast<T>(dlsym(dylib, name));
    if (!res) {
        res = reinterpret_cast<T>(dlsym(dylib, mangled));
    }
    return res;
}

Result<> Mod::Impl::loadPlatformBinary() {
    auto dylib =
        dlopen((m_tempDirName / m_metadata.getBinaryName()).string().c_str(), RTLD_LAZY);
    if (dylib) {
        if (m_platformInfo) {
            delete m_platformInfo;
        }
        m_platformInfo = new PlatformInfo { dylib };

        auto freodImplicitEntry = findSymbolOrMangled<void(*)()>(dylib, "freodImplicitEntry", "_Z17freodImplicitEntryv");
        if (freodImplicitEntry) {
            freodImplicitEntry();
        }

        auto freodCustomEntry = findSymbolOrMangled<void(*)()>(dylib, "freodCustomEntry", "_Z15freodCustomEntryv");
        if (freodCustomEntry) {
            freodCustomEntry();
        }

        return Ok();
    }
    std::string err = (char const*)dlerror();
    return Err("Unable to load the DYLIB: dlerror returned (" + err + ")");
}

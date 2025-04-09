#include <Freod/DefaultInclude.hpp>

#include <Freod/loader/Mod.hpp>
#include <loader/ModImpl.hpp>

using namespace freod::prelude;

template <typename T>
T findSymbolOrMangled(void* so, char const* name, char const* mangled) {
    auto res = reinterpret_cast<T>(dlsym(so, name));
    if (!res) {
        res = reinterpret_cast<T>(dlsym(so, mangled));
    }
    return res;
}

Result<> Mod::Impl::loadPlatformBinary() {
    auto so =
        dlopen((m_tempDirName / m_metadata.getBinaryName()).string().c_str(), RTLD_LAZY);
    if (so) {
        if (m_platformInfo) {
            delete m_platformInfo;
        }
        m_platformInfo = new PlatformInfo{so};

        auto freodImplicitEntry = findSymbolOrMangled<void(*)()>(so, "freodImplicitEntry", "_Z17freodImplicitEntryv");
        if (freodImplicitEntry) {
            freodImplicitEntry();
        }

        auto freodCustomEntry = findSymbolOrMangled<void(*)()>(so, "freodCustomEntry", "_Z15freodCustomEntryv");
        if (freodCustomEntry) {
            freodCustomEntry();
        }

        return Ok();
    }
    std::string err = dlerror();
    log::error("Unable to load the SO: dlerror returned ({})", err);
    return Err("Unable to load the SO: dlerror returned (" + err + ")");
}

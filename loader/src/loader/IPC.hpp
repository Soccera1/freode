#pragma once

#include <string>
#include <matjson.hpp>

namespace freod::ipc {
    void setup();
    matjson::Value processRaw(void* rawHandle, std::string const& buffer);
}

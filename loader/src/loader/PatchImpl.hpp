#pragma once

#include <Freod/loader/Hook.hpp>
#include <Freod/loader/Loader.hpp>
#include <Freod/loader/Mod.hpp>
#include "ModImpl.hpp"
#include "ModPatch.hpp"

using namespace freod::prelude;

class Patch::Impl final : ModPatch {
public:
    Impl(void* address, ByteVector original, ByteVector patch);
    ~Impl();

    static std::shared_ptr<Patch> create(void* address, const ByteVector& patch);
    static std::vector<Patch::Impl*>& allEnabled();

    Patch* m_self = nullptr;
    void* m_address;
    ByteVector m_original;
    ByteVector m_patch;

    Result<> enable();
    Result<> disable();

    ByteVector const& getBytes() const;
    Result<> updateBytes(const ByteVector& bytes);

    uintptr_t getAddress() const;
    matjson::Value getRuntimeInfo() const;

    friend class Patch;
    friend class Mod;
};

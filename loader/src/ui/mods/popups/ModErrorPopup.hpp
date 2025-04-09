#pragma once

#include <Freod/loader/Mod.hpp>

#include "ui/mods/FreodStyle.hpp"
#include "ui/mods/list/ModProblemList.hpp"

using namespace freod::prelude;

class ModErrorPopup : public FreodPopup<Mod*> {
protected:
    Mod* m_mod;
    ModProblemList* m_list;
public:
    static ModErrorPopup* create(Mod* mod);

private:
    bool setup(Mod* mod) override;

    void createList();
};

#pragma once

#include "Event.hpp"
#include <matjson.hpp>
#include <optional>

namespace freod {
    class Mod;
    Mod* getMod();

    enum class ModEventType {
        Loaded,
        DataLoaded,
        DataSaved,
    };

    /**
     * Event that is fired when a mod is loaded / unloaded / enabled / disabled
     */
    class FREOD_DLL ModStateEvent final : public Event {
    protected:
        ModEventType m_type;
        Mod* m_mod;

    public:
        ModStateEvent(Mod* mod, ModEventType type);
        ModEventType getType() const;
        Mod* getMod() const;
    };

    /**
     * Listener for mod load/enable/disable/unload/data save events
     */
    class FREOD_DLL ModStateFilter final : public EventFilter<ModStateEvent> {
    public:
        using Callback = void(ModStateEvent*);

    protected:
        ModEventType m_type;
        Mod* m_mod;

    public:
        ListenerResult handle(std::function<Callback> fn, ModStateEvent* event);

        /**
         * Create a mod state listener
         * @param mod The mod whose events to listen to, or nullptr to listen to 
         * all mods' all state events
         * @param type Type of event to listen to. Ignored if mod is nullptr
         */
        ModStateFilter(Mod* mod, ModEventType type);
        ModStateFilter(ModStateFilter const&) = default;
    };

    /**
     * Event posted to a mod when another mod that depends on it is loaded
     */
    class FREOD_DLL DependencyLoadedEvent final : public Event {
    protected:
        class Impl;
        std::unique_ptr<Impl> m_impl;
    
    public:
        DependencyLoadedEvent(Mod* target, Mod* dependency);
        virtual ~DependencyLoadedEvent();

        Mod* getTarget() const;
        Mod* getDependency() const;
        matjson::Value getDependencySettings() const;
    };

    /**
     * Listen for in a mod when a mod that depends on it is loaded
     */
    class FREOD_DLL DependencyLoadedFilter final : public EventFilter<DependencyLoadedEvent> {
    public:
        using Callback = void(DependencyLoadedEvent*);

    protected:
        class Impl;
        std::unique_ptr<Impl> m_impl;

    public:
        ListenerResult handle(std::function<Callback> fn, DependencyLoadedEvent* event);

        DependencyLoadedFilter(Mod* target = freod::getMod());
        DependencyLoadedFilter(DependencyLoadedFilter&&);
        DependencyLoadedFilter(DependencyLoadedFilter const&);
        DependencyLoadedFilter& operator=(DependencyLoadedFilter const&);
        DependencyLoadedFilter& operator=(DependencyLoadedFilter&&);
        ~DependencyLoadedFilter();
    };
}

// clang-format off
#define $on_mod(type) \
template<class>                                                        \
void FREOD_CONCAT(freodExecFunction, __LINE__)(freod::ModStateEvent*); \
namespace {                                                            \
	struct FREOD_CONCAT(ExecFuncUnique, __LINE__) {};                  \
}                                                                      \
static inline auto FREOD_CONCAT(Exec, __LINE__) = (new freod::EventListener(  \
            &FREOD_CONCAT(freodExecFunction, __LINE__)<FREOD_CONCAT(ExecFuncUnique, __LINE__)>, \
            freod::ModStateFilter(freod::getMod(), freod::ModEventType::type) \
        ), 0);                                                         \
template<class>                                                        \
void FREOD_CONCAT(freodExecFunction, __LINE__)(freod::ModStateEvent*)
// clang-format on

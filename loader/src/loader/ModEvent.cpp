#include <Geode/loader/ModEvent.hpp>

using namespace geode::prelude;

ModStateEvent::ModStateEvent(Mod* mod, ModEventType type) : m_mod(mod), m_type(type) {}

ModEventType ModStateEvent::getType() const {
    return m_type;
}

Mod* ModStateEvent::getMod() const {
    return m_mod;
}

namespace {
    EventListenerPool* getModEventPool(Mod* mod) {
        static std::unordered_map<Mod*, std::shared_ptr<DefaultEventListenerPool>> pools;
        if (pools.count(mod) == 0) {
            pools[mod] = DefaultEventListenerPool::create();
        }
        return pools[mod].get();
    }
}

EventListenerPool* ModStateEvent::getPool() const {
    return getModEventPool(m_mod);
}


ListenerResult ModStateFilter::handle(std::function<Callback> fn, ModStateEvent* event) {
    // log::debug("Event mod filter: {}, {}, {}, {}", m_mod, static_cast<int>(m_type), event->getMod(), static_cast<int>(event->getType()));
    if ((!m_mod || event->getMod() == m_mod) && event->getType() == m_type) {
        fn(event);
    }
    return ListenerResult::Propagate;
}

ModStateFilter::ModStateFilter(Mod* mod, ModEventType type) : m_mod(mod), m_type(type) {}

EventListenerPool* ModStateFilter::getPool() const {
    return getModEventPool(m_mod);
}
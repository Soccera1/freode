#pragma once

#include <Freod/loader/Event.hpp>
#include <Freod/loader/Dispatch.hpp>
#include <Freod/binding/GJGarageLayer.hpp>

using namespace freod::prelude;

#ifdef FREOD_IS_WINDOWS
    #ifdef EXPORTING_MOD
        #define FREOD_TESTDEP_DLL __declspec(dllexport)
    #else
        #define FREOD_TESTDEP_DLL __declspec(dllimport)
    #endif
#else
    #define FREOD_TESTDEP_DLL
#endif

#ifdef MY_MOD_ID
    #undef MY_MOD_ID
#endif
#define MY_MOD_ID "freod.testdep"

namespace api {
    // Important: The function must be declared inline, and return a freod::Result,
    // as it can fail if the api is not available.
    inline freod::Result<int> addNumbers(int a, int b) FREOD_EVENT_EXPORT(&addNumbers, (a, b));

    struct Test {
        freod::Result<int> addNumbers(int a, int b) FREOD_EVENT_EXPORT(&Test::addNumbers, (this, a, b));
    };
}

class FREOD_TESTDEP_DLL TestEvent : public Event {
protected:
    std::string data;

public:
    std::string getData() const;
    TestEvent(std::string const& data);
};

class FREOD_TESTDEP_DLL TestEventFilter : public EventFilter<TestEvent> {
public:
    using Callback = void(TestEvent*);

    ListenerResult handle(std::function<Callback> fn, TestEvent* event);
    TestEventFilter();
    TestEventFilter(TestEventFilter const&) = default;
};

using MyDispatchEvent = freod::DispatchEvent<GJGarageLayer*>;
using MyDispatchFilter = freod::DispatchFilter<GJGarageLayer*>;
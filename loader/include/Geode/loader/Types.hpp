#pragma once

#include "../DefaultInclude.hpp"
#include "../platform/cplatform.h"
#include <matjson.hpp>

#include <string>

namespace freod {
    /**
     * Describes the severity of the log
     * message.
     * @enum Severity
     */
    struct Severity {
        enum {
            /**
             * The message contains information
             * only relevant to the developer /
             * other mod developers.
             */
            Debug,

            /**
             * The message contains general
             * information about that logger's
             * state. The logger is still
             * working correctly.
             */
            Info,

            /**
             * The message contains information
             * about the logger's state that
             * is abnormal and may result in
             * errors if not handled properly.
             */
            Warning,

            /**
             * The message contains information
             * about a general error, such as
             * inability to read files.
             */
            Error
        };

        using type = decltype(Debug);

        type m_value;

        Severity(type t) {
            m_value = t;
        }

        Severity& operator=(type t) {
            m_value = t;
            return *this;
        }

        bool operator==(int other) const {
            return m_value == other;
        }

        bool operator==(Severity const& other) const {
            return m_value == other.m_value;
        }

        operator int() {
            return m_value;
        }

        template <class T>
        static type cast(T t) {
            return static_cast<type>(t);
        }

        static constexpr char const* toString(type lp) {
            switch (lp) {
                case Debug: return "Debug";
                case Info: return "Info";
                case Warning: return "Warning";
                case Error: return "Error";
            }
            return "Undefined";
        }
    };

    constexpr std::string_view FREOD_MOD_EXTENSION = ".freod";

    class Mod;
    class Loader;
    class Hook;
    class VersionInfo;

    class Unknown;

    namespace modifier {
        template <class, class>
        class FieldIntermediate;
    }

    using ModJson = matjson::Value;
}

/**
 * The predeclaration of the implicit entry
 */
FREOD_API void FREOD_CALL freod_implicit_load(freod::Mod*);

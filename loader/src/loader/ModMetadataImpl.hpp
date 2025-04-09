#pragma once

#include <Freod/loader/Loader.hpp>
#include <Freod/loader/Mod.hpp>
#include <Freod/utils/JsonValidation.hpp>
#include <Freod/utils/VersionInfo.hpp>
#include <Freod/loader/Setting.hpp>

using namespace freod::prelude;

namespace freod {
    class ModMetadataLinks::Impl final {
    public:
        std::optional<std::string> m_homepage;
        std::optional<std::string> m_source;
        std::optional<std::string> m_community;
    };

    class ModMetadata::Impl {
    public:
        std::filesystem::path m_path;
        std::string m_binaryName;
        VersionInfo m_version{1, 0, 0};
        std::string m_id;
        std::string m_name;
        std::vector<std::string> m_developers;
        // TODO: remove once #895 is fixed
        std::optional<std::string> m_softInvalidReason;
        std::string m_gdVersion;
        VersionInfo m_freodVersion;
        std::optional<std::string> m_description;
        std::optional<std::string> m_details;
        std::optional<std::string> m_changelog;
        std::optional<std::string> m_supportInfo;
        ModMetadataLinks m_links;
        std::optional<IssuesInfo> m_issues;
        std::vector<Dependency> m_dependencies;
        // todo in v5: make Dependency pimpl and move this as a member there (`matjson::Value settings;`)
        std::unordered_map<std::string, matjson::Value> m_dependencySettings;
        std::vector<Incompatibility> m_incompatibilities;
        std::vector<std::string> m_spritesheets;
        std::vector<std::pair<std::string, matjson::Value>> m_settings;
        std::unordered_set<std::string> m_tags;
        bool m_needsEarlyLoad = false;
        bool m_isAPI = false;

        ModJson m_rawJSON;

        static Result<ModMetadata> createFromFreodZip(utils::file::Unzip& zip);
        static Result<ModMetadata> createFromFreodFile(std::filesystem::path const& path);
        static Result<ModMetadata> createFromFile(std::filesystem::path const& path);
        static Result<ModMetadata> create(ModJson const& json);

        ModJson toJSON() const;
        ModJson getRawJSON() const;

        bool operator==(ModMetadata::Impl const& other) const;

        static bool validateID(std::string const& id);
        static bool validateOldID(std::string const& id);
        static bool isDeprecatedIDForm(std::string const& id);

        static Result<ModMetadata> createFromSchemaV010(ModJson const& rawJson);

        Result<> addSpecialFiles(std::filesystem::path const& dir);
        Result<> addSpecialFiles(utils::file::Unzip& zip);

        std::vector<std::pair<std::string, std::optional<std::string>*>> getSpecialFiles();
    };

    class ModMetadataImpl : public ModMetadata::Impl {
    public:
        static ModMetadata::Impl& getImpl(ModMetadata& info);
    };
}

template <>
struct matjson::Serialize<freod::ModMetadata::Dependency::Importance> {
    static freod::Result<freod::ModMetadata::Dependency::Importance, std::string> fromJson(Value const& value)
    {
        auto str = FREOD_UNWRAP(value.asString());
        if (str == "required") return freod::Ok(freod::ModMetadata::Dependency::Importance::Required);
        if (str == "recommended") return freod::Ok(freod::ModMetadata::Dependency::Importance::Recommended);
        if (str == "suggested") return freod::Ok(freod::ModMetadata::Dependency::Importance::Suggested);
        return freod::Err("Invalid importance");
    }

    static Value toJson(freod::ModMetadata::Dependency::Importance const& value)
    {
        switch (value) {
            case freod::ModMetadata::Dependency::Importance::Required: return "required";
            case freod::ModMetadata::Dependency::Importance::Recommended: return "recommended";
            case freod::ModMetadata::Dependency::Importance::Suggested: return "suggested";
        }
        return "unknown";
    }
};

template <>
struct matjson::Serialize<freod::ModMetadata::Incompatibility::Importance> {
    static freod::Result<freod::ModMetadata::Incompatibility::Importance, std::string> fromJson(Value const& value)
    {
        auto str = FREOD_UNWRAP(value.asString());
        if (str == "breaking") return freod::Ok(freod::ModMetadata::Incompatibility::Importance::Breaking);
        if (str == "conflicting") return freod::Ok(freod::ModMetadata::Incompatibility::Importance::Conflicting);
        if (str == "superseded") return freod::Ok(freod::ModMetadata::Incompatibility::Importance::Superseded);
        return freod::Err("Invalid importance");
    }

    static Value toJson(freod::ModMetadata::Incompatibility::Importance const& value)
    {
        switch (value) {
            case freod::ModMetadata::Incompatibility::Importance::Breaking: return "breaking";
            case freod::ModMetadata::Incompatibility::Importance::Conflicting: return "conflicting";
            case freod::ModMetadata::Incompatibility::Importance::Superseded: return "superseded";
        }
        return "unknown";
    }
};

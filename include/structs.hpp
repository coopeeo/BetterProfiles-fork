#pragma once

#define TEXTURE_BUTTON_ENABLED "GJ_button_01.png"
#define TEXTURE_BUTTON_DISABLED "GJ_button_04.png"

struct ProfileData {
    int id;
    std::optional<std::string> bio; // md?
    std::optional<std::string> pronouns;

    // socials !!
    std::optional<std::string> website;
    std::optional<std::string> social_github;
    std::optional<std::string> social_bluesky;
    std::optional<std::string> social_fediverse;
    std::optional<std::string> social_discord;
    std::optional<std::string> social_matrix;
    std::optional<std::string> social_tumblr;
    std::optional<std::string> social_myspace;
    std::optional<std::string> social_facebook;

    // custom gradient colors
    std::optional<int> color1;
    std::optional<int> color2;

    auto operator<=>(const ProfileData&) const = default;
};

template <>
struct matjson::Serialize<ProfileData> {
    static ProfileData from_json(const matjson::Value& value) {
        return ProfileData {
            .id = value.try_get<int>("id").value_or(0),
            .bio = value.try_get<std::string>("bio"),
            .pronouns = value.try_get<std::string>("pronouns"),
            // socials
            .website = value.try_get<std::string>("website"),
            .social_github = value.try_get<std::string>("social_github"),
            .social_bluesky = value.try_get<std::string>("social_bluesky"),
            .social_fediverse = value.try_get<std::string>("social_fediverse"),
            .social_discord = value.try_get<std::string>("social_discord"),
            .social_matrix = value.try_get<std::string>("social_matrix"),
            .social_tumblr = value.try_get<std::string>("social_tumblr"),
            .social_myspace = value.try_get<std::string>("social_myspace"),
            .social_facebook = value.try_get<std::string>("social_facebook"),
            .color1 = value.try_get<int>("color1"),
            .color2 = value.try_get<int>("color2"),
        };
    }
    static matjson::Value to_json(const ProfileData& profile_data) {
        auto res = matjson::Object {
            { "id", profile_data.id },
            { "bio", profile_data.bio.value_or("") },
            { "pronouns", profile_data.pronouns.value_or("") },
            // socials
            { "website", profile_data.website.value_or("") },
            { "social_github", profile_data.social_github.value_or("") },
            { "social_bluesky", profile_data.social_bluesky.value_or("") },
            { "social_fediverse", profile_data.social_fediverse.value_or("") },
            { "social_discord", profile_data.social_discord.value_or("") },
            { "social_matrix", profile_data.social_matrix.value_or("") },
            { "social_tumblr", profile_data.social_tumblr.value_or("") },
            { "social_myspace", profile_data.social_myspace.value_or("") },
            { "social_facebook", profile_data.social_facebook.value_or("") },
            { "color1", profile_data.color1.value_or(0) },
            { "color2", profile_data.color2.value_or(0) },
        };
        // for some reason .value_or(nullptr) crashes, work around this by setting the value null after the fact
        if (!profile_data.bio.has_value()) res["bio"] = nullptr;
        if (!profile_data.pronouns.has_value()) res["pronouns"] = nullptr;
        if (!profile_data.website.has_value()) res["website"] = nullptr;
        if (!profile_data.social_github.has_value()) res["social_github"] = nullptr;
        if (!profile_data.social_bluesky.has_value()) res["social_bluesky"] = nullptr;
        if (!profile_data.social_fediverse.has_value()) res["social_fediverse"] = nullptr;
        if (!profile_data.social_discord.has_value()) res["social_discord"] = nullptr;
        if (!profile_data.social_matrix.has_value()) res["social_matrix"] = nullptr;
        if (!profile_data.social_tumblr.has_value()) res["social_tumblr"] = nullptr;
        if (!profile_data.social_myspace.has_value()) res["social_myspace"] = nullptr;
        if (!profile_data.social_facebook.has_value()) res["social_facebook"] = nullptr;
        if (!profile_data.color1.has_value()) res["color1"] = nullptr;
        if (!profile_data.color2.has_value()) res["color2"] = nullptr;
        return res;
    }
};
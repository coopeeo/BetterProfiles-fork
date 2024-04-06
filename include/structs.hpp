#pragma once

struct ProfileData {
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
};

template <>
struct matjson::Serialize<ProfileData> {
    static ProfileData from_json(const matjson::Value& value) {
        return ProfileData {
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
        };
    }
    static matjson::Value to_json(const ProfileData& profile_data) {
        return matjson::Object {
            { "bio", profile_data.bio.value_or(nullptr) },
            { "pronouns", profile_data.pronouns.value_or(nullptr) },
            // socials
            { "website", profile_data.website.value_or(nullptr) },
            { "social_github", profile_data.social_github.value_or(nullptr) },
            { "social_bluesky", profile_data.social_bluesky.value_or(nullptr) },
            { "social_fediverse", profile_data.social_fediverse.value_or(nullptr) },
            { "social_discord", profile_data.social_discord.value_or(nullptr) },
            { "social_matrix", profile_data.social_matrix.value_or(nullptr) },
            { "social_tumblr", profile_data.social_tumblr.value_or(nullptr) },
            { "social_myspace", profile_data.social_myspace.value_or(nullptr) },
            { "social_facebook", profile_data.social_facebook.value_or(nullptr) },
        };
    }
    // You can also implement this method:
    // > static bool is_json(const matjson::Value& value);
    // It is only used if you do value.is<User>();
};
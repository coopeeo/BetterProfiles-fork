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

/*
    ? Have to put it down here, because the structs above don't exist yet if it's at the top!
    * Leave at bottom
*/
#include "structsjson.hpp"
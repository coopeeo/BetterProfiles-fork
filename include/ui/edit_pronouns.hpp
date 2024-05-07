#pragma once
#include "structs.hpp"
#include <Geode/ui/TextInput.hpp>

class EditPronounsPopup : public geode::Popup<ProfileData* const&> {
protected:
    bool setup(ProfileData* const& profile_data) override;

    cocos2d::CCMenu* createPronounSet(int set);
    std::vector<std::string> parsePronouns(std::string const& pronouns);
    std::string getModifiedPronouns(std::vector<std::string> pronouns, int set, std::string pronoun);
    void updateUI();

    void onPronounButtonClicked(int set, std::string pronoun);

    ProfileData* m_profile_data;
    geode::TextInput* m_pronouns_input = nullptr;
    cocos2d::CCMenu* m_pronoun_sets_menu = nullptr;
public:
    static EditPronounsPopup* create(ProfileData* const& profile_data);
};

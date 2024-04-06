#pragma once
#include "structs.hpp"

#define TEXTURE_BUTTON_ENABLED "GJ_button_01.png"
#define TEXTURE_BUTTON_DISABLED "GJ_button_04.png"

class EditPronounsPopup : public geode::Popup<ProfileData* const&> {
protected:
    bool setup(ProfileData* const& profile_data) override;

    cocos2d::CCMenu* createPronounSet(int set);
    std::vector<std::string> parsePronouns(std::string const& pronouns);
    std::string getModifiedPronouns(std::vector<std::string> pronouns, int set, std::string pronoun);
    void updateUI();

    void onPronounButtonClicked(CCObject* sender);

    ProfileData* m_profile_data;
    cocos2d::CCLabelBMFont* m_pronouns_label = nullptr;
public:
    static EditPronounsPopup* create(ProfileData* const& profile_data);
};

#pragma once
#include "structs.hpp"

class EditBioPopup : public geode::Popup<ProfileData* const&>, TextInputDelegate {
protected:
    bool setup(ProfileData* const& profile_data) override;

    ProfileData* m_profile_data;
    CCTextInputNode* m_bio_text_node = nullptr;
public:
    static EditBioPopup* create(ProfileData* const& profile_data);
    virtual void onClose(cocos2d::CCObject*) override;
    virtual void textChanged(CCTextInputNode*) override;
};

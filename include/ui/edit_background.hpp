#pragma once
#include "structs.hpp"

class EditBackgroundPopup : public geode::Popup<ProfileData* const&> {
protected:
    bool setup(ProfileData* const& profile_data) override;

    void updateUI();
    static int colorToInt(cocos2d::ccColor3B color);

    ProfileData* m_profile_data;
public:
    static EditBackgroundPopup* create(ProfileData* const& profile_data);
    virtual void onClose(cocos2d::CCObject*) override;
};
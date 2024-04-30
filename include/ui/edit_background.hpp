#pragma once
#include "structs.hpp"

class EditBackgroundPopup : public geode::Popup<ProfileData* const&>, public geode::ColorPickPopupDelegate {
protected:
    bool setup(ProfileData* const& profile_data) override;

    void updateUI();

    // members
    ProfileData* m_profile_data;
    short m_editing_color = 1;

    // nodes
    cocos2d::CCLayerGradient* m_background = nullptr;
public:
    static EditBackgroundPopup* create(ProfileData* const& profile_data);
    virtual void updateColor(cocos2d::ccColor4B const& color) override;
};
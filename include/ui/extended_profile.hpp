#include <Geode/Geode.hpp>
#include <dashauth.hpp>

#include "ui/editpage.hpp"
#include "ui/edit_pronouns.hpp"
#include "structs.hpp"

class ExtendedProfilePage : public geode::Popup<ProfileData const&, GJUserScore* const&> {
protected:
    bool setup(ProfileData const& profile_data, GJUserScore* const& user_score) override;

    void updateUI();

    // members
    ProfileData m_profile_data;
    GJUserScore* m_user_score;
    std::function<void(ProfileData &)> m_callback = nullptr;

    // nodes
    CCMenuItemSpriteExtra* m_edit_button;
    CCLayerGradient* m_background;
    MDTextArea* m_bio_area;
public:
    static ExtendedProfilePage* create(ProfileData const& profile_data, GJUserScore* const& user_score);
    void setCallback(std::function<void(ProfileData &)> callback);
};

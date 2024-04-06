#include <Geode/Geode.hpp>
#include <rooot.dashauth/include/dashauth.hpp>

#include "structs.hpp"

class EditPage : public geode::Popup<ProfileData const&> {
protected:
    ~EditPage();
    bool setup(ProfileData const& profile_data) override;

    // ui funcs
    void setupLoggedIn();

    // callbacks
    void onLogin(CCObject*);

    // members
    ProfileData m_profile_data;

    // members - nodes
    CCLabelBMFont* m_login_prompt_1 = nullptr;
    CCLabelBMFont* m_login_prompt_2 = nullptr;
    CCMenuItemSpriteExtra* m_login_button = nullptr;
    LoadingCircle* m_login_loading_circle = nullptr;
public:
    static EditPage* create(ProfileData const& profile_data);
};

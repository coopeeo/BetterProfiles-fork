#include <Geode/Geode.hpp>
#include <rooot.dashauth/include/dashauth.hpp>

#include "ui/edit_pronouns.hpp"
#include "structs.hpp"

class EditPage : public geode::Popup<ProfileData const&> {
protected:
    ~EditPage();
    bool setup(ProfileData const& profile_data) override;

    virtual void keyDown(cocos2d::enumKeyCodes key) override;

    virtual void draw() override;

    // ui funcs
    void setupLoggedIn();

    // callbacks
    void onLogin(CCObject*);
    void onEditPronouns(CCObject*);
    void onSave(CCObject*);

    // members
    ProfileData m_profile_data;
    ProfileData m_original_data;

    // members - nodes
    CCLabelBMFont* m_login_prompt_1 = nullptr;
    CCLabelBMFont* m_login_prompt_2 = nullptr;
    ButtonSprite* m_login_sprite = nullptr;
    CCMenuItemSpriteExtra* m_login_button = nullptr;
    LoadingCircle* m_login_loading_circle = nullptr;
    CCMenuItemSpriteExtra* m_save_button = nullptr;
    LoadingCircle* m_save_loading_circle = nullptr;
public:
    static EditPage* create(ProfileData const& profile_data);
};

#include <Geode/Geode.hpp>

#include "ui/editpage.hpp"

using namespace geode::prelude;

// this is to prevent crashes
static EditPage* current_edit_page = nullptr;
EditPage::~EditPage() {
    current_edit_page = nullptr;
}

bool EditPage::setup(ProfileData const& profile_data) {
    current_edit_page = this;
    auto win_size = CCDirector::sharedDirector()->getWinSize();
    
    this->setTitle("Customize Profile");

    // if the user isn't logged in, show a login button
    if (Mod::get()->getSavedValue<std::string>("token", "").empty()) {
        log::info("not logged in!");

        auto button_sprite = ButtonSprite::create("Login", 64, true, "bigFont.fnt", "GJ_button_01.png", 32.0f, 1.0f);

        this->m_login_button = CCMenuItemSpriteExtra::create(
            button_sprite,
            this,
            menu_selector(EditPage::onLogin)
        );
        this->m_login_button->setID("login-button"_spr);
        this->m_login_button->setPosition(0.f, -96.f); // 0 0 is center because menu is centered and cocos is weird :>

        this->m_buttonMenu->addChild(this->m_login_button);

        // login prompt
        this->m_login_prompt_1 = CCLabelBMFont::create("You aren't logged in!", "bigFont.fnt");
        this->m_login_prompt_2 = CCLabelBMFont::create("Click the button below to log in.", "bigFont.fnt");
        this->m_login_prompt_1->setID("login-prompt-1"_spr);
        this->m_login_prompt_2->setID("login-prompt-2"_spr);
        this->m_login_prompt_1->setScale(0.75f);
        this->m_login_prompt_2->setScale(0.65f);
        this->m_login_prompt_1->setPosition(win_size / 2 + ccp(0.f, 22.f));
        this->m_login_prompt_2->setPosition(win_size / 2);

        this->m_mainLayer->addChild(this->m_login_prompt_1);
        this->m_mainLayer->addChild(this->m_login_prompt_2);

        return true;
    }

    log::info("token present, initializing UI");
    this->setupLoggedIn();

    return true;
}

void EditPage::setupLoggedIn() {
    // hide all login-related nodes if they're present
    if(this->m_login_prompt_1) this->m_login_prompt_1->setVisible(false);
    if(this->m_login_prompt_2) this->m_login_prompt_2->setVisible(false);
    if(this->m_login_button) this->m_login_button->setVisible(false);
    if(this->m_login_loading_circle) this->m_login_loading_circle->setVisible(false);

    auto win_size = CCDirector::sharedDirector()->getWinSize();

    // actual UI setup below
    auto main_menu = CCMenu::create();
    main_menu->setPosition(win_size / 2);
    
    auto label = CCLabelBMFont::create("meow", "bigFont.fnt");
    main_menu->addChild(label);

    this->m_mainLayer->addChild(main_menu);
}

void EditPage::onLogin(CCObject*) {
    // user clicked the login button

    // hide login prompt, make button unclickable and grayed out, show loading circle

    if(this->m_login_prompt_1) this->m_login_prompt_1->setVisible(false);
    if(this->m_login_prompt_2) this->m_login_prompt_2->setVisible(false);
    if(this->m_login_button) {
        this->m_login_button->setEnabled(false);
        
        auto button_child = this->m_login_button->getChildren()->objectAtIndex(0);
        if (auto button_sprite = typeinfo_cast<ButtonSprite*>(button_child)) {
            button_sprite->setString("Loading...");
            button_sprite->updateBGImage("GJ_button_04.png");
            button_sprite->m_label->setColor(ccc3(175, 175, 175));
        }
    }

    // create loading circle
    this->m_login_loading_circle = LoadingCircle::create();
    this->m_login_loading_circle->setPosition(0.f, 0.f);
    this->m_login_loading_circle->show();
    this->m_mainLayer->addChild(this->m_login_loading_circle);

    dashauth::DashAuthRequest().getToken(Mod::get(), "https://gd-backend.foxgirl.wtf/api/v1")->except([](std::string const& error) {
        log::info("login failed: {}", error);

        // make sure that we can fuck with the ui
        if (current_edit_page != nullptr) {
            // reset all login related nodes
            if(current_edit_page->m_login_prompt_1) current_edit_page->m_login_prompt_1->setVisible(true);
            if(current_edit_page->m_login_prompt_2) current_edit_page->m_login_prompt_2->setVisible(true);
            log::info("set login prompt visible");
            
            if(current_edit_page->m_login_button) {
                current_edit_page->m_login_button->setEnabled(true);
            
                auto button_child = current_edit_page->m_login_button->getChildren()->objectAtIndex(0);
                if (auto button_sprite = typeinfo_cast<ButtonSprite*>(button_child)) {
                    button_sprite->setString("Login");
                    button_sprite->updateBGImage("GJ_button_01.png");
                    button_sprite->m_label->setColor(ccWHITE);
                }
            }

            if(current_edit_page->m_login_loading_circle) {
                current_edit_page->m_login_loading_circle->removeFromParent();
                current_edit_page->m_login_loading_circle = nullptr;
            }
        }

        log::info("failed to get token :c");
        FLAlertLayer::create("Authentication Error", "failed to get token :c", "OK")->show();
    })->then([](std::string const& token) {
        log::info("got token!! {} :3", token);
        Mod::get()->setSavedValue("token", token);

        if (current_edit_page != nullptr) {
            current_edit_page->setupLoggedIn();
        }
    });
}

EditPage* EditPage::create(ProfileData const& profile_data) {
    auto ret = new EditPage();
    if (ret && ret->init(450.f, 280.f, profile_data)) {
        ret->m_profile_data = profile_data;
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}
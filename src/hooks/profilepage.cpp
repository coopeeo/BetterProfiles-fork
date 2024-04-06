#include <Geode/utils/web.hpp>
#include <Geode/modify/ProfilePage.hpp>

#include "structs.hpp"
#include "ui/editpage.hpp"

using namespace geode::prelude;

class $modify(BetterProfilePage, ProfilePage) {
    CCLabelBMFont* m_pronoun_label;
    ProfileData m_profile_data;
    static inline BetterProfilePage* current_profile_page = nullptr;

    bool init(int accountID, bool ownProfile) {
        if (!ProfilePage::init(accountID, ownProfile)) {
            return false;
        }
        log::info("init profilepage");

        current_profile_page = this;

        if (this->m_usernameLabel == nullptr) {
            log::error("m_usernameLabel is null");
            return true;
        }

        auto pos = this->m_usernameLabel->getPosition();

        log::info("creating pronoun label");
        m_fields->m_pronoun_label = CCLabelBMFont::create("meow", "bigFont.fnt"); // "meow" is a placeholder
        m_fields->m_pronoun_label->setPosition(pos.x, pos.y - 18.0f);
        m_fields->m_pronoun_label->setScale(0.5f);
        m_fields->m_pronoun_label->setID("pronoun-label"_spr);
        m_fields->m_pronoun_label->setVisible(false);
        this->m_mainLayer->addChild(m_fields->m_pronoun_label);

        web::AsyncWebRequest()
            .fetch(fmt::format("https://gd-backend.foxgirl.wtf/api/v1/profiles/{}", accountID))
            .json()
            .then([](matjson::Value const& response) {
                if (!response["success"].as_bool()) {
                    log::error("failed to fetch profile data");
                    return;
                }

                // don't touch UI if it doesn't exist anymore
                if (current_profile_page == nullptr) {
                    log::warn("current_profile_page is null (user exited ui), not updating profile data");
                    return;
                }

                log::info("fetched profile data");

                // parse data and update UI
                current_profile_page->m_fields->m_profile_data = response["data"].as<ProfileData>();
                current_profile_page->updateUIState();
            });

        return true;
    }

    virtual void loadPageFromUserInfo(GJUserScore* score) {
        log::debug("loadPageFromUserInfo");
        ProfilePage::loadPageFromUserInfo(score);

        if (score->isCurrentUser()) {
            // add edit button
            auto edit_button = CCMenuItemSpriteExtra::create(
                CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"),
                this,
                menu_selector(BetterProfilePage::onEditButton)
            );
            edit_button->setID("edit-button"_spr);
            
            auto left_menu = this->getChildByIDRecursive("left-menu");
            if (left_menu == nullptr) {
                log::error("left_menu is null");
                return;
            }

            // prevent the button from deduplicating
            if (!left_menu->getChildByID("edit-button"_spr)) {
                left_menu->addChild(edit_button);
                left_menu->updateLayout();
            }
        }
    }

    // prevent crashes
    void onClose(cocos2d::CCObject* sender) {
        ProfilePage::onClose(sender);
        log::info("onClose");
        current_profile_page = nullptr;
    }

    // -- non-hooks below --

    void onEditButton(CCObject*) {
        EditPage::create(m_fields->m_profile_data)->show();
    }

    // updates UI based on m_profile_data
    void updateUIState() {
        log::debug("updating ui");

        if (this->m_fields->m_profile_data.pronouns.has_value()) {
            log::debug("pronouns: {}", this->m_fields->m_profile_data.pronouns.value());
            
            this->m_fields->m_pronoun_label->setString(this->m_fields->m_profile_data.pronouns.value().c_str());
            this->m_fields->m_pronoun_label->setVisible(true);
        }
    }
};
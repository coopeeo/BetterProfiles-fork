#include <Geode/utils/web.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <UIBuilder.hpp>

#include "structs.hpp"
#include "ui/editpage.hpp"

using namespace geode::prelude;

class $modify(BetterProfilePage, ProfilePage) {
    CCLabelBMFont* m_pronoun_label;
    CCMenuItemSpriteExtra* m_profile_button;
    CCSize m_profile_button_content_size;
    ProfileData m_profile_data;
    bool m_data_loaded = false;

    static inline BetterProfilePage* current_profile_page = nullptr;

    static void onModify(auto &self) {
                                                                    // 1 below node ids
        if (!self.setHookPriority("ProfilePage::loadPageFromUserInfo", 0x100000 - 1)) {
            log::warn("failed to set hook priority, ui might look wacky");
        }
    }

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

        Build<CCLabelBMFont>::create("meow", "bigFont.fnt") // "meow" is a placeholder
            .id("pronoun-label"_spr)
            .pos(pos.x, pos.y - 18.0f)
            .scale(0.5f)
            .store(m_fields->m_pronoun_label)
            .parent(this->m_mainLayer)
            .visible(false);
        return true;
    }

    void loadPageFromUserInfo(GJUserScore* score) {
        log::debug("loadPageFromUserInfo");
        ProfilePage::loadPageFromUserInfo(score);

        auto left_menu = this->getChildByIDRecursive("left-menu");
        if (left_menu == nullptr) {
            log::error("left_menu is null");
            return;
        }

        if (!this->m_fields->m_profile_button) {
            Build<CCSprite>::createSpriteName("GJ_profileButton_001.png")
                .scale(0.55f)
                .intoMenuItem([this](auto) {
                    log::info("profile button clicked");
                    if (this->m_score->isCurrentUser()) {
                        this->onEditButton();
                    } else {
                        log::info("not current user");
                    }
                })
                    .id("profile-button"_spr)
                    .visible(false)
                    .store(this->m_fields->m_profile_button)
                    .parent(left_menu)
                    .pos(408.f, -118.f);
            this->m_fields->m_profile_button_content_size = this->m_fields->m_profile_button->getContentSize();
            this->m_fields->m_profile_button->setContentSize({0.f, 0.f});
        }
        left_menu->updateLayout();

        this->fetchProfileData(score->m_accountID);
    }

    // prevent crashes
    void onClose(cocos2d::CCObject* sender) {
        ProfilePage::onClose(sender);
        log::info("onClose");
        current_profile_page = nullptr;
    }

    void onUpdate(cocos2d::CCObject* sender) {
        ProfilePage::onUpdate(sender);
        this->fetchProfileData(this->m_accountID);
        log::info("onUpdate");
    }

    // -- non-hooks below --

    void onEditButton() {
        auto edit_page = EditPage::create(m_fields->m_profile_data);
        edit_page->setCallback([this](ProfileData const& profile_data) {
            this->m_fields->m_profile_data = profile_data;
            this->updateUIState();
        });
        edit_page->show();
    }

    void fetchProfileData(int account_id) {
        web::AsyncWebRequest()
            .fetch(fmt::format("https://gd-backend.foxgirl.wtf/api/v1/profiles/{}", account_id))
            .json()
            .then([account_id](matjson::Value const& response) {
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
                current_profile_page->m_fields->m_profile_data.id = account_id;
                current_profile_page->m_fields->m_data_loaded = true;
                current_profile_page->updateUIState();
        }).expect([account_id](std::string const& error) {
            log::error("failed to fetch profile data: {}", error);
            if (current_profile_page != nullptr) {
                current_profile_page->m_fields->m_profile_data = ProfileData();
                current_profile_page->m_fields->m_profile_data.id = account_id;
                current_profile_page->m_fields->m_data_loaded = false;
                current_profile_page->updateUIState();
            }
        });
    }

    // updates UI based on m_profile_data
    void updateUIState() {
        log::debug("updating ui");

        if (this->m_fields->m_data_loaded || this->m_score->isCurrentUser()) {
            log::debug("profile has data (or is self)");
            if (this->m_fields->m_profile_button != nullptr) {
                this->m_fields->m_profile_button->setVisible(true);
                this->m_fields->m_profile_button->setContentSize(this->m_fields->m_profile_button_content_size);
                if (auto left_menu = this->getChildByIDRecursive("left-menu")) {
                    left_menu->updateLayout();
                }
            } else {
                log::error("wtf! m_profile_button is null!");
            }
        }

        if (this->m_fields->m_profile_data.pronouns.has_value()) {
            log::debug("pronouns: {}", this->m_fields->m_profile_data.pronouns.value());
            
            this->m_fields->m_pronoun_label->setString(this->m_fields->m_profile_data.pronouns.value().c_str());
            this->m_fields->m_pronoun_label->setVisible(true);
        }
    }
};
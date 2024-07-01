#include <Geode/utils/web.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <UIBuilder.hpp>

#include "backend.hpp"
#include "structs.hpp"
#include "ui/extended_profile.hpp"
#include "state.hpp"

using namespace geode::prelude;

class $modify(BetterProfilePage, ProfilePage) {
    struct Fields {
        CCLabelBMFont* m_pronoun_label = nullptr;
        CCMenuItemSpriteExtra* m_profile_button;
        CCSize m_profile_button_content_size;
        ProfileData m_profile_data;
        bool m_data_loaded = false;
    };

    static inline BetterProfilePage* current_profile_page = nullptr;

    bool init(int accountID, bool ownProfile) {
        if (!ProfilePage::init(accountID, ownProfile)) {
            return false;
        }
        log::info("init profilepage");
        current_profile_page = this;

        // important:
        // don't actually init any UI here
        // sometimes this function gets called *after*
        // loadPageFromUserInfo, which is where the UI is used

        return true;
    }

    void loadPageFromUserInfo(GJUserScore* score) {
        log::debug("loadPageFromUserInfo");
        ProfilePage::loadPageFromUserInfo(score);

        if (this->m_usernameLabel == nullptr) {
            log::error("m_usernameLabel is null");
            return;
        }

        auto pos = this->m_usernameLabel->getPosition();

        if (!this->m_fields->m_pronoun_label) {
            log::debug("creating pronoun label");
            Build<CCLabelBMFont>::create("meow", "bigFont.fnt") // "meow" is a placeholder
                .id("pronoun-label"_spr)
                .pos(pos.x, pos.y - 18.0f)
                .scale(0.5f)
                .store(m_fields->m_pronoun_label)
                .parent(this->m_mainLayer)
                .visible(false);
        }

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
                    auto epp = ExtendedProfilePage::create(m_fields->m_profile_data, m_score);
                    if (this->m_score->isCurrentUser()) {
                        epp->setCallback([this](ProfileData &profile_data) {
                            this->m_fields->m_profile_data = profile_data;
                            State::sharedInstance()->profiles[this->m_accountID] = profile_data;
                            this->updateUIState();
                        });
                    }
                    epp->show();
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

        // show profile button for own user quickly
        this->updateUIState();

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
        this->fetchProfileData(this->m_accountID, true);
        log::info("onUpdate");
    }

    // -- non-hooks below --

    void fetchProfileData(int account_id, bool force = false) {
        auto state = State::sharedInstance();

        if (state->profiles.contains(account_id) && !force) {
            log::info("using cached profile data");
            this->m_fields->m_profile_data = state->profiles[account_id];
            this->m_fields->m_data_loaded = true;
            this->updateUIState();
            return;
        }

        web::AsyncWebRequest()
            .fetch(fmt::format("{}/api/v1/profiles/{}", BACKEND_PREFIX, account_id))
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
                auto profile_data = response["data"].as<ProfileData>();
                current_profile_page->m_fields->m_profile_data = profile_data;
                current_profile_page->m_fields->m_profile_data.id = account_id;
                current_profile_page->m_fields->m_data_loaded = true;

                State::sharedInstance()->profiles[account_id] = profile_data;
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

        if (Loader::get()->isModLoaded("bitz.customprofiles")) {
            log::debug("customprofiles loaded, doing extra support");
            if (auto custom_gradient = typeinfo_cast<CCLayerGradient*>(this->getChildByIDRecursive("bitz.customprofiles/normal-gradient"))) {
                log::debug("found custom gradient");
                if (this->m_fields->m_profile_data.color1.has_value()) {
                    log::debug("setting custom gradient color 1");
                    int r = this->m_fields->m_profile_data.color1.value() >> 16 & 0xFF;
                    int g = this->m_fields->m_profile_data.color1.value() >> 8 & 0xFF;
                    int b = this->m_fields->m_profile_data.color1.value() & 0xFF;
                    custom_gradient->setStartColor(ccc3(r, g, b));
                }

                if (this->m_fields->m_profile_data.color2.has_value()) {
                    log::debug("setting custom gradient color 2");
                    int r = this->m_fields->m_profile_data.color2.value() >> 16 & 0xFF;
                    int g = this->m_fields->m_profile_data.color2.value() >> 8 & 0xFF;
                    int b = this->m_fields->m_profile_data.color2.value() & 0xFF;
                    custom_gradient->setEndColor(ccc3(r, g, b));
                }
            }
        }
    }
};
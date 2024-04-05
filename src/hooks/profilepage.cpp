#include <Geode/utils/web.hpp>
#include <Geode/modify/ProfilePage.hpp>

#include "structs.hpp"

using namespace geode::prelude;

class $modify(BetterProfilePage, ProfilePage) {
    CCLabelBMFont* m_pronoun_label;
    ProfileData m_profile_data;

    bool init(int accountID, bool ownProfile) {
        if (!ProfilePage::init(accountID, ownProfile)) {
            return false;
        }
        log::info("init profilepage");

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
            .fetch(fmt::format("http://localhost:61475/api/v1/profiles/{}", accountID))
            .json()
            .then([this](matjson::Value const& response) {
                if (!response["success"].as_bool()) {
                    log::error("failed to fetch profile data");
                    return;
                }
                log::info("fetched profile data");
                auto data = response["data"];

                // parse data into m_profileData (i hate this code lmao)
                auto bio = data.try_get<std::string>("bio");
                log::info("bio: {}", bio.value_or("null"));

                m_fields->m_profile_data = data.as<ProfileData>();

                log::debug("i am so confused");

                if (m_fields->m_profile_data.pronouns.has_value()) {
                    log::debug("settings pronoun label");
                    log::debug("pronouns: {}", m_fields->m_profile_data.pronouns.value());
                    
                    m_fields->m_pronoun_label->setString(m_fields->m_profile_data.pronouns.value().c_str());
                    m_fields->m_pronoun_label->setVisible(true);
                }
            });

        return true;
    }

    virtual void loadPageFromUserInfo(GJUserScore* score) {
        log::debug("loadPageFromUserInfo");
        ProfilePage::loadPageFromUserInfo(score);

        // add edit button
        auto edit_button = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"),
			this,
			nullptr
		);
        edit_button->setID("edit-button"_spr);

        auto left_menu = this->getChildByIDRecursive("left-menu");
        if (left_menu == nullptr) {
            log::error("left_menu is null");
            return;
        }

        if (!left_menu->getChildByID("edit-button"_spr)) {
            left_menu->addChild(edit_button);
            left_menu->updateLayout();
        }
    }
};
#include <Geode/utils/web.hpp>
#include <Geode/modify/ProfilePage.hpp>

using namespace geode::prelude;

class $modify(BetterProfilePage, ProfilePage) {
    CCLabelBMFont* m_pronoun_label;

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
        this->m_pronoun_label = CCLabelBMFont::create("meow", "bigFont.fnt"); // "meow" is a placeholder
        this->m_pronoun_label->setPosition(pos.x, pos.y - 18.0f);
        this->m_pronoun_label->setScale(0.5f);
        this->m_pronoun_label->setID("pronoun-label"_spr);
        this->m_pronoun_label->setVisible(false);
        this->m_mainLayer->addChild(this->m_pronoun_label);

        web::AsyncWebRequest()
            .fetch(fmt::format("http://localhost:61475/api/v1/profiles/{}", accountID))
            .json()
            .then([this](matjson::Value const& response) {
                auto pronouns = response["data"]["pronouns"].as_string();

                this->m_pronoun_label->setString(pronouns.c_str());
                this->m_pronoun_label->setVisible(true);
            });

        return true;
    }

    virtual void loadPageFromUserInfo(GJUserScore* score) {
        log::debug("loadPageFromUserInfo");
        ProfilePage::loadPageFromUserInfo(score);
    }
};
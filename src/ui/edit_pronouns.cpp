#include <Geode/Geode.hpp>
#include <string>
#include <vector>

#include "ui/edit_pronouns.hpp"

using namespace geode::prelude;

EditPronounsPopup* EditPronounsPopup::create(ProfileData* const& profile_data) {
    auto ret = new EditPronounsPopup();
    if (ret && ret->init(450.f, 280.f, profile_data)) {
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool EditPronounsPopup::setup(ProfileData* const& profile_data) {
    this->m_profile_data = profile_data;
    log::info("current pronouns: {}", profile_data->pronouns);
    auto win_size = CCDirector::sharedDirector()->getWinSize();

    this->setTitle("Change Pronouns");

    this->m_pronouns_label = CCLabelBMFont::create("meow", "bigFont.fnt");
    this->m_pronouns_label->setPosition(win_size.width / 2, 60.f);
    this->m_pronouns_label->setScale(0.75f);
    this->m_mainLayer->addChild(this->m_pronouns_label);

    auto pronoun_sets_menu = CCMenu::create();
    pronoun_sets_menu->setLayout(RowLayout::create());
    pronoun_sets_menu->setPosition(win_size / 2);
    pronoun_sets_menu->setID("pronoun-sets-menu"_spr);
    pronoun_sets_menu->addChild(this->createPronounSet(1));
    pronoun_sets_menu->addChild(this->createPronounSet(2));
    pronoun_sets_menu->addChild(this->createPronounSet(3));
    pronoun_sets_menu->updateLayout();

    this->m_mainLayer->addChild(pronoun_sets_menu);

    this->parsePronouns(profile_data->pronouns.value_or(""));
    this->updateUI();

    return true;
}

std::vector<std::string> EditPronounsPopup::parsePronouns(std::string const& pronouns_const) {
    std::string pronouns = pronouns_const;
    std::vector<std::string> sets;

    std::string tmp;
    std::stringstream pronouns_ss(pronouns);
    while (getline(pronouns_ss, tmp, '/')) {
        // ignore if it's a 2nd pronoun
        if (!(tmp == "them" || tmp == "its" || tmp == "her" || tmp == "him")) {
            sets.push_back(tmp);
        }
    }

    return sets;
}

std::string EditPronounsPopup::getModifiedPronouns(std::vector<std::string> pronouns, int set, std::string pronoun) {
    if (pronouns.size() < set) {
        log::debug("resizing pronouns to {}", set);
        pronouns.resize(set);
    }

    if (pronouns.at(set - 1) == pronoun) {
        if (set == 1 && pronouns.size() == 1) {
            return ""; // no pronouns (prevent freeze)
        }
        pronouns.erase(pronouns.begin() + (set - 1));
    } else {
        pronouns.at(set - 1) = pronoun;
    }

    std::string new_pronouns = "";
    for (auto const& p : pronouns) {
        new_pronouns += p + "/";
    }
    new_pronouns.pop_back();

    if (pronouns.size() == 1) {
        std::string second_pronoun;
        if (pronouns.at(0) == "they") {
            second_pronoun = "them";
        } else if (pronouns.at(0) == "it") {
            second_pronoun = "its";
        } else if (pronouns.at(0) == "she") {
            second_pronoun = "her";
        } else if (pronouns.at(0) == "he") {
            second_pronoun = "him";
        }
        new_pronouns += "/" + second_pronoun;
    }

    return new_pronouns;
}

void EditPronounsPopup::updateUI() {
    auto menu = typeinfo_cast<CCMenu*>(this->m_mainLayer->getChildByID("pronoun-sets-menu"_spr));
    if (!menu) {
        log::error("failed to get pronoun sets menu");
        return;
    }

    auto pronouns = this->parsePronouns(this->m_profile_data->pronouns.value_or(""));

    auto pronoun_sets_menu = typeinfo_cast<CCMenu*>(this->m_mainLayer->getChildByID("pronoun-sets-menu"_spr));

    for (int i = 0; i < 3; i++) {
        auto set = i + 1;

        std::string pronoun = "";
        if (pronouns.size() < set) {
            //log::info("no pronouns for set {}", set);
        } else {
            pronoun = pronouns.at(i);
        }        

        //log::info("pronoun {} in set {}", pronoun, set);

        auto set_menu = typeinfo_cast<CCMenu*>(pronoun_sets_menu->getChildByID(fmt::format("set-{}", set)));
        if (!set_menu) {
            log::error("failed to get pronoun set menu for set {}", set);
            return;
        }

        // loop over set_menu children
        for (int j = 0; j < set_menu->getChildren()->count(); j++) {
            if (auto button = typeinfo_cast<CCMenuItemSpriteExtra*>(set_menu->getChildren()->objectAtIndex(j))) {
                if (auto button_sprite = typeinfo_cast<ButtonSprite*>(button->getChildren()->objectAtIndex(0))) {
                    button_sprite->updateBGImage(TEXTURE_BUTTON_DISABLED);
                }
            }
        }

        if (auto button = typeinfo_cast<CCMenuItemSpriteExtra*>(set_menu->getChildByID(pronoun))) {
            if (auto button_sprite = typeinfo_cast<ButtonSprite*>(button->getChildren()->objectAtIndex(0))) {
                button_sprite->updateBGImage(TEXTURE_BUTTON_ENABLED);
            }
        }
    }

    // update pronouns label
    this->m_pronouns_label->setString(this->m_profile_data->pronouns.value_or("").c_str());
}

CCMenu* EditPronounsPopup::createPronounSet(int set) {
    auto menu = CCMenu::create();
    menu->setPosition(0.f, 0.f);
    menu->setLayout(ColumnLayout::create());

    menu->setTag(set);
    menu->setID(fmt::format("set-{}", set));

    auto pronoun_button_they = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("they/them", 40, true, "bigFont.fnt", TEXTURE_BUTTON_ENABLED, 32.0f, 1.0f),
        this,
        menu_selector(EditPronounsPopup::onPronounButtonClicked)
    );
    pronoun_button_they->setID("they");
    menu->addChild(pronoun_button_they);

    auto pronoun_button_it = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("it/its", 40, true, "bigFont.fnt", TEXTURE_BUTTON_ENABLED, 32.0f, 1.0f),
        this,
        menu_selector(EditPronounsPopup::onPronounButtonClicked)
    );
    pronoun_button_it->setID("it");
    menu->addChild(pronoun_button_it);

    auto pronoun_button_she = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("she/her", 40, true, "bigFont.fnt", TEXTURE_BUTTON_ENABLED, 32.0f, 1.0f),
        this,
        menu_selector(EditPronounsPopup::onPronounButtonClicked)
    );
    pronoun_button_she->setID("she");
    menu->addChild(pronoun_button_she);

    auto pronoun_button_he = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("he/him", 40, true, "bigFont.fnt", TEXTURE_BUTTON_ENABLED, 32.0f, 1.0f),
        this,
        menu_selector(EditPronounsPopup::onPronounButtonClicked)
    );
    pronoun_button_he->setID("he");
    menu->addChild(pronoun_button_he);

    menu->updateLayout();

    return menu;
}

// this should *ONLY* be called for buttons inside of a pronoun set ccmenu
void EditPronounsPopup::onPronounButtonClicked(CCObject* sender) {
    auto button = typeinfo_cast<CCMenuItemSpriteExtra*>(sender);
    if (!button) {
        return;
    }

    auto menu = typeinfo_cast<CCMenu*>(button->getParent());
    if (!menu) {
        return;
    }

    auto set = menu->getTag();
    auto pronoun = button->getID();

    std::vector<std::string> pronouns = this->parsePronouns(this->m_profile_data->pronouns.value_or(""));

    //std::vector<std::string> pronouns = this->parsePronouns("she/her");
    if (pronouns.size() + 1 < set && (pronouns.size() != 0 && set != 1)) {
        Notification::create(fmt::format("Select set {} before trying to change set {}", set - 1, set), NotificationIcon::Error)->show();
        return;
    }

    log::info("current pronouns: {} ({})", pronouns, pronouns.size());

    this->m_profile_data->pronouns = this->getModifiedPronouns(pronouns, set, pronoun);
    this->updateUI();
}

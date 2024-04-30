#include <Geode/Geode.hpp>

#include <UIBuilder.hpp>
#include "ui/extended_profile.hpp"
#include "utils.hpp"

using namespace geode::prelude;

ExtendedProfilePage* ExtendedProfilePage::create(ProfileData* const& profile_data, GJUserScore* const& user_score) {
    auto ret = new ExtendedProfilePage();
    if (ret && ret->init(441.f, 292.f, profile_data, user_score)) {
        ret->m_profile_data = profile_data;
        ret->m_user_score = user_score;
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ExtendedProfilePage::setup(ProfileData* const& profile_data, GJUserScore* const& user_score) {
    auto win_size = CCDirector::sharedDirector()->getWinSize();
    auto game_manager = GameManager::sharedState();

    auto player_col1 = game_manager->colorForIdx(user_score->m_color1);
    auto player_col2 = game_manager->colorForIdx(user_score->m_color2);

    auto col1 = profile_data->color1.has_value() ? Utils::intToColor(profile_data->color1.value_or(0)) : player_col1;
    auto col2 = profile_data->color2.has_value() ? Utils::intToColor(profile_data->color2.value_or(0)) : player_col2;

    auto bg_sprite = getChildOfType<CCScale9Sprite>(this->m_mainLayer, 0);

    auto popup_content_size = CCSize(0.f, 0.f);
    if (bg_sprite) {
        popup_content_size = bg_sprite->getContentSize();
        bg_sprite->setVisible(false);

        auto gradient_layer = CCLayerGradient::create({col1.r, col1.g, col1.b, 255}, ccc4(col2.r, col2.g, col2.b, 255));
        gradient_layer->setContentSize(popup_content_size - ccp(5.f, 5.f));
        gradient_layer->ignoreAnchorPointForPosition(false);
        gradient_layer->setPosition(win_size / 2);
        this->m_mainLayer->addChild(gradient_layer, -1);

        auto popup_frame = CCScale9Sprite::create("GJ_square07.png");
        popup_frame->setContentSize(popup_content_size);
        popup_frame->setPosition(win_size / 2);
        this->m_mainLayer->addChild(popup_frame, 1);

        // when we're done with bg_sprite, nuke it
        bg_sprite->removeFromParentAndCleanup(true);
    } else {
        log::error("background is null");
    }

    auto darkener = CCScale9Sprite::create("square02b_001.png");
    darkener->setContentSize(popup_content_size - ccp(15.f, 15.f));
    darkener->setPosition(win_size / 2);
    darkener->setColor(ccc3(0, 0, 0));
    darkener->setOpacity(50);
    this->m_mainLayer->addChild(darkener, 0);

    auto bio = MDTextArea::create(profile_data->bio.value_or("this player <cr>doesn't</c> have a bio set!"), {280.f, 150.f});
    bio->setPosition(win_size / 2);
    this->m_mainLayer->addChild(bio);

    this->setTitle(fmt::format("{}'s Profile", user_score->m_userName));

    auto spider = SimplePlayer::create(0);
    spider->updatePlayerFrame(user_score->m_playerSpider, IconType::Spider);
    spider->setColor(player_col1);
    spider->setSecondColor(player_col2);
    if (user_score->m_glowEnabled) {
        spider->setGlowOutline(game_manager->colorForIdx(user_score->m_color3));
    }
    spider->setPosition({win_size.width / 4 + 32.f, 32.f});

    this->m_mainLayer->addChild(spider);

    // create menu
    auto menu = CCMenu::create();
    menu->setPosition(win_size / 2);
    this->m_mainLayer->addChild(menu);
    
    // add edit button if this is the current user
    Build<CCSprite>::createSpriteName("GJ_profileButton_001.png")
        .scale(0.55f)
        .intoMenuItem([this](auto) {
                log::info("edit button clicked");
                auto edit_page = EditPage::create(*this->m_profile_data);
                edit_page->setCallback(this->m_callback);
                edit_page->show();
            })
            .id("edit-button"_spr)
            .visible(false)
            .store(this->m_edit_button)
            .parent(menu)
            /*.pos(408.f, -118.f)*/;

    return true;
}

void ExtendedProfilePage::setCallback(std::function<void(ProfileData const&)> callback) {
    this->m_callback = callback;
    if (this->m_edit_button) {
        this->m_edit_button->setVisible(true);
    }
}

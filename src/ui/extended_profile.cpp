#include <Geode/Geode.hpp>

#include "ui/extended_profile.hpp"
//#include <UIBuilder.hpp>

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

    auto col1 = game_manager->colorForIdx(user_score->m_color1);
    auto col2 = game_manager->colorForIdx(user_score->m_color2);

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
    spider->setColor(col1);
    spider->setSecondColor(col2);
    if (user_score->m_glowEnabled) {
        spider->setGlowOutline(game_manager->colorForIdx(user_score->m_color3));
    }
    spider->setPosition({win_size.width / 4 + 32.f, 32.f});

    this->m_mainLayer->addChild(spider);

    return true;
}

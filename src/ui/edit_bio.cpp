#include <Geode/Geode.hpp>
#include <UIBuilder.hpp>
#include <string>
#include <vector>

#include "ui/edit_bio.hpp"

using namespace geode::prelude;

EditBioPopup* EditBioPopup::create(ProfileData* const& profile_data) {
    auto ret = new EditBioPopup();
    if (ret && ret->init(450.f, 280.f, profile_data)) {
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool EditBioPopup::setup(ProfileData* const& profile_data) {
    this->m_profile_data = profile_data;
    log::info("current bio: {}", profile_data->bio);
    auto win_size = CCDirector::sharedDirector()->getWinSize();

    this->setTitle("Change Bio (WIP!)");

    // this bitch does NOT support newlines :(
    Build<CCTextInputNode>::create(400.f, 200.f, "bio placeholder bleh !!", "chatFont.fnt")
        .id("bio-text-field"_spr)
        .pos(win_size / 2)
        .store(m_bio_text_node);

    // this bitch does but we can't use uibuilder
    m_mainLayer->addChild(m_bio_text_node);

    // idfk - todo: make class based on CCTextInputNode that supports newlines (and doesn't break when using them)
    m_bio_text_node->setAllowedChars("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz,;.:-_<>^°1!2\"34$5%6&7/8(9)0=?´+*#'\n ");
    m_bio_text_node->setString(profile_data->bio.value_or("").c_str());
    m_bio_text_node->setDelegate(this);

    return true;
}

void EditBioPopup::onClose(cocos2d::CCObject* a) {
    // store text from popup when user closes the popup (to not discard what they wrote)
    this->m_profile_data->bio = m_bio_text_node->getString();
    geode::Popup<ProfileData* const&>::onClose(a);
}

void EditBioPopup::enterPressed(CCTextInputNode* a) {
    log::debug("enter pressed !!!!");
    TextInputDelegate::enterPressed(a);
}

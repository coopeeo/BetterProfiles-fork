#include <Geode/Geode.hpp>
#include <Geode/ui/ColorPickPopup.hpp>

#include "ui/edit_background.hpp"

using namespace geode::prelude;

EditBackgroundPopup* EditBackgroundPopup::create(ProfileData* const& profile_data) {
    auto ret = new EditBackgroundPopup();
    if (ret && ret->init(340.f, 260.f, profile_data)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool EditBackgroundPopup::setup(ProfileData* const& profile_data) {
    auto win_size = CCDirector::sharedDirector()->getWinSize();

    this->setTitle("meow meow");

    return true;
}

void EditBackgroundPopup::onClose(cocos2d::CCObject* a) {
    // todo
    geode::Popup<ProfileData* const&>::onClose(a);
}

void EditBackgroundPopup::updateUI() {
    // todo
}

int EditBackgroundPopup::colorToInt(cocos2d::ccColor3B col) {
    return (col.r << 16) | (col.g << 8) | col.b;
}

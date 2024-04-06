#include <Geode/Geode.hpp>
#include <rooot.dashauth/include/dashauth.hpp>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>
class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		auto myButton = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"),
			this,
			menu_selector(MyMenuLayer::onMyButton)
		);

		auto menu = this->getChildByID("bottom-menu");
		menu->addChild(myButton);

		myButton->setID("my-button"_spr);

		menu->updateLayout();

		return true;
	}

	void onMyButton(CCObject*) {
		dashauth::DashAuthRequest().getToken(Mod::get(), "https://gd-backend.foxgirl.wtf/api/v1")->except([](std::string const& error) {
			log::info("failed to get token :c");
			FLAlertLayer::create("DashAuth Error", "failed to get token :c", "OK")->show();
		})->then([](std::string const& token) {
			log::info("got token!! {} :3", token);
			FLAlertLayer::create("meow", fmt::format("{}", token), "bomb brazil")->show();
		});
	}
};

#include <Geode/Geode.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/ui/GeodeUI.hpp>
// No migration Needed
using namespace geode::prelude;

class PopupModSettings : public CCLayer {
  public:
	void ShowPopup(CCObject *) {
		geode::openSettingsPopup(Mod::get(), true);
	}
};

class $modify(PauseLayer) {
	void customSetup() {
		PauseLayer::customSetup();

		// tidak perlu bikin tombol kalau setting dimatikan
		if (!Mod::get()->getSettingValue<bool>("settings-button")) return;

		auto spr = CCSprite::create("Button.png"_spr);
		if (!spr) {
			log::warn("Click Sounds: Button.png tidak ditemukan, tombol settings dilewati");
			return;
		}
		spr->setScale(0.7f);

		auto btn = CCMenuItemSpriteExtra::create(
		    spr,
		    this,
		    menu_selector(PopupModSettings::ShowPopup));
		btn->setID("SettingButton"_spr);

		// [GD 2.2091] ID menu bisa berbeda / tidak ada -> jangan langsung dipakai
		auto menu = this->getChildByID("left-button-menu");
		if (menu) {
			btn->setPosition({menu->getContentSize().width / 2, btn->getContentSize().height / 2});
			menu->addChild(btn);
			menu->updateLayout();
		} else {
			log::warn("Click Sounds: 'left-button-menu' tidak ada di PauseLayer, memakai menu sendiri");
			auto fallback = CCMenu::create();
			fallback->setID("settings-menu"_spr);
			fallback->setPosition({30.f, 30.f});
			btn->setPosition({0.f, 0.f});
			fallback->addChild(btn);
			this->addChild(fallback);
		}
	}
};

class $modify(EditorPauseLayer) {
	bool init(LevelEditorLayer *po) {
		if (!EditorPauseLayer::init(po))
			return false;

		if (!Mod::get()->getSettingValue<bool>("settings-button")) return true;

		auto spr = CCSprite::create("Button.png"_spr);
		if (!spr) {
			log::warn("Click Sounds: Button.png tidak ditemukan, tombol settings dilewati");
			return true;
		}
		spr->setScale(0.6f);

		auto btn = CCMenuItemSpriteExtra::create(
		    spr,
		    this,
		    menu_selector(PopupModSettings::ShowPopup));
		btn->setID("SettingButton"_spr);

		auto menu = this->getChildByID("settings-menu");
		if (menu) {
			btn->setPosition({menu->getContentSize().width / 2, btn->getContentSize().height / 2});
			menu->addChild(btn);
			menu->updateLayout();
		} else {
			log::warn("Click Sounds: 'settings-menu' tidak ada di EditorPauseLayer, memakai menu sendiri");
			auto fallback = CCMenu::create();
			fallback->setID("settings-menu"_spr);
			fallback->setPosition({30.f, 30.f});
			btn->setPosition({0.f, 0.f});
			fallback->addChild(btn);
			this->addChild(fallback);
		}
		return true;
	}
};

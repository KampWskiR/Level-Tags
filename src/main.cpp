#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/ui/LoadingSpinner.hpp>
#include <Geode/modify/LevelSearchLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>

#include "hooks/levelInfoLayer.cpp"
#include "layers/tagsSearch.hpp"
#include "tagsManager.hpp"

using namespace geode::prelude;

class $modify(TagsMenuLayer, MenuLayer) {
    struct Fields {async::TaskHolder<geode::utils::web::WebResponse> m_listener;};
    $override
    bool init() {
        if (!MenuLayer::init()) return false;
        if (TagsManager::sharedState()->tags.size() != 0) return true;
        
        TagsManager::sharedState()->loadTagsInfo();
        return true;
    };
};

class $modify(TagsLevelSearchLayer, LevelSearchLayer) {
    $override
    bool init(int p0) {
        if (!LevelSearchLayer::init(p0)) return false;

        if (auto menu = this->getChildByID("other-filter-menu")) {
            auto searchSpr = CircleButtonSprite::createWithSprite("search.png"_spr, 1, CircleBaseColor::DarkPurple);
            searchSpr->setScale(0.8);
            auto searchBtn = CCMenuItemSpriteExtra::create(searchSpr, this, menu_selector(TagsLevelSearchLayer::menu));
            menu->addChild(searchBtn);
            menu->updateLayout();
        }

        return true;
    };

    void menu(CCObject* sender) {
        CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, TagsSearch::scene()));
    }
};
#pragma once

using namespace geode::prelude;

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "Geode/ui/GeodeUI.hpp"
#include <random>

#include "tagsSelect.hpp"
#include "../tagsManager.hpp"
//#include "../levelManagerDelegate.hpp"
#include "credits.hpp"
#include "uiSquare.hpp"
#include "kofiPopup.hpp"

class TagsSearch : public cocos2d::CCLayer {
public:
    async::TaskHolder<geode::utils::web::WebResponse> m_listener;
    CCMenu* includeMenu;
    CCMenu* excludeMenu;
    CCMenuItemSpriteExtra* searchBtn;
    matjson::Value filters;
    std::vector<std::string> includeTags;
    std::vector<std::string> excludeTags;
    bool includeSelected = true;
    matjson::Value levelPages;
    int fetchCount = 0;
    CCLabelBMFont* sortResultLabel;
    CCLabelBMFont* versionResultLabel;
    int sort = 0; // 0: Recent, 1: Oldest, 2: Random
    int version = 0; // 0: Any, 1: 2.2, 2: 2.1, 3: 2.0, 4: 1.9, 5: 1.8, 6: 1.7, 7: 1.6, 8: 1.5, 9: 1.4, 10: 1.3, 11: 1.2, 12: 1.1, 13: 1.0


    virtual bool init();
    void sortChange(CCObject* sender);
    void versionChange(CCObject* sender);
    // void createInclude(CCPoint pos);
    // void createExclude(CCPoint pos);
    // void createSort(CCPoint pos);
    // void createVersion(CCPoint pos);
    // void createDifficulty(CCPoint pos);
    void credits(CCObject*);
    void settings(CCObject*);
    void kofi(CCObject*);
    void onTags(cocos2d::CCObject* sender);
    void updateTags(bool include, std::vector<std::string> tags);
    void search(CCObject*);
    void keyDown(cocos2d::enumKeyCodes key, double timestamp) override;

    void onBack(CCObject*);
    static CCScene* scene();
    static TagsSearch* create();
};
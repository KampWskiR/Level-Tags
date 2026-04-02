#include "tagsSearch.hpp"

bool TagsSearch::init() {
    if(!CCLayer::init()) return false;
    auto winSize = CCDirector::get()->getWinSize();

    CCMenu* m_menu = CCMenu::create();
    m_menu->setPosition({0,0});
    m_menu->setContentSize(winSize);
    m_menu->setID("main-menu");
    addChild(m_menu, 999);

    CCSprite* backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    CCMenuItemSpriteExtra* backBtn = CCMenuItemSpriteExtra::create(backSpr, this, menu_selector(TagsSearch::onBack));
    backBtn->setPosition({24.f, getContentHeight() - 23.f});
    m_menu->addChild(backBtn);

    auto m_background = CCLayerColor::create();
    m_background->setAnchorPoint({ 0.f, 0.f });
    m_background->setScaleX(winSize.width);
    m_background->setScaleY(winSize.height);
    m_background->setColor({ 25, 12, 12 });
    m_background->setOpacity(255);
    m_background->setPosition(ccp(-5, -5));
    m_background->setID("background");
    addChild(m_background, -2);

    addChild(TagsManager::addBgAnim(winSize));

    auto top = CCSprite::create("searchTop.png"_spr);
    top->setPosition({ getContentWidth() / 2, getContentHeight() - 14.f });
    addChild(top);

    auto title = CCSprite::create("explore.png"_spr);
    title->setPosition({getContentWidth() / 2, getContentHeight() - 17.f});
    title->setScale(1.25f);
    addChild(title);

    auto bottom = CCSprite::create("searchTop.png"_spr);
    bottom->setFlipY(true);
    bottom->setAnchorPoint({ 0.5f, 0.f });
    bottom->setPosition({ getContentWidth() / 2, -15.f });
    addChild(bottom);

    // bottom menu
    auto bottomMenu = CCMenu::create();
    bottomMenu->setPosition({ getContentWidth() / 2, 30.f });
    bottomMenu->setContentSize({ 200.f, 50.f });
    bottomMenu->setLayout(AxisLayout::create()->setGap(10.f)->setAxis(Axis::Row)->setAxisReverse(false));
    addChild(bottomMenu);

    auto settingsSpr = CircleButtonSprite::createWithSprite("settings.png"_spr, 1.f, CircleBaseColor::DarkPurple);
    settingsSpr->setScale(0.75f);

    auto settings = CCMenuItemSpriteExtra::create(settingsSpr, this, menu_selector(TagsSearch::settings));
    settings->setID("settings");
    bottomMenu->addChild(settings);

    auto searchSpr = CircleButtonSprite::createWithSprite("search2.png"_spr, 1.f, CircleBaseColor::DarkPurple); 

    searchBtn = CCMenuItemSpriteExtra::create(searchSpr, this, menu_selector(TagsSearch::search));
    searchBtn->setID("search-btn");
    bottomMenu->addChild(searchBtn);

    auto creditsSpr = CircleButtonSprite::createWithSprite("creditss.png"_spr, 1.f, CircleBaseColor::DarkPurple);
    creditsSpr->setScale(0.75f);

    auto credits = CCMenuItemSpriteExtra::create(creditsSpr, this, menu_selector(TagsSearch::credits));
    credits->setID("credits");
    bottomMenu->addChild(credits);

    bottomMenu->updateLayout();

    // include & exclude
    auto includeLabel = CCLabelBMFont::create("Include", "bigFont.fnt");
    includeLabel->setPosition({ getContentWidth() / 2 - 185.f, getContentHeight() - 43.f });
    includeLabel->setColor({ 255, 222, 89 });
    includeLabel->setID("include-label");
    includeLabel->setScale(0.3);
    addChild(includeLabel);

    auto excludeLabel = CCLabelBMFont::create("Exclude", "bigFont.fnt");
    excludeLabel->setPosition({ getContentWidth() / 2 + 185.f, getContentHeight() - 43.f });
    excludeLabel->setColor({ 255, 63, 121 });
    excludeLabel->setID("exclude-label");
    excludeLabel->setScale(0.3);
    addChild(excludeLabel);

    auto tagsMenu = CCMenu::create();
    tagsMenu->setPosition({ 0, getContentHeight() - 80 });
    tagsMenu->setContentHeight(30);
    tagsMenu->setID("tags-menu");
    addChild(tagsMenu);

    // include
    auto includeLayer = CCLayer::create();
    includeLayer->setContentSize({ 200, 30 });

    auto includeBg = CCScale9Sprite::create("square07white.png"_spr);
    includeBg->setColor({255, 222, 89});
    includeBg->setContentSize({ 200, 30 });
    includeLayer->addChild(includeBg, 2);

    auto includeGradient = CCLayerGradient::create(ccc4(42, 36, 7, 255), ccc4(25, 12, 12, 255));
    includeGradient->setContentSize({ 198, 28 });
    includeGradient->setPosition({-99.f, -14.f});
    includeGradient->setVector({0, 1});
    includeLayer->addChild(includeGradient, 1);
    
    includeMenu = CCMenu::create();
    includeMenu->setContentSize({ 190, 30 });
    includeMenu->setPosition({0, 0});
    includeMenu->setLayout(AxisLayout::create()->setGap(5)->setDefaultScaleLimits(0.1f, 0.5f));
    includeMenu->setID("include-menu");
    includeMenu->setZOrder(1);
    includeLayer->addChild(includeMenu, 3);

    auto include = CCMenuItemSpriteExtra::create(includeLayer, this, menu_selector(TagsSearch::onTags));
    include->setPosition({ tagsMenu->getContentWidth() / 2 - 105, tagsMenu->getContentHeight() / 2 });
    include->setID("include");
    tagsMenu->addChild(include);

    // exclude
    auto excludeLayer = CCLayer::create();
    excludeLayer->setContentSize({ 200, 30 });

    auto excludeBg = CCScale9Sprite::create("square07white.png"_spr);
    excludeBg->setColor({255, 63, 121});
    excludeBg->setContentSize({ 200, 30 });
    excludeLayer->addChild(excludeBg, 2);

    auto excludeGradient = CCLayerGradient::create(ccc4(62, 15, 29, 255), ccc4(25, 12, 12, 255));
    excludeGradient->setContentSize({ 198, 28 });
    excludeGradient->setPosition({-99.f, -14.f});
    excludeGradient->setVector({0, 1});
    excludeLayer->addChild(excludeGradient, 1);

    excludeMenu = CCMenu::create();
    excludeMenu->setContentSize({ 190, 30 });
    excludeMenu->setPosition({0, 0});
    excludeMenu->setLayout(AxisLayout::create()->setGap(5)->setDefaultScaleLimits(0.1f, 0.5f));
    excludeMenu->setID("exclude-menu");
    excludeMenu->setZOrder(1);
    excludeLayer->addChild(excludeMenu, 3);

    auto exclude = CCMenuItemSpriteExtra::create(excludeLayer, this, menu_selector(TagsSearch::onTags));
    exclude->setPosition({ tagsMenu->getContentWidth() / 2 + 105, tagsMenu->getContentHeight() / 2 });
    exclude->setID("exclude");
    tagsMenu->addChild(exclude);

    // sort by
    auto sortLabel = CCLabelBMFont::create("Sort by", "bigFont.fnt");
    sortLabel->setPosition({ winSize.width / 2 - 115.f, getContentHeight() - 93.f });
    sortLabel->setColor({255, 222, 89});
    sortLabel->setID("sort-label");
    sortLabel->setScale(0.3);
    addChild(sortLabel);

    auto sortMenu = CCMenu::create();
    sortMenu->setPosition({ 0.f, getContentHeight() - 130.f });
    sortMenu->setContentSize({ winSize.width, 30.f });
    sortMenu->setID("sort-menu");
    addChild(sortMenu);

    auto sortSquare = UIsquare::create(true, {130.f, 30.f});
    sortSquare->setPosition({ winSize.width / 2 - 70.f, 15.f });
    sortMenu->addChild(sortSquare);

    sortResultLabel = CCLabelBMFont::create("Recent", "bigFont.fnt");
    sortResultLabel->setColor({ 200, 200, 200 });
    sortResultLabel->setScale(0.5f);
    sortResultLabel->setPosition({ sortMenu->getContentWidth() / 2 - 70.f, sortMenu->getContentHeight() / 2 });
    sortMenu->addChild(sortResultLabel);

    auto sortArrowSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    sortArrowSpr->setScale(0.5f);

    auto sortArrowLeft = CCMenuItemSpriteExtra::create(sortArrowSpr, this, menu_selector(TagsSearch::sortChange));
    sortArrowLeft->setID("sort-left");
    sortArrowLeft->setPosition({ sortMenu->getContentWidth() / 2 - 120.f, sortMenu->getContentHeight() / 2 });
    sortMenu->addChild(sortArrowLeft);

    auto sortArrowFlipped = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    sortArrowFlipped->setScale(0.5f);
    sortArrowFlipped->setFlipX(true);

    auto sortArrowRight = CCMenuItemSpriteExtra::create(sortArrowFlipped, this, menu_selector(TagsSearch::sortChange));
    sortArrowRight->setID("sort-right");
    sortArrowRight->setPosition({ sortMenu->getContentWidth() / 2 - 20.f, sortMenu->getContentHeight() / 2 });
    sortMenu->addChild(sortArrowRight);

    // version
    auto versionLabel = CCLabelBMFont::create("version", "bigFont.fnt");
    versionLabel->setPosition({ winSize.width / 2 + 115.f, getContentHeight() - 93.f });
    versionLabel->setColor({255, 63, 121});
    versionLabel->setID("sort-label");
    versionLabel->setScale(0.3);
    addChild(versionLabel);

    auto versionSquare = UIsquare::create(false, {130.f, 30.f});
    versionSquare->setPosition({ winSize.width / 2 + 70.f, 15.f });
    sortMenu->addChild(versionSquare);

    versionResultLabel = CCLabelBMFont::create("Any", "bigFont.fnt");
    versionResultLabel->setColor({ 200, 200, 200 });
    versionResultLabel->setScale(0.5f);
    versionResultLabel->setPosition({ sortMenu->getContentWidth() / 2 + 70.f, sortMenu->getContentHeight() / 2 });
    sortMenu->addChild(versionResultLabel);

    auto versionArrowSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    versionArrowSpr->setScale(0.5f);

    auto versionArrowLeft = CCMenuItemSpriteExtra::create(versionArrowSpr, this, menu_selector(TagsSearch::versionChange));
    versionArrowLeft->setID("version-left");
    versionArrowLeft->setPosition({ sortMenu->getContentWidth() / 2 + 20.f, sortMenu->getContentHeight() / 2 });
    sortMenu->addChild(versionArrowLeft);

    auto versionArrowFlipped = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    versionArrowFlipped->setScale(0.5f);
    versionArrowFlipped->setFlipX(true);

    auto versionArrowRight = CCMenuItemSpriteExtra::create(versionArrowFlipped, this, menu_selector(TagsSearch::versionChange));
    versionArrowRight->setID("version-right");
    versionArrowRight->setPosition({ sortMenu->getContentWidth() / 2 + 120.f, sortMenu->getContentHeight() / 2 });
    sortMenu->addChild(versionArrowRight);

    auto comingSoon = CCMenuItemSpriteExtra::create(CCSprite::create("filtersComing.png"_spr), this, menu_selector(TagsSearch::kofi));
    comingSoon->setPosition({ getContentWidth() / 2, getContentHeight() - 190.f });
    m_menu->addChild(comingSoon);

    updateTags(true, std::vector<std::string>());
    updateTags(false, std::vector<std::string>());

    setKeyboardEnabled(true);

    return true;
}

void TagsSearch::sortChange(CCObject* sender) {
    std::vector<std::string> sortNames = { "Recent", "Oldest", "Random" };

    auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
    bool isLeft = (btn->getID() == "sort-left");

    sort = (sort + (isLeft ? 2 : 1)) % 3;

    sortResultLabel->setString(sortNames[sort].c_str());
}

void TagsSearch::versionChange(CCObject* sender) {
    std::vector<std::string> versionNames = { "Any", "2.2", "2.1", "2.0", "1.9", "1.8", "1.7", "1.6", "1.5", "1.4", "1.3", "1.2", "1.1", "1.0" };

    auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
    bool isLeft = (btn->getID() == "version-left");

    version = (version + (isLeft ? 13 : 1)) % 14;

    versionResultLabel->setString(versionNames[version].c_str());
};

void TagsSearch::credits(CCObject*) {
    CreditsPopup::create()->show();
}

void TagsSearch::settings(CCObject*) {
    openSettingsPopup(Mod::get());
}

void TagsSearch::kofi(CCObject*) {
    KofiPopup::create()->show();
}

void TagsSearch::onTags(CCObject* sender) {
    auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
    includeSelected = btn->getID() == "include" ? true : false;
    TagsSelect::create(this)->show();
}

void TagsSearch::updateTags(bool include, std::vector<std::string> tags) {
    auto tagMenu = include ? includeMenu : excludeMenu;
    if (!tagMenu) return;
    tagMenu->removeAllChildren();

    if (include) {
        includeTags = tags;
    } else {
        excludeTags = tags;
    };
    if (tags.empty()) {
        auto label = CCLabelBMFont::create("Select Tags", "chatFont.fnt");
        label->setPosition({ tagMenu->getContentWidth() / 2, tagMenu->getContentHeight() / 2 });
        label->setOpacity(25);
        label->setScale(0.8);
        tagMenu->addChild(label);
        return;
    }
    for (const auto& tag : tags) {
        auto tagBtn = TagsManager::addTag(TagsManager::getTagObject(tag), 0.5);
        tagBtn->setID(tag.c_str());
        tagMenu->addChild(tagBtn);
    }
    tagMenu->updateLayout();
    if (tagMenu->getChildrenCount() >= 6) {
        tagMenu->setLayout(AxisLayout::create()->setGap(3)->setGrowCrossAxis(true)->setDefaultScaleLimits(0.25f, 0.25f));
    } else {
        tagMenu->setLayout(AxisLayout::create()->setGap(5)->setDefaultScaleLimits(0.1f, 0.5f));
    }
}

void TagsSearch::search(CCObject*) {
    auto loading = LoadingSpinner::create(20);
    loading->setPosition(searchBtn->getPosition());
    searchBtn->getParent()->addChild(loading, 999);
    searchBtn->setEnabled(false);
    static_cast<CircleButtonSprite*>(searchBtn->getChildren()->objectAtIndex(0))->setColor({50, 50, 50});

    std::string includeStr, excludeStr;
    for (size_t i = 0; i < includeTags.size(); ++i) {
        includeStr += includeTags[i];
        if (i != includeTags.size() - 1) includeStr += ",";
    }
    for (size_t i = 0; i < excludeTags.size(); ++i) {
        excludeStr += excludeTags[i];
        if (i != excludeTags.size() - 1) excludeStr += ",";
    }
    std::string queryStr;
    if (!includeStr.empty()) queryStr += "?i=" + includeStr;
    if (!excludeStr.empty()) {
        if (!queryStr.empty()) {
            queryStr += "&e=" + excludeStr;
        } else {
            queryStr += "?e=" + excludeStr;
        }
    }

    auto req = geode::utils::web::WebRequest();

    m_listener.spawn(
        req.get(fmt::format("{}/search{}", Mod::get()->getSettingValue<std::string>("serverUrl"), queryStr)),
        [this, loading](geode::utils::web::WebResponse value) {
            auto jsonStr = value.string().unwrapOr("{}");
            auto json = matjson::parse(jsonStr).unwrapOr("{}");
            std::vector<int> levels;

            if (json.isArray()) {
                for (const auto& id : json) {
                    int levelID = std::stoi(id.asString().unwrapOr("0"));

                    bool versionMatch = false;

                    const struct VersionRange {
                        int min;
                        int max;
                    } versionRanges[] = {
                        {128, INT_MAX}, // Any
                        {97454398, INT_MAX}, // 2.2
                        {28356243, 97454394}, // 2.1
                        {11020438, 28356225}, // 2.0
                        {2810991, 11020426}, // 1.9
                        {1627371, 2810918}, // 1.8
                        {827316, 1627362}, // 1.7
                        {420781, 827308}, // 1.6
                        {184440, 420780}, // 1.5
                        {121074, 184425}, // 1.4
                        {63419, 121068}, // 1.3
                        {10049, 63415}, // 1.2
                        {1942, 10043}, // 1.1
                        {128, 1941}, // 1.0
                    };

                    if (version == 0) {
                        versionMatch = true;
                    } else if (version >= 1 && version <= 13) {
                        auto range = versionRanges[version];
                        if (levelID >= range.min && levelID <= range.max) {
                            versionMatch = true;
                        }
                    }

                    if (versionMatch) levels.push_back(levelID);
                }

                if (sort == 0) {
                    std::sort(levels.rbegin(), levels.rend());
                } else if (sort == 1) {
                    std::sort(levels.begin(), levels.end());
                } else if (sort == 2) {
                    std::mt19937 rng(std::random_device{}());
                    std::shuffle(levels.begin(), levels.end(), rng);
                }

                int limit = 0;
                std::string resultLevels;
                for (const auto& id : levels) {
                    resultLevels += std::to_string(id) + ",";
                    if (++limit == 100) break;
                }
                if (!resultLevels.empty()) resultLevels.pop_back();

                if (resultLevels.empty()) {
                    Notification::create("No levels found", NotificationIcon::Error, 1)->show();
                } else {
                    CCDirector::sharedDirector()->pushScene(
                        LevelBrowserLayer::scene(GJSearchObject::create(SearchType::Type19, resultLevels))
                    );
                }
                loading->removeFromParent();
                searchBtn->setEnabled(true);
                static_cast<CircleButtonSprite*>(searchBtn->getChildren()->objectAtIndex(0))->setColor({255, 255, 255});
            }
        }
    );
}

CCScene* TagsSearch::scene() {
    auto layer = TagsSearch::create();
    layer->setID("tags-search");
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}

TagsSearch* TagsSearch::create() {
    auto ret = new TagsSearch();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void TagsSearch::keyDown(cocos2d::enumKeyCodes key, double timestamp) {
    if (key == cocos2d::enumKeyCodes::KEY_Escape) this->onBack(nullptr);
    else if (key == cocos2d::enumKeyCodes::KEY_Enter) this->search(nullptr);
    CCLayer::keyDown(key, 0.0);
}

void TagsSearch::onBack(CCObject*) {
    auto director = CCDirector::get();
    director->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}
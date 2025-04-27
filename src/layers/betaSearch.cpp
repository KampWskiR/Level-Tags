#include "betaSearch.hpp"

bool BetaSearch::setup(std::string tag) {
    setTitle("beta search");
    m_title->setScale(1);

    menuPage1 = CCMenu::create();
    menuPage1->setContentSize({400, 270});
    menuPage1->setPosition({0, 0});
    menuPage1->setID("menu-page1");
    m_mainLayer->addChild(menuPage1);

    menuPage2 = CCMenu::create();
    menuPage2->setContentSize({400, 270});
    menuPage2->setPosition({0, 0});
    menuPage2->setVisible(false);
    menuPage2->setID("menu-page2");
    m_mainLayer->addChild(menuPage2);
    
    auto tagsBG = CCScale9Sprite::create("square02b_001.png");
    tagsBG->setContentSize({380, 180});
    tagsBG->setPosition({m_mainLayer->getContentWidth() / 2, 102});
    tagsBG->setColor({ 0, 0, 0 });
    tagsBG->setOpacity(100);
    menuPage1->addChild(tagsBG);

    includeBG = CCMenuItemSpriteExtra::create(CCSprite::create("tagContainer.png"_spr), this, menu_selector(BetaSearch::tagMenuChange));
    includeBG->setPosition({105, 210});
    includeBG->setID("include-bg");
    includeBG->setEnabled(false);
    includeBG->setColor({ 0, 0, 0 });
    includeBG->setOpacity(100);
    menuPage1->addChild(includeBG);

    excludeBG = CCMenuItemSpriteExtra::create(CCSprite::create("tagContainer.png"_spr), this, menu_selector(BetaSearch::tagMenuChange));
    excludeBG->setPosition({295, 210});
    excludeBG->setID("exclude-bg");
    excludeBG->setColor({ 0, 0, 0 });
    excludeBG->setOpacity(100);
    menuPage1->addChild(excludeBG);

    select = CCSprite::create("select.png"_spr);
    select->setPosition({105, 210});
    select->setID("select");
    menuPage1->addChild(select);

    auto includeLabel = CCLabelBMFont::create("Include", "bigFont.fnt");
    includeLabel->setPosition({36, 230});
    includeLabel->setID("include-label");
    includeLabel->setScale(0.3);
    menuPage1->addChild(includeLabel);

    auto excludeLabel = CCLabelBMFont::create("Exclude", "bigFont.fnt");
    excludeLabel->setPosition({225, 230});
    excludeLabel->setID("exclude-label");
    excludeLabel->setScale(0.3);
    menuPage1->addChild(excludeLabel);
    
    auto menu = CCMenu::create();
    menu->setPosition({0, 0});
    menu->setID("menu");
    menu->setContentSize({400, 270});
    m_mainLayer->addChild(menu);

    auto aple = CCMenuItemSpriteExtra::create(CCSprite::create("goodapple.png"_spr), this, menu_selector(BetaSearch::makeVisible));
    aple->setPosition({m_mainLayer->getContentWidth()/2, 80});
    aple->setOpacity(0);
    aple->m_scaleMultiplier = 0.f;
    menuPage2->addChild(aple);

    auto sogy = CCMenuItemSpriteExtra::create(CCSprite::create("yes.png"_spr), this, menu_selector(BetaSearch::makeVisible));
    sogy->setPosition({m_mainLayer->getContentWidth()/2+100, 80});
    sogy->setOpacity(0);
    sogy->m_scaleMultiplier = 0.f;
    menuPage2->addChild(sogy);

    auto glugus = CCMenuItemSpriteExtra::create(CCSprite::create("jpg.png"_spr), this, menu_selector(BetaSearch::makeVisible));
    glugus->setPosition({m_mainLayer->getContentWidth()/2-100, 80});
    glugus->setOpacity(0);
    glugus->m_scaleMultiplier = 0.f;
    menuPage2->addChild(glugus);

    auto infoBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"), this, menu_selector(BetaSearch::info));
    infoBtn->setPosition({375, 27});
    infoBtn->setColor({ 80, 80, 80 });
    infoBtn->setID("info");
    menu->addChild(infoBtn);

    auto creditsSpr = ButtonSprite::create("Credits", 0.5f);
    creditsSpr->setContentHeight(25);
    creditsSpr->m_BGSprite->setContentHeight(25);

    auto creditsBtn = CCMenuItemSpriteExtra::create(creditsSpr, this, menu_selector(BetaSearch::credits));
    creditsBtn->setPosition({menuPage2->getContentWidth() / 2, 20});
    creditsBtn->setID("credits");
    menuPage2->addChild(creditsBtn);

    auto filterBG = CCScale9Sprite::create("square02b_001.png");
    filterBG->setContentSize({380, 90});
    filterBG->setPosition({m_mainLayer->getContentWidth() / 2, 180});
    filterBG->setColor({ 0, 0, 0 });
    filterBG->setOpacity(100);
    menuPage2->addChild(filterBG);

    auto sortMenu = CCMenu::create();
    sortMenu->setPosition({0, 165});
    sortMenu->setID("menu-sort");
    sortMenu->setContentSize({400, 30});
    menuPage2->addChild(sortMenu);

    auto sortLabel = CCLabelBMFont::create("Sort by", "bigFont.fnt");
    sortLabel->setPosition({sortMenu->getContentWidth() / 2, 25});
    sortLabel->setScale(0.35);
    sortMenu->addChild(sortLabel);

    sortText = CCLabelBMFont::create("Recent levels", "bigFont.fnt");
    sortText->setPosition({sortMenu->getContentWidth() / 2, 12});
    sortText->setID("sort");
    sortText->setScale(0.6);
    sortMenu->addChild(sortText);

    auto spr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    spr->setScale(0.5);
    auto leftBtn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(BetaSearch::sortPageDown));
    leftBtn->setPosition({sortMenu->getContentWidth() / 2 - 85, 16});
    sortMenu->addChild(leftBtn);

    auto flippedSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    flippedSpr->setScale(0.5);
    flippedSpr->setFlipX(true);
    auto rightBtn = CCMenuItemSpriteExtra::create(flippedSpr, this, menu_selector(BetaSearch::sortPageUp));
    rightBtn->setPosition({sortMenu->getContentWidth() / 2 + 85, 16});
    sortMenu->addChild(rightBtn);

    auto search = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_longBtn06_001.png"), this, menu_selector(BetaSearch::search));
    search->setPosition({370, 245});
    search->setID("search");
    menu->addChild(search);
    
    // auto savedToggle = CCMenuItemToggler::create(
    //     CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"), CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"), this, menu_selector(BetaSearch::saved)
    // );
    // savedToggle->setID("saved-toggle");
    // savedToggle->setPosition({40, 200});
    // menuPage2->addChild(savedToggle);

    // auto savedLabel = CCLabelBMFont::create("Saved Levels", "bigFont.fnt");
    // savedLabel->setPosition({115, 200});
    // savedLabel->setID("saved-label");
    // savedLabel->setScale(0.5);
    // menuPage2->addChild(savedLabel);

    // auto toggleUncompleted = CCMenuItemToggler::create(
    //     CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"), CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"), this, menu_selector(BetaSearch::uncompleted)
    // );
    // toggleUncompleted->setID("uncompleted-toggle");
    // toggleUncompleted->setPosition({40, 200});
    // menuPage2->addChild(toggleUncompleted);

    // auto uncompletedLabel = CCLabelBMFont::create("Uncompleted", "bigFont.fnt");
    // uncompletedLabel->setPosition({115, 200});
    // uncompletedLabel->setID("uncompleted-label");
    // uncompletedLabel->setScale(0.5);
    // menuPage2->addChild(uncompletedLabel);

    // auto toggleCompleted = CCMenuItemToggler::create(
    //     CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"), CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"), this, menu_selector(BetaSearch::uncompleted)
    // );
    // toggleCompleted->setID("completed-toggle");
    // toggleCompleted->setPosition({40, 160});
    // menuPage2->addChild(toggleCompleted);

    // auto completedLabel = CCLabelBMFont::create("Completed", "bigFont.fnt");
    // completedLabel->setPosition({105, 160});
    // completedLabel->setID("completed-label");
    // completedLabel->setScale(0.5);
    // menuPage2->addChild(completedLabel);

    auto menuInclude = CCMenu::create();
    menuInclude->setContentSize({180, 30});
    menuInclude->setPosition({105, 210});
    menuInclude->setID("menu-include");
    menuInclude->setLayout(AxisLayout::create()->setGap(5)->setDefaultScaleLimits(0.3f, 1.f));
    menuInclude->updateLayout();
    menuPage1->addChild(menuInclude);

    auto menuExclude = CCMenu::create();
    menuExclude->setContentSize({180, 30});
    menuExclude->setPosition({295, 210});
    menuExclude->setID("menu-exclude");
    menuExclude->setLayout(AxisLayout::create()->setGap(5)->setDefaultScaleLimits(0.3f, 1.f));
    menuExclude->updateLayout();
    menuPage1->addChild(menuExclude);

    auto categoryMenu = CCMenu::create();
    categoryMenu->setContentSize({380, 22});
    categoryMenu->setPosition({m_mainLayer->getContentWidth() / 2, 175});
    categoryMenu->setLayout(AxisLayout::create()->setGap(5));
    categoryMenu->setID("category-menu");
    menuPage1->addChild(categoryMenu);

    auto leftPageBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png"), this, menu_selector(BetaSearch::menuChange));
    leftPageBtn->setPosition({-20, 140});
    menu->addChild(leftPageBtn);

    auto flippedPageSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    flippedPageSpr->setFlipX(true);

    auto rightPageBtn = CCMenuItemSpriteExtra::create(flippedPageSpr, this, menu_selector(BetaSearch::menuChange));
    rightPageBtn->setPosition({420, 140});
    menu->addChild(rightPageBtn);

    auto styleBtn = CCSprite::create("styleBtn.png"_spr);
    styleBtn->setScale(0.65);
    auto categoryStyle = CCMenuItemSpriteExtra::create(styleBtn, this, menu_selector(BetaSearch::category));
    categoryStyle->setTag(2);
    categoryMenu->addChild(categoryStyle);

    auto themeBtn = CCSprite::create("themeBtn.png"_spr);
    themeBtn->setScale(0.65);
    auto categoryTheme = CCMenuItemSpriteExtra::create(themeBtn, this, menu_selector(BetaSearch::category));
    categoryTheme->setTag(3);
    categoryMenu->addChild(categoryTheme);

    auto metaBtn = CCSprite::create("metaBtn.png"_spr);
    metaBtn->setScale(0.65);
    auto categoryMeta = CCMenuItemSpriteExtra::create(metaBtn, this, menu_selector(BetaSearch::category));
    categoryMeta->setTag(4);
    categoryMenu->addChild(categoryMeta);

    auto gameplayBtn = CCSprite::create("gameplayBtn.png"_spr);
    gameplayBtn->setScale(0.65);
    auto categoryGameplay = CCMenuItemSpriteExtra::create(gameplayBtn, this, menu_selector(BetaSearch::category));
    categoryGameplay->setTag(5);
    categoryMenu->addChild(categoryGameplay);

    categoryMenu->updateLayout();

    menuStyle = createMenu("style");
    menuPage1->addChild(menuStyle);
    menuStyle->setVisible(true);

    menuTheme = createMenu("theme");
    menuTheme->setContentWidth(440);
    menuTheme->setScale(0.85);
    menuPage1->addChild(menuTheme);

    menuMeta = createMenu("meta");
    menuPage1->addChild(menuMeta);

    menuGameplay = createMenu("gameplay");
    menuGameplay->setContentWidth(440);
    menuGameplay->setScale(0.85);
    menuPage1->addChild(menuGameplay);

    auto json = TagsManager::sharedState()->tags;

    if (!json.contains("style") || !json.contains("theme") || !json.contains("meta") || !json.contains("gameplay")) {
        Notification::create("Failed to retrieve tags from the server.", NotificationIcon::Error, 2)->show();
        return true;
    }

    std::vector<std::tuple<std::string, int, CCMenu*>> categories = {
        {"style", 2, menuStyle}, {"theme", 3, menuTheme}, {"meta", 4, menuMeta}, {"gameplay", 5, menuGameplay}
    };
    
    for (auto const& [category, index, menu] : categories) {
        for (auto const& [key, value] : json[category]) {
            if (TagsManager::sharedState()->getTagObject(key) == matjson::Value()) continue;
            auto spr = CCMenuItemSpriteExtra::create(
                TagsManager::addTag(TagsManager::sharedState()->getTagObject(key), 0.5), this, menu_selector(TagDesc::open)
            );
            auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(BetaSearch::btn));
            btn->setID(key);
            btn->setZOrder(tagZ++);
            btn->setTag(index);
            menu->addChild(btn);
            menu->updateLayout();
        }
    }

    return true;
}

void BetaSearch::makeVisible(CCObject* sender) {
    static_cast<CCMenuItemSpriteExtra*>(sender)->setOpacity(255);
};

void BetaSearch::info(CCObject* sender) {
    auto infoBtn = static_cast<CCMenuItemSpriteExtra*>(sender);
    tagInfoSelected = !tagInfoSelected;
    if (tagInfoSelected) {
        infoBtn->setColor({255, 255, 255});
    } else {
        infoBtn->setColor({80, 80, 80});
    }
};

void BetaSearch::credits(CCObject*) {
    CreditsPopup::create(true)->show();
}

CCMenu* BetaSearch::createMenu(std::string name) {
    CCMenu* menu = CCMenu::create();
    menu->setContentSize({380, 100});
    menu->setAnchorPoint({0.5, 1});
    menu->setPosition({m_mainLayer->getContentWidth() / 2, 160});
    menu->setVisible(false);
    menu->setID(fmt::format("menu-{}", name));
    menu->setLayout(AxisLayout::create()->setAutoScale(false)->setGrowCrossAxis(true)->setGap(2));
    menu->updateLayout();
    return menu;
}

void BetaSearch::uncompleted(CCObject*) {
    uncompletedSelected = !uncompletedSelected;
}

void BetaSearch::saved(CCObject*) {
    savedSelected = !savedSelected;
}

void BetaSearch::sortPageUp(CCObject*) {
    if (sort >= 2) sort = 0;
    else sort++;

    switch (sort) {
        case 0: sortText->setString("Recent levels"); break;
        case 1: sortText->setString("Oldest levels"); break;
        case 2: sortText->setString("Random"); break;
    }
}

void BetaSearch::sortPageDown(CCObject*) {
    if(sort <= 0) sort = 2;
    else sort--;

    switch (sort) {
        case 0: sortText->setString("Recent levels"); break;
        case 1: sortText->setString("Oldest levels"); break;
        case 2: sortText->setString("Random"); break;
    }
}

void BetaSearch::tagMenuChange(CCObject* sender) {
    if (auto category = static_cast<CCMenuItemSpriteExtra*>(sender); category->getID() == "include-bg") {
        includeSelected = true;
        select->setPositionX(105);
        includeBG->setEnabled(false);
        excludeBG->setEnabled(true);
    } else if (category->getID() == "exclude-bg") {
        includeSelected = false;
        select->setPositionX(295);
        includeBG->setEnabled(true);
        excludeBG->setEnabled(false);
    }
}

void BetaSearch::menuChange(CCObject* sender) {
    if (menuPage1->isVisible()) {
        menuPage1->setVisible(false);
        menuPage2->setVisible(true);
        if (auto info = m_mainLayer->getChildByID("menu")->getChildByID("info")) info->setVisible(false);
    } else {
        menuPage1->setVisible(true);
        menuPage2->setVisible(false);
        if (auto info = m_mainLayer->getChildByID("menu")->getChildByID("info")) info->setVisible(true);
    }
}

void BetaSearch::search(CCObject*) {
    auto loading = LoadingSpinner::create(20);
    loading->setPosition({370, 245});
    m_mainLayer->getChildByID("menu")->addChild(loading);
    m_mainLayer->getChildByID("menu")->getChildByID("search")->setVisible(false);

    m_listener.bind([this, loading](web::WebTask::Event* e) {
        if (auto res = e->getValue(); res && res->ok()) {
            auto jsonStr = res->string().unwrapOr("{}");
            auto json = matjson::parse(jsonStr).unwrapOr("{}");
            std::vector<std::pair<int, std::string>> levels;

            if (json.isObject()) {
                for (const auto& [key, value] : json) {
                    std::vector<std::string> tags;
                    if (value.isArray()) {
                        for (const auto& tag : value) {
                            if (tag.isString()) tags.push_back(tag.asString().unwrapOr(""));
                        }
                    }

                    bool includeMatch = include.empty() || std::all_of(include.begin(), include.end(), [&tags](const std::string& inc) {
                        return std::find(tags.begin(), tags.end(), inc) != tags.end();
                    });

                    bool excludeMatch = std::any_of(exclude.begin(), exclude.end(), [&tags](const std::string& exc) {
                        return std::find(tags.begin(), tags.end(), exc) != tags.end();
                    });

                    if (includeMatch && !excludeMatch) {
                        int levelID = std::stoi(key);
                        levels.emplace_back(levelID, key);
                    }
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
                for (const auto& [id, key] : levels) {
                    resultLevels += key + ",";
                    limit++;
                    if (limit == 100) break;
                }
                if (!resultLevels.empty()) resultLevels.pop_back();

                m_mainLayer->getChildByID("menu")->removeChild(loading);
                m_mainLayer->getChildByID("menu")->getChildByID("search")->setVisible(true);

                if (resultLevels.empty()) {
                    Notification::create("No levels found", NotificationIcon::Error, 1)->show();
                } else {
                    CCDirector::sharedDirector()->pushScene(LevelBrowserLayer::scene(GJSearchObject::create(SearchType::Type19, resultLevels)));
                }
            }
        }
    });

    auto req = web::WebRequest();
    m_listener.setFilter(req.get("https://raw.githubusercontent.com/KampWskiR/test3/main/leveltags.json"));
}

void BetaSearch::category(CCObject* sender) {
    CCMenuItemSpriteExtra* category = static_cast<CCMenuItemSpriteExtra*>(sender);
    menuStyle->setVisible(false);
    menuTheme->setVisible(false);
    menuMeta->setVisible(false);
    menuGameplay->setVisible(false);

    switch (category->getTag()) {
        case 2: menuStyle->setVisible(true); break;
        case 3: menuTheme->setVisible(true); break;
        case 4: menuMeta->setVisible(true); break;
        case 5: menuGameplay->setVisible(true); break;
    }
}

void BetaSearch::btn(CCObject* sender) {
    CCMenuItemSpriteExtra* clickedButton = static_cast<CCMenuItemSpriteExtra*>(sender);

    if (tagInfoSelected) {
        TagDesc::create(clickedButton->getID())->show();
        return;
    }
    

    if (clickedButton->getParent()->getID() == "menu-include" || clickedButton->getParent()->getID() == "menu-exclude") {
        if (clickedButton->getParent()->getID() == "menu-include") {
            include.erase(std::remove(include.begin(), include.end(), clickedButton->getID()), include.end());
        } else {
            exclude.erase(std::remove(exclude.begin(), exclude.end(), clickedButton->getID()), exclude.end());
        }
        clickedButton->getParent()->removeChild(clickedButton);
        clickedButton->m_baseScale = 1;
        clickedButton->setScale(1);
        switch (clickedButton->getTag()) {
            case 2: menuStyle->addChild(clickedButton); break;
            case 3: menuTheme->addChild(clickedButton); break;
            case 4: menuMeta->addChild(clickedButton); break;
            case 5: menuGameplay->addChild(clickedButton); break;
        }
        menuStyle->updateLayout();
        menuTheme->updateLayout();
        menuMeta->updateLayout();
        menuGameplay->updateLayout();
    } else {
        if (includeSelected) {
            include.push_back(clickedButton->getID());
        } else {
            exclude.push_back(clickedButton->getID());
        }
        clickedButton->getParent()->removeChild(clickedButton);
        menuPage1->getChildByID(includeSelected ? "menu-include" : "menu-exclude")->addChild(clickedButton);
        menuPage1->getChildByID(includeSelected ? "menu-include" : "menu-exclude")->updateLayout();
    }
    menuStyle->updateLayout();
    menuTheme->updateLayout();
    menuMeta->updateLayout();
    menuGameplay->updateLayout();
    menuPage1->getChildByID(includeSelected ? "menu-include" : "menu-exclude")->updateLayout();
}

BetaSearch* BetaSearch::create(std::string tag) {
    auto ret = new BetaSearch();
    if (ret->initAnchored(400.f, 270.f, tag, "square.png"_spr)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

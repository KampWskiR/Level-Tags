#include "tagsSelect.hpp"

bool TagsSelect::init(TagsSearch* search) {
    if (!Popup::init(420.f, 270.f, "square.png"_spr)) return false;
    m_search = search;
    setTitle(search->includeSelected ? "Include Tags" : "Exclude Tags");
    m_title->setPositionY(255);

    auto closeSpr = CCSprite::create();
    closeSpr->setContentSize({80, 25});

    auto closeBG = UIsquare::create(!search->includeSelected, {80, 25});
    closeBG->setPosition({closeSpr->getContentWidth() / 2, closeSpr->getContentHeight() / 2});
    closeSpr->addChild(closeBG);
    
    auto closeLabel = CCLabelBMFont::create("done", "bigFont.fnt");
    closeLabel->setScale(0.6f);
    closeLabel->setPosition({closeSpr->getContentWidth() / 2, closeSpr->getContentHeight() / 2 + 2});
    closeSpr->addChild(closeLabel);

    m_closeBtn->setSprite(closeSpr);
    m_closeBtn->setPosition({m_mainLayer->getContentWidth() / 2, 23});

    auto stencil = CCLayerColor::create(ccc4(0, 0, 0, 255), m_mainLayer->getContentWidth(), m_mainLayer->getContentHeight());
    stencil->setAnchorPoint({0, 0});
    stencil->setPosition({0, 0});

    auto clipper = CCClippingNode::create(stencil);
    clipper->setAnchorPoint({0.5f, 0.5f});
    clipper->setPosition({0, 0});

    auto texture = CCTextureCache::sharedTextureCache()->addImage("tagsBG.png"_spr, false);
    ccTexParams texParams = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
    texture->setTexParameters(&texParams);

    auto bgTexture = CCSprite::createWithTexture(texture, CCRectMake(0, 0, texture->m_uPixelsWide * 2, texture->m_uPixelsHigh * 2));
    bgTexture->setAnchorPoint(ccp(0.5f, 0.5f));
    bgTexture->setPosition({0.f, 0.f});
    bgTexture->setOpacity(25);

    clipper->addChild(bgTexture);

    m_mainLayer->addChild(clipper);

    auto bgMove = CCMoveBy::create(10.0f, {272.f, -277.f});
    auto bgReset = CCMoveTo::create(0.0f, {0.f, 0.f});
    auto bgSeq = CCSequence::create(bgMove, bgReset, nullptr);
    auto bgRepeat = CCRepeatForever::create(bgSeq);
    bgTexture->runAction(bgRepeat);

    for (int i = 0; i < 4; ++i) {
        auto corner = CCSprite::create("corner.png"_spr);
        corner->setRotation(90 * i);
        corner->setPosition({ (i < 2 ? 24.f : 396.f), (i % 3 == 0 ? 23.8f : 246.3f) });
        m_mainLayer->addChild(corner);
    }

    auto infoBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"), this, menu_selector(TagsSelect::info));
    infoBtn->setPosition({25,25});
    infoBtn->setColor({ 80, 80, 80 });
    infoBtn->setID("info");
    m_closeBtn->getParent()->addChild(infoBtn);

    auto selectBg = UIsquare::create(!search->includeSelected, {380, 30});
    selectBg->setPosition({m_mainLayer->getContentWidth() / 2, 227});
    m_mainLayer->addChild(selectBg);

    auto tagsMenu = CCMenu::create();
    tagsMenu->setContentSize({360, 30});
    tagsMenu->setPosition({m_mainLayer->getContentWidth() / 2, 227});
    tagsMenu->setID("tags-menu");
    tagsMenu->setLayout(AxisLayout::create()->setGap(5)->setDefaultScaleLimits(0.3f, 1.f));
    tagsMenu->updateLayout();
    m_mainLayer->addChild(tagsMenu);

    auto tagsBG = UIsquare::create(search->includeSelected, {380, 165});
    tagsBG->setPosition({m_mainLayer->getContentWidth() / 2, 125});
    m_mainLayer->addChild(tagsBG);

    auto searchBG = UIsquare::create(!search->includeSelected, {80, 22});
    searchBG->setPosition({349, 192});
    m_mainLayer->addChild(searchBG);

    auto categoryMenu = CCMenu::create();
    categoryMenu->setContentSize({380, 22});
    categoryMenu->setPosition({m_mainLayer->getContentWidth() / 2, 192});
    categoryMenu->setLayout(AxisLayout::create()->setGap(5));
    categoryMenu->setID("category-menu");
    m_mainLayer->addChild(categoryMenu);

    auto styleBtn = CCSprite::create("styleBtn.png"_spr);
    styleBtn->setScale(0.65);
    auto categoryStyle = CCMenuItemSpriteExtra::create(styleBtn, this, menu_selector(TagsSelect::category));
    categoryStyle->setTag(0);
    categoryMenu->addChild(categoryStyle);

    auto themeBtn = CCSprite::create("themeBtn.png"_spr);
    themeBtn->setScale(0.65);
    auto categoryTheme = CCMenuItemSpriteExtra::create(themeBtn, this, menu_selector(TagsSelect::category));
    categoryTheme->setTag(1);
    categoryMenu->addChild(categoryTheme);

    auto metaBtn = CCSprite::create("metaBtn.png"_spr);
    metaBtn->setScale(0.65);
    auto categoryMeta = CCMenuItemSpriteExtra::create(metaBtn, this, menu_selector(TagsSelect::category));
    categoryMeta->setTag(2);
    categoryMenu->addChild(categoryMeta);

    auto gameplayBtn = CCSprite::create("gameplayBtn.png"_spr);
    gameplayBtn->setScale(0.65);
    auto categoryGameplay = CCMenuItemSpriteExtra::create(gameplayBtn, this, menu_selector(TagsSelect::category));
    categoryGameplay->setTag(3);
    categoryMenu->addChild(categoryGameplay);

    searchInput = TextInput::create(80.f, "search tag");
    searchInput->setDelegate(this);
    searchInput->hideBG();
    categoryMenu->addChild(searchInput);

    categoryMenu->updateLayout();

    searchInput->setScale(0.7f);
    searchInput->setWidth(110.f);

    auto line = CCSprite::createWithSpriteFrameName("edit_vLine_001.png");
    line->setRotation(90.f);
    line->setScaleX(0.5f);
    line->setScaleY(4.5f);
    line->setPosition({ m_mainLayer->getContentWidth() / 2, 176 });
    m_mainLayer->addChild(line);

    menuStyle = createMenu("style");
    m_mainLayer->addChild(menuStyle);
    menuStyle->setVisible(true);

    menuTheme = createMenu("theme");
    menuTheme->setContentWidth(440);
    menuTheme->setScale(0.85);
    m_mainLayer->addChild(menuTheme);

    menuMeta = createMenu("meta");
    m_mainLayer->addChild(menuMeta);

    menuGameplay = createMenu("gameplay");
    menuGameplay->setContentWidth(440);
    menuGameplay->setScale(0.85);
    m_mainLayer->addChild(menuGameplay);

    auto json = TagsManager::sharedState()->tags;

    if (!json.contains("style") || !json.contains("theme") || !json.contains("meta") || !json.contains("gameplay")) {
        Notification::create("Failed to retrieve tags from the server.", NotificationIcon::Error, 2)->show();
        return true;
    }

    std::vector<std::tuple<std::string, int, CCMenu*>> categories = {
        {"style", 0, menuStyle}, {"theme", 1, menuTheme}, {"meta", 2, menuMeta}, {"gameplay", 3, menuGameplay}
    };
    
    for (auto const& [category, index, menu] : categories) {
        for (auto const& [key, value] : json[category]) {
            if (TagsManager::sharedState()->getTagObject(key) == matjson::Value()) continue;
            auto spr = TagsManager::addTag(TagsManager::sharedState()->getTagObject(key), 0.5);
            auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(TagsSelect::onTag));
            btn->setID(key);
            btn->setZOrder(tagZ++);
            btn->setTag(index);
            const auto& tags = search->includeSelected ? search->excludeTags : search->includeTags;
            if (std::find(tags.begin(), tags.end(), key) != tags.end()) {
                btn->setEnabled(false);
                auto color = spr->getColor();
                spr->setColor({static_cast<GLubyte>(color.r * 0.3f), static_cast<GLubyte>(color.g * 0.3f), static_cast<GLubyte>(color.b * 0.3f)});
            }
            menu->addChild(btn);
            menu->updateLayout();
        }
    }

    for (const auto& tag : (search->includeSelected ? search->includeTags : search->excludeTags)) {
        if (menuStyle->getChildByID(tag)) onTag(menuStyle->getChildByID(tag));
        if (menuTheme->getChildByID(tag)) onTag(menuTheme->getChildByID(tag));
        if (menuMeta->getChildByID(tag)) onTag(menuMeta->getChildByID(tag));
        if (menuGameplay->getChildByID(tag)) onTag(menuGameplay->getChildByID(tag));
    }

    return true;
}

CCMenu* TagsSelect::createMenu(std::string name) {
    auto layout = AxisLayout::create();
    layout->ignoreInvisibleChildren(true);
    layout->setAutoScale(false);
    layout->setGrowCrossAxis(true);
    layout->setGap(2);

    CCMenu* menu = CCMenu::create();
    menu->setContentSize({380, 100});
    menu->setAnchorPoint({0.5, 1});
    menu->setPosition({m_mainLayer->getContentWidth() / 2, 172});
    menu->setVisible(false);
    menu->setID(fmt::format("menu-{}", name));
    menu->setLayout(layout);
    menu->updateLayout();
    return menu;
}

void TagsSelect::category(CCObject* sender) {
    CCMenuItemSpriteExtra* category = static_cast<CCMenuItemSpriteExtra*>(sender);
    menuStyle->setVisible(false);
    menuTheme->setVisible(false);
    menuMeta->setVisible(false);
    menuGameplay->setVisible(false);

    switch (category->getTag()) {
        case 0: menuStyle->setVisible(true); break;
        case 1: menuTheme->setVisible(true); break;
        case 2: menuMeta->setVisible(true); break;
        case 3: menuGameplay->setVisible(true); break;
    }
}

void TagsSelect::onTag(CCObject* sender) {
    CCMenuItemSpriteExtra* clickedButton = static_cast<CCMenuItemSpriteExtra*>(sender);

    if (tagInfoSelected) {
        TagDesc::create(clickedButton->getID())->show();
        return;
    }

    if (clickedButton->getParent()->getID() == "tags-menu") {
        tagMenu.erase(std::remove(tagMenu.begin(), tagMenu.end(), clickedButton->getID()), tagMenu.end());
        clickedButton->getParent()->removeChild(clickedButton);
        clickedButton->m_baseScale = 1;
        clickedButton->setScale(1);
        switch (clickedButton->getTag()) {
            case 0: menuStyle->addChild(clickedButton); break;
            case 1: menuTheme->addChild(clickedButton); break;
            case 2: menuMeta->addChild(clickedButton); break;
            case 3: menuGameplay->addChild(clickedButton); break;
        }
        menuStyle->updateLayout();
        menuTheme->updateLayout();
        menuMeta->updateLayout();
        menuGameplay->updateLayout();
    } else {
        if (m_mainLayer->getChildByID("tags-menu")->getChildrenCount() >= 10) return;
        tagMenu.push_back(clickedButton->getID());
        clickedButton->getParent()->removeChild(clickedButton);
        m_mainLayer->getChildByID("tags-menu")->addChild(clickedButton);
        m_mainLayer->getChildByID("tags-menu")->updateLayout();
    }
    menuStyle->updateLayout();
    menuTheme->updateLayout();
    menuMeta->updateLayout();
    menuGameplay->updateLayout();
    m_mainLayer->getChildByID("tags-menu")->updateLayout();
}

void TagsSelect::info(CCObject* sender) {
    auto infoBtn = static_cast<CCMenuItemSpriteExtra*>(sender);
    tagInfoSelected = !tagInfoSelected;
    infoBtn->setColor(tagInfoSelected ? ccColor3B(255, 255, 255) : ccColor3B(80, 80, 80));
}

void TagsSelect::textChanged(CCTextInputNode* input) {
    std::string query = input->getString();
    std::transform(query.begin(), query.end(), query.begin(), ::tolower);

    for (CCMenu* menu : {menuStyle, menuTheme, menuMeta, menuGameplay}) {
        auto children = menu->getChildren();
        for (int i = 0; i < children->count(); ++i) {
            auto item = static_cast<CCMenuItemSpriteExtra*>(children->objectAtIndex(i));
            item->setVisible(std::string(item->getID()).find(query) != std::string::npos);
        }
        menu->updateLayout();
    }
}

void TagsSelect::onClose(CCObject*) {
    auto tags = m_mainLayer->getChildByID("tags-menu")->getChildren();
    std::vector<std::string> tagsSelected;
    for (int i = 0; i < m_mainLayer->getChildByID("tags-menu")->getChildrenCount(); i++) {
        auto tag = static_cast<CCMenuItemSpriteExtra*>(tags->objectAtIndex(i));
        if (tag->getParent()->getID() == "tags-menu") tagsSelected.push_back(tag->getID());
    }
    m_search->updateTags(m_search->includeSelected, tagsSelected);
    Popup::onClose(nullptr);
}

TagsSelect* TagsSelect::create(TagsSearch* search) {
    auto popup = new TagsSelect;
    if (popup->init(search)) {
        popup->autorelease();
        return popup;
    }
    delete popup;
    return nullptr;
}
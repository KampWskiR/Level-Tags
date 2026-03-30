#include "tagDesc.hpp"

bool TagDesc::init(std::string tag) {
    if (!Popup::init(305.f, 120.f, "square.png"_spr)) return false;
    matjson::Value tagObject = TagsManager::sharedState()->getTagObject(tag)[tag];
    ccColor3B color = tagObject[1].as<ccColor3B>().unwrapOr(ccColor3B{ 0, 0, 0 });
    std::string description = tagObject[2].as<std::string>().unwrapOr("no description provided");

    auto closeSpr = CCSprite::create();
    closeSpr->setContentSize({60, 20});

    auto closeBG = UIsquare::create(true, {60, 20});
    closeBG->setPosition({closeSpr->getContentWidth() / 2, closeSpr->getContentHeight() / 2});
    closeSpr->addChild(closeBG);
    
    auto closeLabel = CCLabelBMFont::create("close", "bigFont.fnt");
    closeLabel->setScale(0.4f);
    closeLabel->setPosition({closeSpr->getContentWidth() / 2, closeSpr->getContentHeight() / 2 + 1});
    closeSpr->addChild(closeLabel);

    m_closeBtn->setSprite(closeSpr);
    m_closeBtn->setPosition({m_mainLayer->getContentWidth() / 2, 20.f});

    m_mainLayer->addChild(TagsManager::addBgAnim(m_size));
    m_mainLayer->addChild(TagsManager::addCorners(m_size, 0.75f));

    auto iconBg = UIsquare::create(true, {70, 70});
    iconBg->setPosition({60, 70});
    m_mainLayer->addChild(iconBg);

    auto descMenu = UIsquare::create(false, {180, 70});
    descMenu->setPosition({190, 70});
    m_mainLayer->addChild(descMenu);

    auto tagLabel = CCLabelBMFont::create(tag.c_str(), "bigFont.fnt");
    tagLabel->limitLabelWidth(50, 0.5, 0.2);
    tagLabel->setPosition({60, 55});
    tagLabel->setColor(color);
    m_mainLayer->addChild(tagLabel);

    auto tagIcon = CCSprite::createWithSpriteFrameName(fmt::format("{}"_spr, tag + ".png").c_str());
    if (!tagIcon || tagIcon->getUserObject("geode.texture-loader/fallback")) tagIcon = CCSprite::create("404.png"_spr);
    tagIcon->setColor(color);
    tagIcon->setPosition({60, 78});
    if (tagIcon) tagIcon->setScale(0.5);
    m_mainLayer->addChild(tagIcon);

    auto desc = SimpleTextArea::create(description.c_str(), "chatFont.fnt", 0.65f, 160);
    desc->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
    desc->setPosition({190, 70});
    desc->setColor({255, 255, 255, 255});
    m_mainLayer->addChild(desc);

    return true;
}

TagDesc* TagDesc::create(std::string tag) {
    auto popup = new TagDesc;
    if (popup->init(tag)) {
        popup->autorelease();
        return popup;
    }
    delete popup;
    return nullptr;
}

void TagDesc::open(CCObject* sender) {
    CCMenuItemSpriteExtra* btn = static_cast<CCMenuItemSpriteExtra*>(sender);
    if (TagsManager::sharedState()->getTagObject(btn->getID().c_str()) == matjson::Value()) return;
    create(btn->getID().c_str())->show();
}

#include "tagDesc.hpp"

bool TagDesc::setup(std::string tag) {
    matjson::Value tagObject = TagsManager::sharedState()->getTagObject(tag)[tag];
    ccColor3B color = tagObject[1].as<ccColor3B>().unwrapOr(ccColor3B(0, 0, 0));
    std::string description = tagObject[2].as<std::string>().unwrapOr("no description provided");
    auto iconBg = CCScale9Sprite::create("square02b_001.png");
    iconBg->setContentSize({70, 70});
    iconBg->setPosition({50, m_mainLayer->getContentHeight() / 2});
    iconBg->setColor({0, 0, 0});
    iconBg->setOpacity(60);
    m_mainLayer->addChild(iconBg);

    auto descMenu = CCScale9Sprite::create("square02b_001.png");
    descMenu->setContentSize({200, 70});
    descMenu->setPosition({190, m_mainLayer->getContentHeight() / 2});
    descMenu->setColor({0, 0, 0});
    descMenu->setOpacity(60);
    m_mainLayer->addChild(descMenu);

    auto tagLabel = CCLabelBMFont::create(tag.c_str(), "bigFont.fnt");
    tagLabel->limitLabelWidth(50, 0.5, 0.2);
    tagLabel->setPosition({50, 35});
    tagLabel->setColor(color);
    m_mainLayer->addChild(tagLabel);

    auto tagIcon = CCSprite::createWithSpriteFrameName(fmt::format("{}"_spr, tag + ".png").c_str());
    if (!tagIcon || tagIcon->getUserObject("geode.texture-loader/fallback")) tagIcon = CCSprite::create("404.png"_spr);
    tagIcon->setColor(color);
    tagIcon->setPosition({50, 58});
    if (tagIcon) tagIcon->setScale(0.5);
    m_mainLayer->addChild(tagIcon);

    auto desc = SimpleTextArea::create(description.c_str(), "bigFont.fnt", 0.25, 180);
    desc->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
    desc->setPosition({190, m_mainLayer->getContentHeight() / 2});
    desc->setColor({color.r, color.g, color.b, 255});
    m_mainLayer->addChild(desc);

    return true;
}

TagDesc* TagDesc::create(std::string tag) {
    auto ret = new TagDesc();
    if (ret->initAnchored(300.f, 100.f, tag, "square.png"_spr)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void TagDesc::open(CCObject* sender) {
    CCMenuItemSpriteExtra* btn = static_cast<CCMenuItemSpriteExtra*>(sender);
    if (TagsManager::sharedState()->getTagObject(btn->getID().c_str()) == matjson::Value()) return;
    create(btn->getID().c_str())->show();
}

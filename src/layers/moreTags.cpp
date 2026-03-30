#include "moreTags.hpp"

bool MoreTags::init(matjson::Value tags) {
    if (!Popup::init(280.f, 20.f, "square.png"_spr)) return false;
    auto tagMenu = CCMenu::create();
    tagMenu->setContentWidth(235);
    tagMenu->setPosition({m_mainLayer->getContentWidth() / 2, 95});
    tagMenu->setAnchorPoint({0.5, 1});
    tagMenu->setID("level-tags");
    tagMenu->setLayout(RowLayout::create()->setGrowCrossAxis(true)->setAutoScale(false)->setGap(2));
    m_mainLayer->addChild(tagMenu, 1);

    if (!tags.isNull() && tags.isArray()) {
        for (const auto& tag : tags) {
            if (TagsManager::sharedState()->getTagObject(tag.asString().unwrapOr("")) == matjson::Value()) continue;
            auto tagNode = CCMenuItemSpriteExtra::create(
                TagsManager::addTag(TagsManager::sharedState()->getTagObject(tag.asString().unwrapOr("")), 0.35), this, menu_selector(TagDesc::open)
            );
            tagNode->setID(tag.asString().unwrapOr(""));
            tagMenu->addChild(tagNode);
            tagMenu->updateLayout();
        }
        m_mainLayer->setContentHeight(55.f + tagMenu->getContentHeight());
        m_mainLayer->updateLayout();
        tagMenu->setPositionY(m_mainLayer->getContentHeight() - 15.f);

        auto closeSpr = CCSprite::create();
        closeSpr->setContentSize({80, 25});

        auto closeBG = UIsquare::create(false, {70, 20});
        closeBG->setPosition({closeSpr->getContentWidth() / 2, closeSpr->getContentHeight() / 2});
        closeSpr->addChild(closeBG);
            
        auto closeLabel = CCLabelBMFont::create("close", "bigFont.fnt");
        closeLabel->setScale(0.5f);
        closeLabel->setPosition({closeSpr->getContentWidth() / 2, closeSpr->getContentHeight() / 2 + 2});
        closeSpr->addChild(closeLabel);

        m_closeBtn->setSprite(closeSpr);
        m_closeBtn->setPosition({m_mainLayer->getContentWidth() / 2, 20});
    }

    auto descMenu = UIsquare::create(true, {240, tagMenu->getContentHeight() + 10.f});
    descMenu->setPosition({m_mainLayer->getContentWidth() / 2, m_mainLayer->getContentHeight() - 10.f});
    descMenu->setAnchorPoint({0.5, 1});
    m_mainLayer->addChild(descMenu, 0);

    auto bgAnim = TagsManager::sharedState()->addBgAnim(m_mainLayer->getContentSize());
    bgAnim->setZOrder(-1);
    m_mainLayer->addChild(bgAnim);
    m_bgSprite->setZOrder(-2);

    for (int i = 0; i < 4; ++i) {
        auto corner = CCSprite::create("corner.png"_spr);
        corner->setRotation(90 * i);
        corner->setPosition({ (i < 2 ? 24.f : 256.f), (i % 3 == 0 ? 23.8f : m_mainLayer->getContentHeight() - 23.7f) });
        m_mainLayer->addChild(corner);
    }

    return true;
}

MoreTags* MoreTags::create(matjson::Value tags) {
    auto popup = new MoreTags;
    if (popup->init(tags)) {
        popup->autorelease();
        return popup;
    }
    delete popup;
    return nullptr;
}
#include "discordPopup.hpp"

bool DiscordPopup::init() {
    if (!Popup::init(240.f, 120.f, "square.png"_spr)) return false;
    setTitle("Join Discord");
    m_title->setScale(0.8f);

    m_mainLayer->addChild(TagsManager::sharedState()->addBgAnim(m_size));
    m_mainLayer->addChild(TagsManager::sharedState()->addCorners(m_size, 1.0f));

    auto bg = CCScale9Sprite::create("square02b_001.png");
    bg->setContentSize({180.f, 40.f});
    bg->setPosition({m_mainLayer->getContentWidth() / 2, 63.f});
    bg->setColor({ 0, 0, 0 });
    bg->setOpacity(100);  
    m_mainLayer->addChild(bg);

    auto label = CCLabelBMFont::create("Join our Discord server\nto request level tags!", "bigFont.fnt");
    label->setPosition({m_mainLayer->getContentWidth() / 2, 64.f});
    label->setScale(0.35f);
    m_mainLayer->addChild(label);

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
    m_closeBtn->setPosition({m_mainLayer->getContentWidth() / 2 - 35.f, 20.f});

    auto joinSpr = CCSprite::create();
    joinSpr->setContentSize({60, 20});

    auto joinBG = UIsquare::create(false, {60, 20});
    joinBG->setPosition({joinSpr->getContentWidth() / 2, joinSpr->getContentHeight() / 2});
    joinSpr->addChild(joinBG);
    
    auto joinLabel = CCLabelBMFont::create("join!", "bigFont.fnt");
    joinLabel->setScale(0.4f);
    joinLabel->setPosition({joinSpr->getContentWidth() / 2, joinSpr->getContentHeight() / 2 + 1});
    joinSpr->addChild(joinLabel);

    auto joinBtn = CCMenuItemSpriteExtra::create(joinSpr, this, menu_selector(DiscordPopup::join));
    joinBtn->setPosition({m_mainLayer->getContentWidth() / 2 + 35.f, 20.f});
    m_closeBtn->getParent()->addChild(joinBtn);

    auto up = CCEaseInOut::create(CCScaleTo::create(1.f, 1.15f), 2.0f);
    auto down = CCEaseInOut::create(CCScaleTo::create(1.f, 1.f), 2.0f);
    auto btnSeq = CCSequence::create(up, down, nullptr);
    auto btnRepeat = CCRepeatForever::create(btnSeq);
    joinSpr->runAction(btnRepeat);

    return true;
}

void DiscordPopup::join(CCObject*) {
    web::openLinkInBrowser("https://discord.gg/6GXYHf9WTB");
    onClose(this);
}

DiscordPopup* DiscordPopup::create() {
    auto popup = new DiscordPopup;
    if (popup->init()) {
        popup->autorelease();
        return popup;
    }
    delete popup;
    return nullptr;
}
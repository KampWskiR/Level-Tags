#include "kofiPopup.hpp"

bool KofiPopup::init() {
    if (!Popup::init(280.f, 160.f, "square.png"_spr)) return false;
    setTitle("Support us on kofi!");
    m_title->setScale(0.75f);

    m_mainLayer->addChild(TagsManager::sharedState()->addBgAnim(m_size));
    m_mainLayer->addChild(TagsManager::sharedState()->addCorners(m_size, 1.0f));

    auto bg = CCScale9Sprite::create("square02b_001.png");
    bg->setContentSize({240.f, 85.f});
    bg->setPosition({m_mainLayer->getContentWidth() / 2, 80.f});
    bg->setColor({ 0, 0, 0 });
    bg->setOpacity(100);  
    m_mainLayer->addChild(bg);

    auto label = CCLabelBMFont::create("By becoming a Supporter, you'll get:", "bigFont.fnt");
    label->setPosition({m_mainLayer->getContentWidth() / 2, 110.f});
    label->setScale(0.35f);
    m_mainLayer->addChild(label);

    auto perks = CCLabelBMFont::create(
        "- Request Priority.\n"
        "- Early Access to the mod updates.\n"
        "- A Supporter Discord role\n"
        "- A mention in the Credits page.", "chatFont.fnt"
    );
    perks->setPosition({m_mainLayer->getContentWidth() / 2, 72.f});
    perks->setScale(0.7f);
    perks->setAlignment(kCCTextAlignmentLeft);
    m_mainLayer->addChild(perks);

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
    m_closeBtn->setPosition({m_mainLayer->getContentWidth() / 2 - 45.f, 20.f});

    auto openSpr = CCSprite::create();
    openSpr->setContentSize({60, 20});

    auto openBG = UIsquare::create(false, {80, 20});
    openBG->setPosition({openSpr->getContentWidth() / 2, openSpr->getContentHeight() / 2});
    openSpr->addChild(openBG);
    
    auto openLabel = CCLabelBMFont::create("open kofi", "bigFont.fnt");
    openLabel->setScale(0.4f);
    openLabel->setPosition({openSpr->getContentWidth() / 2, openSpr->getContentHeight() / 2 + 1});
    openSpr->addChild(openLabel);

    auto joinBtn = CCMenuItemSpriteExtra::create(openSpr, this, menu_selector(KofiPopup::open));
    joinBtn->setPosition({m_mainLayer->getContentWidth() / 2 + 40.f, 20.f});
    m_closeBtn->getParent()->addChild(joinBtn);

    auto up = CCEaseInOut::create(CCScaleTo::create(1.f, 1.15f), 2.0f);
    auto down = CCEaseInOut::create(CCScaleTo::create(1.f, 1.f), 2.0f);
    auto btnSeq = CCSequence::create(up, down, nullptr);
    auto btnRepeat = CCRepeatForever::create(btnSeq);
    openSpr->runAction(btnRepeat);

    return true;
}

void KofiPopup::open(CCObject*) {
    web::openLinkInBrowser("https://ko-fi.com/leveltags");
    onClose(this);
}

KofiPopup* KofiPopup::create() {
    auto popup = new KofiPopup;
    if (popup->init()) {
        popup->autorelease();
        return popup;
    }
    delete popup;
    return nullptr;
}
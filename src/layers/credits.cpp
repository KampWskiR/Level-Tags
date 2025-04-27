#include "credits.hpp"

bool CreditsPopup::setup(bool a) {
    setTitle("Credits");
    m_title->setPositionY(203);
    m_title->setScale(1);

    matjson::Value staff = matjson::parse(R"({
        "kampwski": ["developer", 8776613],
        "nnexterr": ["admin", 1249399],
        "MrNexis": ["moderator", 99199],
        "mariokirby1703": ["moderator", 4685259],
        "PAHC": ["moderator", 5629061],
        "Vexowy": ["moderator", 8672482],
        "ItzCyberCube": ["moderator", 10895417],
        "SkoeGMD": ["moderator", 6631358],
        "Hidden4125": ["moderator", 6391509],
        "pafciu": ["helper", 9282706],
        "dpoopoop": ["helper", 9058574],
        "Lyaten": ["helper", 26329404]
    })").unwrapOr("{}");

    auto s1 = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
    s1->setPosition({25, 25});
    m_mainLayer->addChild(s1);

    auto s2 = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
    s2->setPosition({335, 25});
    s2->setFlipX(true);
    m_mainLayer->addChild(s2);

    auto s3 = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
    s3->setPosition({25, 195});
    s3->setFlipY(true);
    m_mainLayer->addChild(s3);

    auto s4 = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
    s4->setPosition({335, 195});
    s4->setFlipX(true);
    s4->setFlipY(true);
    m_mainLayer->addChild(s4);

    auto descMenu = CCScale9Sprite::create("square02b_001.png");
    descMenu->setContentSize({320, 160});
    descMenu->setPosition({m_mainLayer->getContentWidth() / 2, 185});
    descMenu->setAnchorPoint({0.5, 1});
    descMenu->setColor({117, 65, 39});
    m_mainLayer->addChild(descMenu);

    auto staffLabel = CCLabelBMFont::create("Staff", "bigFont.fnt");
    staffLabel->setPosition({m_mainLayer->getContentWidth() / 2, 178});
    staffLabel->setScale(0.35);
    m_mainLayer->addChild(staffLabel);

    auto menu = CCMenu::create();
    menu->setPosition({m_mainLayer->getContentWidth() / 2, 124});
    menu->setContentWidth(320);
    menu->setLayout(AxisLayout::create()->setGrowCrossAxis(true)->setAutoScale(false)->setGap(8));
    menu->setID("credits-menu");
    m_mainLayer->addChild(menu);

    for (const auto& [key, value] : staff) {
        auto btn = CCMenuItemSpriteExtra::create( tabSprite(key, value[0].asString().unwrapOr("")), this, menu_selector(CreditsPopup::btn));
        btn->setID(key);
        btn->setTag(value[1].asInt().unwrapOr(0));
        menu->addChild(btn);
    }
    menu->updateLayout();

    auto moreLabel = CCLabelBMFont::create("More", "bigFont.fnt");
    moreLabel->setPosition({m_mainLayer->getContentWidth() / 2, 70});
    moreLabel->setScale(0.35);
    m_mainLayer->addChild(moreLabel);

    auto colon = CCMenuItemSpriteExtra::create( tabSprite("Colon", "Mod Inspiration"), this, menu_selector(CreditsPopup::btn));
    colon->setID("Colon");
    colon->setTag(106255);
    colon->setPosition({120, -32});
    menu->addChild(colon);

    auto docs = CCMenuItemSpriteExtra::create( tabSprite("GD Styles List", "Mod Inspiration"), this, menu_selector(CreditsPopup::docs));
    docs->setID("Docs");
    docs->setPosition({200, -32});
    menu->addChild(docs);

    return true;
}

CCNode* CreditsPopup::tabSprite(std::string name, std::string role) {
    auto layer = CCNode::create();
    layer->setPosition({0,0});
    layer->setContentSize({70, 25});

    auto bg = CCScale9Sprite::create("square02b_001.png");
    bg->setContentSize({140, 50});
    bg->setScale(0.5);
    bg->setPosition({layer->getContentWidth() / 2, layer->getContentHeight() / 2});
    bg->setColor({ 89, 49, 29 });
    layer->addChild(bg);

    auto label = CCLabelBMFont::create(name.c_str(), "goldFont.fnt");
    label->setPosition({layer->getContentWidth() / 2, 18});
    label->limitLabelWidth(60, 0.6, 0.2);
    layer->addChild(label);

    ccColor3B color = { 255, 255, 255 };
    if (role == "developer") color = { 255, 150, 230 };
    else if (role == "admin") color = { 255, 50, 50 };
    else if (role == "moderator") color = { 200, 100, 0 };
    else if (role == "helper") color = { 0, 200, 255 };

    auto roleLabel = CCLabelBMFont::create(role.c_str(), "bigFont.fnt");
    roleLabel->setPosition({layer->getContentWidth() / 2, 8});
    roleLabel->setColor(color);
    roleLabel->limitLabelWidth(60, 0.25, 0.2);
    layer->addChild(roleLabel);

    return layer;
}

CreditsPopup* CreditsPopup::create(bool a) {
    auto ret = new CreditsPopup();
    if (ret->initAnchored(360.f, 220.f, a, "square.png"_spr)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void CreditsPopup::btn(CCObject* sender) {
    ProfilePage::create(sender->getTag(), false)->show();
}

void CreditsPopup::docs(CCObject* sender) {
    web::openLinkInBrowser("https://x.com/stylesofgd");
}
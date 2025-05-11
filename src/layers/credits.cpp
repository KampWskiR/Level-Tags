#include "credits.hpp"

bool CreditsPopup::setup(bool a) {
    auto title = CCSprite::create("credits.png"_spr);
    title->setPosition({m_mainLayer->getContentWidth() / 2, m_mainLayer->getContentHeight() - 20});
    title->setScale(0.6);
    m_mainLayer->addChild(title);

    auto descMenu = CCScale9Sprite::create("square02b_001.png");
    descMenu->setContentSize({305, 160});
    descMenu->setPosition({m_mainLayer->getContentWidth() / 2, 105});
    descMenu->setColor({ 0, 0, 0 });
    descMenu->setOpacity(100);
    m_mainLayer->addChild(descMenu);

    for (int i = 0; i < 4; ++i) {
        auto corner = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
        corner->setRotation(90 * i);
        corner->setPosition({ (i < 2 ? 24.6f : 315.5f), (i % 3 == 0 ? 24.6f : 195.5f) });
        m_mainLayer->addChild(corner);
    }

    auto loading = LoadingSpinner::create(20.f);
    loading->setPosition({m_mainLayer->getContentWidth() / 2, 105});
    m_mainLayer->addChild(loading);

    m_listener.bind([this, loading](web::WebTask::Event* e) {
        if (auto res = e->getValue(); res && res->ok()) {
            auto jsonStr = res->string().unwrapOr("{}");
            auto json = matjson::parse(jsonStr).unwrapOr("{}");

            if (loading) m_mainLayer->removeChild(loading);
            
            auto menu = CCMenu::create();
            menu->setPosition({m_mainLayer->getContentWidth() / 2, 180});
            menu->setContentWidth(300);
            menu->setAnchorPoint({0.5, 1});
            menu->setLayout(AxisLayout::create()->setGrowCrossAxis(true)->setAutoScale(false)->setGap(5));
            menu->setID("credits-menu");
            m_mainLayer->addChild(menu);
            
            for (const auto& [key, value] : json) {
                auto btn = CCMenuItemSpriteExtra::create( tabSprite(key, value[0].asString().unwrapOr("")), this, menu_selector(CreditsPopup::btn));
                btn->setID(key);
                btn->setTag(value[1].asInt().unwrapOr(0));
                menu->addChild(btn);
            }
        
            auto docs = CCMenuItemSpriteExtra::create( tabSprite("GD Styles List", "Mod Inspiration"), this, menu_selector(CreditsPopup::docs));
            docs->setID("Docs");
            menu->addChild(docs);
        
            menu->updateLayout();
        }
    });

    auto req = web::WebRequest();
    m_listener.setFilter(req.get(fmt::format("https://raw.githubusercontent.com/KampWskiR/test3/main/credits.json")));

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
    bg->setColor({ 90, 50, 40 });
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

void CreditsPopup::btn(CCObject* sender) {
    ProfilePage::create(sender->getTag(), false)->show();
}

void CreditsPopup::docs(CCObject* sender) {
    web::openLinkInBrowser("https://x.com/stylesofgd");
}

CreditsPopup* CreditsPopup::create(bool a) {
    auto ret = new CreditsPopup();
    if (ret->initAnchored(340.f, 220.f, a, "square.png"_spr)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
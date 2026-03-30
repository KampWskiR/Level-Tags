#include "credits.hpp"

bool CreditsPopup::init() {
    if (!Popup::init(340.f, 220.f, "square.png"_spr)) return false;
    setTitle("Staff");
    m_title->setPositionY(204.f);

    m_mainLayer->addChild(TagsManager::sharedState()->addBgAnim(m_size));
    m_mainLayer->addChild(TagsManager::sharedState()->addCorners(m_size, 1.0f));

    auto descMenu = UIsquare::create(true, {305.f, 160.f});
    descMenu->setPosition({m_mainLayer->getContentWidth() / 2, 105});
    m_mainLayer->addChild(descMenu);

    auto loading = LoadingSpinner::create(20.f);
    loading->setPosition({m_mainLayer->getContentWidth() / 2, 105});
    m_mainLayer->addChild(loading);

    auto req = geode::utils::web::WebRequest();

    m_listener.spawn(
        req.get(fmt::format("{}/credits", Mod::get()->getSettingValue<std::string>("serverUrl"))),
        [this, loading](geode::utils::web::WebResponse value) {
            auto jsonStr = value.string().unwrapOr("{}");
            auto json = matjson::parse(jsonStr).unwrapOr("{}");

            if (loading) m_mainLayer->removeChild(loading);

            for (int i = 0; i < 3; i++) {
                CCSprite* dot = CCSprite::createWithSpriteFrameName("gj_navDotBtn_off_001.png");
                dot->setScale(0.75f);
                dot->setPosition({m_mainLayer->getContentWidth() / 2 + (i - 1) * 20, 15.f});
                m_mainLayer->addChild(dot);
            }

            pageDot = CCSprite::createWithSpriteFrameName("gj_navDotBtn_on_001.png");
            pageDot->setScale(0.75f);
            pageDot->setPosition({150.f, 15.f});
            m_mainLayer->addChild(pageDot);

            auto staffMenu = CCMenu::create();
            staffMenu->setPosition({m_mainLayer->getContentWidth() / 2, 180});
            staffMenu->setContentWidth(300);
            staffMenu->setAnchorPoint({0.5, 1});
            staffMenu->setLayout(AxisLayout::create()->setGrowCrossAxis(true)->setAutoScale(false)->setGap(5));
            staffMenu->setID("staff-menu");
            m_mainLayer->addChild(staffMenu);

            auto helpersMenu = CCMenu::create();
            helpersMenu->setPosition({m_mainLayer->getContentWidth() / 2, 180});
            helpersMenu->setContentWidth(300);
            helpersMenu->setAnchorPoint({0.5, 1});
            helpersMenu->setVisible(false);
            helpersMenu->setLayout(AxisLayout::create()->setGrowCrossAxis(true)->setAutoScale(false)->setGap(5));
            helpersMenu->setID("helpers-menu");
            m_mainLayer->addChild(helpersMenu);

            auto stMenu = CCMenu::create();
            stMenu->setPosition({m_mainLayer->getContentWidth() / 2, 180});
            stMenu->setContentWidth(300);
            stMenu->setAnchorPoint({0.5, 1});
            stMenu->setVisible(false);
            stMenu->setLayout(AxisLayout::create()->setGrowCrossAxis(true)->setAutoScale(false)->setGap(5));
            stMenu->setID("special-thanks-menu");
            m_mainLayer->addChild(stMenu);

            auto arrowSprRight = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
            arrowSprRight->setFlipX(true);

            auto pageArrowRight = CCMenuItemSpriteExtra::create(arrowSprRight, this, menu_selector(CreditsPopup::switchPage));
            pageArrowRight->setPosition(360.f, m_mainLayer->getContentHeight() / 2);
            pageArrowRight->setTag(1);
            m_closeBtn->getParent()->addChild(pageArrowRight);

            auto pageArrowLeft = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), this, menu_selector(CreditsPopup::switchPage));
            pageArrowLeft->setPosition(-20.f, m_mainLayer->getContentHeight() / 2);
            pageArrowLeft->setTag(-1);
            m_closeBtn->getParent()->addChild(pageArrowLeft);
            
            for (const auto& [key, value] : json) {
                auto role = value[0].asString().unwrapOr("");
                auto btn = CCMenuItemSpriteExtra::create( tabSprite(key, role), this, menu_selector(CreditsPopup::btn));
                btn->setID(key);
                btn->setTag(value[1].asInt().unwrapOr(0));

                if (role == "developer" || role == "admin" || role == "moderator") staffMenu->addChild(btn);
                else if (role == "helper") helpersMenu->addChild(btn);
                else stMenu->addChild(btn);
            }
        
            auto docs = CCMenuItemSpriteExtra::create( tabSprite("GD Styles List", "Mod Inspiration"), this, menu_selector(CreditsPopup::docs));
            docs->setID("Docs");
            stMenu->addChild(docs);
        
            staffMenu->updateLayout();
            helpersMenu->updateLayout();
            stMenu->updateLayout();
        }
    );

    return true;
}

CCNode* CreditsPopup::tabSprite(std::string name, std::string role) {
    auto bg = UIsquare::create(false, {70.f, 25.f});
    bg->setPosition({bg->getContentWidth() / 2, bg->getContentHeight() / 2});

    auto label = CCLabelBMFont::create(name.c_str(), "goldFont.fnt");
    label->setPosition({bg->getContentWidth() / 2, 18});
    label->limitLabelWidth(60, 0.6, 0.2);
    bg->addChild(label);

    ccColor3B color = { 255, 255, 255 };
    if (role == "developer") color = { 255, 150, 230 };
    else if (role == "admin") color = { 255, 50, 50 };
    else if (role == "moderator") color = { 200, 100, 0 };
    else if (role == "helper") color = { 0, 200, 255 };
    else if (role == "supporter") color = { 200, 0, 255 };

    auto roleLabel = CCLabelBMFont::create(role.c_str(), "bigFont.fnt");
    roleLabel->setPosition({bg->getContentWidth() / 2, 8});
    roleLabel->setColor(color);
    roleLabel->limitLabelWidth(60, 0.25, 0.2);
    bg->addChild(roleLabel);

    return bg;
}

void CreditsPopup::switchPage(CCObject* sender) {
    if (auto btn = static_cast<CCMenuItemSpriteExtra*>(sender); btn->getTag() == 1) {
        page++;
        if (page > 2) page = 0;
    } else {
        page--;
        if (page < 0) page = 2;
    }
    m_mainLayer->getChildByID("staff-menu")->setVisible(false);
    m_mainLayer->getChildByID("helpers-menu")->setVisible(false);
    m_mainLayer->getChildByID("special-thanks-menu")->setVisible(false);
    switch (page) {
        case 0:
            setTitle("Staff");
            m_mainLayer->getChildByID("staff-menu")->setVisible(true);
            pageDot->setPosition({150.f, 15.f});
            break;
        case 1:
            setTitle("Helpers");
            m_mainLayer->getChildByID("helpers-menu")->setVisible(true);
            pageDot->setPosition({170.f, 15.f});
            break;
        case 2:
            setTitle("Special Thanks");
            m_mainLayer->getChildByID("special-thanks-menu")->setVisible(true);
            pageDot->setPosition({190.f, 15.f});
            break;
    }
}

void CreditsPopup::btn(CCObject* sender) {
    ProfilePage::create(sender->getTag(), false)->show();
}

void CreditsPopup::docs(CCObject* sender) {
    web::openLinkInBrowser("https://x.com/stylesofgd");
}

CreditsPopup* CreditsPopup::create() {
    auto popup = new CreditsPopup;
    if (popup->init()) {
        popup->autorelease();
        return popup;
    }
    delete popup;
    return nullptr;
}
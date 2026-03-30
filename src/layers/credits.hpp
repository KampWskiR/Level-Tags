#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

#include "../tagsManager.hpp"
#include "uiSquare.hpp"

using namespace geode::prelude;

class CreditsPopup : public Popup {
protected:
    async::TaskHolder<geode::utils::web::WebResponse> m_listener;
    CCSprite* arrowSpr;
    CCSprite* pageDot;
    int page = 0;

    bool init();
    CCNode* tabSprite(std::string name, std::string role);
    void switchPage(CCObject* sender);
    void btn(CCObject* sender);
    void docs(CCObject* sender);
public:
    static CreditsPopup* create();
};
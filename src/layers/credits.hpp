#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

#include "../tagsManager.hpp"
#include "credits.hpp"

using namespace geode::prelude;

class CreditsPopup : public geode::Popup<bool> {
protected:
    bool setup(bool a);
    CCNode* tabSprite(std::string name, std::string role);
    void btn(CCObject* sender);
    void docs(CCObject* sender);
public:
    static CreditsPopup* create(bool a);
};
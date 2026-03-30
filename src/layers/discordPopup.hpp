#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

#include "../tagsManager.hpp"
#include "uiSquare.hpp"

using namespace geode::prelude;

class DiscordPopup : public Popup {
protected:
    bool init();
    void join(CCObject*);
public:
    static DiscordPopup* create();
};
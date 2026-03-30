#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

#include "../tagsManager.hpp"
#include "uiSquare.hpp"

using namespace geode::prelude;

class KofiPopup : public Popup {
protected:
    bool init();
    void open(CCObject*);
public:
    static KofiPopup* create();
};
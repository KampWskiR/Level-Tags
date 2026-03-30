#pragma once

#include <Geode/Geode.hpp>

#include "../tagsManager.hpp"
#include "tagDesc.hpp"
#include "uiSquare.hpp"

using namespace geode::prelude;

class MoreTags : public Popup {
protected:
    bool init(matjson::Value tags);
public:
    static MoreTags* create(matjson::Value tags);
};
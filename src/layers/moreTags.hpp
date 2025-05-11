#pragma once

#include <Geode/Geode.hpp>

#include "../tagsManager.hpp"
#include "tagDesc.hpp"

using namespace geode::prelude;

class MoreTags : public geode::Popup<matjson::Value> {
protected:
    bool setup(matjson::Value tags);
public:
    static MoreTags* create(matjson::Value tags);
};
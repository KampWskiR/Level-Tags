#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include "levelCell.hpp"

using namespace geode::prelude;

class $modify(TagsLevelBrowserLayer, LevelBrowserLayer) {
    struct Fields {
        matjson::Value tags;
        async::TaskHolder<geode::utils::web::WebResponse> m_listener;
        int m_listeners = 0;
    };
    void loadLevelsFinished(CCArray* p0, char const* p1, int p2) override;
};
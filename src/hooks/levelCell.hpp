#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/modify/LevelCell.hpp>

#include "../tagsManager.hpp"
#include "../layers/tagDesc.hpp"
#include "../layers/moreTags.hpp"

using namespace geode::prelude;

class $modify(TagsLevelCell, LevelCell) {
    struct Fields {
        async::TaskHolder<geode::utils::web::WebResponse> m_listener;
        matjson::Value tags;
        bool extended = false;
        float creatorNameY;
    };
    
    void adjustY(const std::vector<std::string>& elements, float y);
    void adjustPositions();
    void loadCustomLevelCell();
    CCMenu* createTagContainer(bool extended);
    void updateTags(bool extended);
    void setExpand(CCObject* sender);
};
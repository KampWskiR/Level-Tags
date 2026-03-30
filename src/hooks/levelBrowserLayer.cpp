#include "levelBrowserLayer.hpp"

using namespace geode::prelude;

void TagsLevelBrowserLayer::loadLevelsFinished(CCArray* p0, char const* p1, int p2) {
    LevelBrowserLayer::loadLevelsFinished(p0, p1, p2);
    if (m_searchObject->m_searchType == SearchType::MyLevels || !p0 || p0->count() == 0) return;

    std::vector<int> levelIds;
    
    for (auto level : CCArrayExt<GJGameLevel*>(p0)) {
        if (!geode::cast::typeinfo_cast<GJGameLevel*>(level)) return;
        levelIds.push_back(level->m_levelID.value());
    }

    if (std::all_of(levelIds.begin(), levelIds.end(), [](int id) {
        return TagsManager::sharedState()->cachedTags[std::to_string(id)].size() != 0;
    })) {
        return;
    }

    std::string ids;
    for (size_t i = 0; i < levelIds.size(); ++i) {
        ids += std::to_string(levelIds[i]);
        if (i != levelIds.size() - 1) ids += ",";
    }

    int curList = ++m_fields->m_listeners;
    auto req = geode::utils::web::WebRequest();

    m_fields->m_listener.spawn(
        req.get(fmt::format("{}/get?id={}", Mod::get()->getSettingValue<std::string>("serverUrl"), ids)),
        [this, curList](geode::utils::web::WebResponse value) {
            auto jsonStr = value.string().unwrapOr("{}");
            auto json = matjson::parse(jsonStr).unwrapOr("{}");
                
            for (const auto& [id, tags] : json) {
                if (tags.size() == 0) continue;
                TagsManager::sharedState()->cachedTags[id] = tags;
            }
            if (!m_list || !m_list->m_listView || !m_list->m_listView->m_tableView) return;

            auto tableChildren = m_list->m_listView->m_tableView->getChildren();
            if (!tableChildren || tableChildren->count() == 0) return;

            auto contentLayer = static_cast<CCContentLayer*>(tableChildren->objectAtIndex(0));
            auto children = contentLayer->getChildren();
            for (int i = 0; i < children->count(); ++i) {
                if (auto levelCell = static_cast<TagsLevelCell*>(children->objectAtIndex(i))) {
                    if (TagsManager::sharedState()->cachedTags[std::to_string(levelCell->m_level->m_levelID.value())].size() == 0) continue;
                    levelCell->m_fields->tags = TagsManager::sortTags(TagsManager::sharedState()->cachedTags[std::to_string(levelCell->m_level->m_levelID.value())]);
                    if (levelCell->m_fields->tags.size() == 0) continue;
                    if (!levelCell) return;
                    levelCell->adjustPositions();
                    levelCell->updateTags(false);
                }
            }
        }
    );
}
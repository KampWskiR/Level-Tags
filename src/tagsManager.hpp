#pragma once

#include <Geode/Geode.hpp>

#include "layers/tagDesc.hpp"

using namespace geode::prelude;

class TagsManager {
private:
    inline static TagsManager* instance = nullptr;
public:
    inline static matjson::Value cachedTags;
    inline static matjson::Value cachedDescs;
    inline static matjson::Value tags;
    
    static TagsManager* sharedState();
    static matjson::Value sortTags(matjson::Value tags);
    static matjson::Value getTags(int id);
    static CCSprite* getTagSprite(std::string tag);
    static matjson::Value getTagObject(std::string tag);
    static IconButtonSprite* addTag(matjson::Value tag, float scale);
};
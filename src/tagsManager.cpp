#include "tagsManager.hpp"

TagsManager* TagsManager::sharedState() {
    if (instance == nullptr) instance = new TagsManager;
    return instance;
}

matjson::Value TagsManager::sortTags(matjson::Value tags) {
    matjson::Value result = matjson::Value::array();
    std::vector<std::string> order;

    auto tagsOrder = Mod::get()->getSettingValue<std::string>("tags-order");
    if (tagsOrder == "style, theme, meta, gameplay") order = {"style", "theme", "meta", "gameplay"};
    else if (tagsOrder == "meta, gameplay, style, theme") order = {"meta", "gameplay", "style", "theme"};
    else if (tagsOrder == "gameplay, meta, style, theme") order = {"gameplay", "meta", "style", "theme"};

    for (auto key : order) {
        if (tags.contains(key)) {
            for (auto i = 0; i < tags[key].size(); i++) {
                result.push(tags[key][i].as<std::string>().unwrap());
            }
        }
    }
    return result;
}

ccColor3B TagsManager::color(const std::string& tag) {
    std::hash<std::string> hasher;
    size_t hashValue = hasher(tag);

    GLubyte r = (hashValue >> 16) & 0xFF;
    GLubyte g = (hashValue >> 8) & 0xFF;
    GLubyte b = hashValue & 0xFF;

    if (r < 100) r = 100;
    if (g < 100) g = 100;
    if (b < 100) b = 100;

    return { r, g, b };
}

CCSprite* TagsManager::getTagSprite(std::string tag) {
    auto tagIcon = CCSprite::createWithSpriteFrameName(fmt::format("{}"_spr, tag + ".png").c_str());
    if (!tagIcon || tagIcon->getUserObject("geode.texture-loader/fallback")) tagIcon = CCSprite::create("404.png"_spr);
    tagIcon->setColor(TagsManager::color(tag));
    tagIcon->setPosition({50, 58});
    tagIcon->setScale(0.5);

    return tagIcon;
}

matjson::Value TagsManager::getTagObject(std::string tag) {
    for (auto& [category, tagObj] : TagsManager::sharedState()->tags) {
        if (tagObj.contains(tag)) {
            for (auto& [key, value] : tagObj) {
                if (key == tag) {
                    matjson::Value obj = matjson::Value();
                    obj[tag] = tagObj[tag];
                    return obj;
                }
            }
        }
    }
    return matjson::Value();
};

IconButtonSprite* TagsManager::addTag(matjson::Value tag, float scale) {
    std::string tagName;
    matjson::Value tagInfo = matjson::Value::array();
    for (auto& [key, value] : tag) {
        tagName = key;
        tagInfo = value;
    }
    ccColor3B tagColor = tagInfo[1].as<ccColor3B>().unwrapOr(ccColor3B(0,0,0));
    auto tagNode = IconButtonSprite::create(
        "tagSquare.png"_spr, 
        getTagSprite(tagName.c_str()), 
        tagName.c_str(), "bigFont.fnt"
    );
    tagNode->setScale(scale);
    tagNode->setColor(tagColor); 
    tagNode->setOpacity(255);
    tagNode->getIcon()->setScale(0.5);

    // if (tagName == "circles") {
    //     CCDelayTime* delay = CCDelayTime::create(0.3);
    //     CCActionInterval* sequence = CCSequence::create(
    //         CCTintTo::create(0, 255, 50, 50), delay, CCTintTo::create(0, 100, 25, 25), delay,
    //         CCTintTo::create(0, 255, 125, 50), delay, CCTintTo::create(0, 100, 50, 25), delay, nullptr
    //     ); tagNode->runAction(CCRepeatForever::create(sequence));
    // }

    return tagNode;
}
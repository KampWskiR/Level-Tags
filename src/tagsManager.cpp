#include "tagsManager.hpp"

TagsManager* TagsManager::sharedState() {
    if (instance == nullptr) instance = new TagsManager;
    return instance;
}

matjson::Value TagsManager::sortTags(matjson::Value tags) {
    matjson::Value result = matjson::Value::array();
    std::vector<std::string> order;

    if (Mod::get()->getSettingValue<bool>("hideStyle")) tags["style"] = matjson::Value::array();
    if (Mod::get()->getSettingValue<bool>("hideTheme")) tags["theme"] = matjson::Value::array();
    if (Mod::get()->getSettingValue<bool>("hideMeta")) tags["meta"] = matjson::Value::array();
    if (Mod::get()->getSettingValue<bool>("hideGameplay")) tags["gameplay"] = matjson::Value::array();

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

CCSprite* TagsManager::getTagSprite(std::string tag) {
    auto tagIcon = CCSprite::createWithSpriteFrameName(fmt::format("{}"_spr, tag + ".png").c_str());
    if (!tagIcon || tagIcon->getUserObject("geode.texture-loader/fallback")) tagIcon = CCSprite::create("404.png"_spr);
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
    ccColor3B tagColor = tagInfo[1].as<ccColor3B>().unwrapOr(ccColor3B{0, 0, 0});
    auto tagNode = IconButtonSprite::create(
        "tagSquare.png"_spr, 
        getTagSprite(tagName.c_str()), 
        tagName.c_str(), "bigFont.fnt"
    );
    tagNode->setScale(scale);
    tagNode->setColor(tagColor); 
    tagNode->setOpacity(255);
    tagNode->getIcon()->setScale(0.5);
    tagNode->getIcon()->setPositionX(1.f);

    return tagNode;
}

CCClippingNode *TagsManager::addBgAnim(CCSize size) {
    auto stencil = CCLayerColor::create(ccc4(0, 0, 0, 255), size.width, size.height);
    stencil->setAnchorPoint({0, 0});
    stencil->setPosition({0, 0});

    auto clipper = CCClippingNode::create(stencil);
    clipper->setAnchorPoint({0.5f, 0.5f});
    clipper->setPosition({0, 0});

    auto texture = CCTextureCache::sharedTextureCache()->addImage("tagsBG.png"_spr, false);
    ccTexParams texParams = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
    texture->setTexParameters(&texParams);

    auto bgTexture = CCSprite::createWithTexture(texture, CCRectMake(0, 0, texture->m_uPixelsWide * 2, texture->m_uPixelsHigh * 2));
    bgTexture->setAnchorPoint(ccp(0.5f, 0.5f));
    bgTexture->setPosition({0.f, 0.f});
    bgTexture->setOpacity(25);

    clipper->addChild(bgTexture);
    
    if (Mod::get()->getSettingValue<bool>("bganim")) {
        auto bgMove = CCMoveBy::create(10.0f, {272.f, -277.f});
        auto bgReset = CCMoveTo::create(0.0f, {0.f, 0.f});
        auto bgSeq = CCSequence::create(bgMove, bgReset, nullptr);
        auto bgRepeat = CCRepeatForever::create(bgSeq);
        bgTexture->runAction(bgRepeat);
    };

    return clipper;
}

CCLayer* TagsManager::addCorners(CCSize size, float scale) {
    auto layer = CCLayer::create();
    layer->setContentSize(size);
    layer->setAnchorPoint({0.5f, 0.5f});
    layer->setPosition({0, 0});

    for (int i = 0; i < 4; ++i) {
        auto corner = CCSprite::create("corner.png"_spr);
        corner->setRotation(90 * i);
        corner->setScale(scale);
        corner->setAnchorPoint({0.f, 0.f});
        corner->setPosition({ (i < 2 ? 0.f : size.width), (i % 3 == 0 ? 0.f : size.height) });
        layer->addChild(corner);
    }
    return layer;
}

void TagsManager::loadTagsInfo() {
    auto req = geode::utils::web::WebRequest();
    m_listener.spawn(
        req.get(fmt::format("{}/tags", Mod::get()->getSettingValue<std::string>("serverUrl"))),
            [](geode::utils::web::WebResponse value) {
            auto jsonStr = value.string().unwrapOr("{}");
            TagsManager::sharedState()->tags = matjson::parse(jsonStr).unwrapOr("{}");
        }
    );
}

std::string TagsManager::urlEncode(std::string str) {
    std::ostringstream encodedStream;
    encodedStream << std::hex << std::uppercase << std::setfill('0');

    for (char c : str) {
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encodedStream << c;
        } else {
            encodedStream << '%' << std::setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(c));
        }
    }

    return encodedStream.str();
}
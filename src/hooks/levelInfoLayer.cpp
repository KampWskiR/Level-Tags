#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

#include "../tagsManager.hpp"
#include "../layers/tagDesc.hpp"
#include "../layers/moreTags.hpp"
#include "../layers/requestTag.hpp"

using namespace geode::prelude;

class $modify(TagsLevelInfoLayer, LevelInfoLayer) {
    struct Fields {
        EventListener<web::WebTask> m_listener;
        matjson::Value tags;
    };

    void request(CCObject* sender) {RequestTag::create({std::to_string(m_level->m_levelID.value()), m_level->isPlatformer()})->show();}

    void moreTags(CCObject* sender) {MoreTags::create(m_fields->tags)->show();}

    $override
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;

        if (!Mod::get()->getSettingValue<bool>("hideRequest")) {
            auto menu = this->getChildByID("left-side-menu");
            auto request = CCMenuItemSpriteExtra::create(CircleButtonSprite::createWithSprite("icon.png"_spr, 1.2, CircleBaseColor::DarkPurple), this, menu_selector(TagsLevelInfoLayer::request));
            request->setID("tag-request");
            menu->addChild(request);
            menu->updateLayout();
        }

        loadCustomLevelInfoLayer();
        return true;
    };

    void loadCustomLevelInfoLayer() {
        if (!Mod::get()->getSettingValue<bool>("levelInfoShow")) return;

        if (TagsManager::sharedState()->tags.size() == 0) {
            Notification::create("Failed to retrieve tags from the server.", NotificationIcon::Error, 2)->show();
            return;
        }

        if (TagsManager::sharedState()->cachedTags[std::to_string(m_level->m_levelID)].size() != 0) {
            m_fields->tags = TagsManager::sortTags(TagsManager::sharedState()->cachedTags[std::to_string(m_level->m_levelID)]);
            updateTags();
            return;
        }

        m_fields->m_listener.bind([this](web::WebTask::Event* e) {
            if (auto res = e->getValue(); res && res->ok()) {
                auto jsonStr = res->string().unwrapOr("{}");
                auto json = matjson::parse(jsonStr).unwrapOr("{}");

                m_fields->tags = TagsManager::sortTags(json);
                TagsManager::sharedState()->cachedTags[std::to_string(m_level->m_levelID)] = json;
                updateTags();
            }
        });

        auto req = web::WebRequest();
        m_fields->m_listener.setFilter(req.get(fmt::format("https://raw.githubusercontent.com/KampWskiR/test3/main/tags/{}.json", m_level->m_levelID.value())));
    };

    void updateTags() {
        if (auto titleLabel = this->getChildByID("title-label")) titleLabel->setPositionY(titleLabel->getPositionY() - 4);
        if (auto titleLabel = this->getChildByID("title-label")) titleLabel->setScale(titleLabel->getScale() - 0.1);
        if (auto dailyLabel = this->getChildByID("daily-label")) dailyLabel->setPositionY(dailyLabel->getPositionY() - 2);

        CCSize winSize = CCDirector::get()->getWinSize();

        auto tagMenu = CCMenu::create();
        tagMenu->setLayout(RowLayout::create()->setAutoScale(false)->setGap(2)->setAxisAlignment(AxisAlignment::Center));
        tagMenu->setPosition({winSize.width / 2, winSize.height - 8});
        tagMenu->setID("level-tags");
        this->addChild(tagMenu);

        if (!m_fields->tags.isNull() && m_fields->tags.isArray()) {
            auto tagCount = m_fields->tags.size();
            for (const auto& tag : m_fields->tags) {
                if (TagsManager::sharedState()->getTagObject(tag.asString().unwrapOr("")) == matjson::Value()) {
                    tagCount--;
                    continue;
                }
                auto tagNode = CCMenuItemSpriteExtra::create(
                    TagsManager::addTag(TagsManager::sharedState()->getTagObject(tag.asString().unwrapOr("")),
                    0.35), this, menu_selector(TagDesc::open)
                );
                tagNode->setID(tag.asString().unwrapOr(""));
                tagMenu->addChild(tagNode);
                tagMenu->updateLayout();
                if (tagNode->getPositionX() > 400 && tagCount != tagMenu->getChildrenCount()) {
                    auto expandSpr = IconButtonSprite::create("tagSquare.png"_spr, CCSprite::createWithSpriteFrameName("PBtn_Arrow_001.png"), "more", "bigFont.fnt");
                    expandSpr->setScale(0.35);

                    auto tagExpand = CCMenuItemSpriteExtra::create(expandSpr, this, menu_selector(TagsLevelInfoLayer::moreTags));
                    tagExpand->setAnchorPoint({0.5, 0.5});
                    tagExpand->setColor({255,255,255});
                    tagExpand->setOpacity(255);
                    tagMenu->addChild(tagExpand);
                    tagMenu->updateLayout();
                    break;
                }
            }
        }
    }
};
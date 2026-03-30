#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

#include "../tagsManager.hpp"
#include "../layers/tagDesc.hpp"
#include "../layers/moreTags.hpp"
#include "../layers/discordPopup.hpp"

using namespace geode::prelude;

class $modify(TagsLevelInfoLayer, LevelInfoLayer) {
    struct Fields {
        async::TaskHolder<geode::utils::web::WebResponse> m_listener;
        matjson::Value tags;
    };

    void request(CCObject* sender) {DiscordPopup::create()->show();}

    void moreTags(CCObject* sender) {MoreTags::create(m_fields->tags)->show();}

    $override
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;

        if (!Mod::get()->getSettingValue<bool>("hideRequest")) {
            auto menu = this->getChildByID("left-side-menu");

            auto spr = CircleButtonSprite::createWithSprite("discord.png"_spr, 1.15f, CircleBaseColor::DarkPurple);
            static_cast<CCSprite*>(spr->getChildren()->firstObject())->setPosition({25.f, 26.f});

            auto request = CCMenuItemSpriteExtra::create(spr, this, menu_selector(TagsLevelInfoLayer::request));
            request->setID("tags-discord");
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
            if (m_fields->tags.size() == 0) return;
            updateTags();
            return;
        }

        auto req = geode::utils::web::WebRequest();

        m_fields->m_listener.spawn(
            req.get(fmt::format("{}/get?id={}", Mod::get()->getSettingValue<std::string>("serverUrl"), m_level->m_levelID.value())),
            [this](geode::utils::web::WebResponse value) {
                auto jsonStr = value.string().unwrapOr("{}");
                auto json = matjson::parse(jsonStr).unwrapOr("{}");

                m_fields->tags = TagsManager::sortTags(json[std::to_string(m_level->m_levelID)]);
                TagsManager::sharedState()->cachedTags[std::to_string(m_level->m_levelID)] = json[std::to_string(m_level->m_levelID)];
                if (m_fields->tags.size() == 0) return;
                updateTags();
            }
        );
    };

    void updateTags() {
        if (auto titleLabel = this->getChildByID("title-label")) titleLabel->setPositionY(titleLabel->getPositionY() - 4);
        if (auto titleLabel = this->getChildByID("title-label")) titleLabel->setScale(titleLabel->getScale() - 0.1);
        if (auto dailyLabel = this->getChildByID("daily-label")) dailyLabel->setPositionY(dailyLabel->getPositionY() - 4);

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
                    TagsManager::addTag(TagsManager::sharedState()->getTagObject(tag.asString().unwrapOr("")), 0.35), this, menu_selector(TagDesc::open)
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
#pragma once

#include <Geode/utils/web.hpp>
#include <Geode/ui/LoadingSpinner.hpp>
#include <random>

#include "../tagsManager.hpp"
#include "../layers/credits.hpp"

using namespace geode::prelude;

class BetaSearch : public geode::Popup<std::string> {
private:
    EventListener<web::WebTask> m_listener;
    bool includeSelected = true;
    CCMenu* menuPage1;
    CCMenu* menuPage2;
    CCMenu* menuStyle;
    CCMenu* menuTheme;
    CCMenu* menuMeta;
    CCMenu* menuGameplay;
    CCSprite* select;
    CCMenuItemSpriteExtra* includeBG;
    CCMenuItemSpriteExtra* excludeBG;
    bool uncompletedSelected = false;
    bool savedSelected = false;
    std::vector<std::string> include;
    std::vector<std::string> exclude;
    CCLabelBMFont* sortText;
    std::string jsonStr;
    int sort = 0;
    int tagZ = 0;
    bool tagInfoSelected = false;

    void makeVisible(CCObject* sender);
    void info(CCObject*);
    void credits(CCObject*);
    CCMenu* createMenu(std::string name);
    void uncompleted(CCObject*);
    void sortPageUp(CCObject*);
    void sortPageDown(CCObject*);
    void tagMenuChange(CCObject* sender);
    void menuChange(CCObject* sender);
    void search(CCObject* sender);
    void category(CCObject* sender);
    void btn(CCObject* sender);
    void saved(CCObject* sender);
    void addTagsList(int tag, std::vector<std::string> tags);

protected:
    bool setup(std::string tag);

public:
    static BetaSearch* create(std::string tag);
};
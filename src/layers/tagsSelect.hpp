#pragma once

#include <Geode/Geode.hpp>

#include "tagsSearch.hpp"
#include "uiSquare.hpp"

class TagsSearch;

using namespace geode::prelude;

class TagsSelect : public Popup, public TextInputDelegate {
protected:
    TagsSearch* m_search;
    std::vector<std::string> tagMenu;
    TextInput* searchInput;
    CCMenu* menuStyle;
    CCMenu* menuTheme;
    CCMenu* menuMeta;
    CCMenu* menuGameplay;
    bool tagInfoSelected = false;
    int tagZ = 0;
    bool init(TagsSearch* search);
    CCMenu* createMenu(std::string name);
    void category(CCObject* sender);
    void onTag(CCObject* sender);
    void info(CCObject* sender);
    virtual void textChanged(CCTextInputNode* input) override;
    virtual void onClose(cocos2d::CCObject*) override;
public:
    static TagsSelect* create(TagsSearch* search);
};
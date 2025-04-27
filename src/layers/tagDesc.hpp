#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/ui/LoadingSpinner.hpp>

#include "../tagsManager.hpp"

using namespace geode::prelude;

class TagDesc : public geode::Popup<std::string> {
private:
    EventListener<web::WebTask> m_listener;
protected:
    bool setup(std::string tag);
public:
    static TagDesc* create(std::string tag);
    void open(CCObject* sender);
};
#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/LoadingSpinner.hpp>

#include "../tagsManager.hpp"

using namespace geode::prelude;

class TagDesc : public geode::Popup<std::string> {
protected:
    bool setup(std::string tag);
public:
    static TagDesc* create(std::string tag);
    void open(CCObject* sender);
};
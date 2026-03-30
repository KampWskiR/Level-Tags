#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/LoadingSpinner.hpp>

#include "../tagsManager.hpp"
#include "uiSquare.hpp"

using namespace geode::prelude;

class TagDesc : public Popup {
protected:
    bool init(std::string tag);
public:
    static TagDesc* create(std::string tag);
    void open(CCObject* sender);
};
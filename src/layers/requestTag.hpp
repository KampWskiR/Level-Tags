#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class RequestTag : public geode::Popup<std::vector<std::variant<std::string, bool>>> {
protected:
    bool setup(std::vector<std::variant<std::string, bool>> level) override;
    void discord(CCObject* sender);

public:
    static RequestTag* create(std::vector<std::variant<std::string, bool>> level);
};
#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class UIsquare : public CCLayer {
protected:
	bool init(bool primary, CCSize size);
public:
    geode::NineSlice* m_bg;
    CCLayerGradient* m_gradient;
	static UIsquare* create(bool primary, CCSize size);
};
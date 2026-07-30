#include "uiSquare.hpp"

bool UIsquare::init(bool primary, CCSize size) {
	if (!CCLayer::init()) return false;
    setContentSize(size);
    ignoreAnchorPointForPosition(false);

    m_gradient = CCLayerGradient::create(ccc4(62, 15, 29, 255), ccc4(25, 12, 12, 255));
    if (primary) m_gradient = CCLayerGradient::create(ccc4(42, 36, 7, 255), ccc4(25, 12, 12, 255));
    m_gradient->setContentSize({size.width - 3.f, size.height - 3.f});
    m_gradient->setPosition({size.width / 2, size.height / 2});
    m_gradient->ignoreAnchorPointForPosition(false);
    m_gradient->setVector({0, 1});
    addChild(m_gradient);

    m_bg = geode::NineSlice::create("square07white.png"_spr);
    m_bg->setColor(primary ? ccc3(255, 222, 89) : ccc3(255, 63, 121));
    m_bg->setPosition(size.width / 2, size.height / 2);
    m_bg->setContentSize(size);
    addChild(m_bg);

	return true;
}

UIsquare* UIsquare::create(bool primary, CCSize size) {
	auto ret = new UIsquare();
	if (ret->init(primary, size)) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}
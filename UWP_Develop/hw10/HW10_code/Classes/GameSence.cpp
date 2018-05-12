#include "GameSence.h"
#include <stdlib.h>

USING_NS_CC;

Scene* GameSence::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameSence::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameSence::init()
{

	if (!Layer::init())
	{
		return false;
	}

	//add touch listener
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	//listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameSence::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("level-background-0.jpg");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bg, 0);

	auto gold = Sprite::createWithSpriteFrameName("gold-0-0.png");
	Animate* goldAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("goldAnimation"));
	gold->runAction(RepeatForever::create(goldAnimate));
	gold->setPosition(Vec2(100, 480));
	this->addChild(gold, 1);

	stone = Sprite::create("stone.png");
	stone->setPosition(Vec2(560, 480));
	stoneLayer = Layer::create();
	stoneLayer->ignoreAnchorPointForPosition(true);
	stoneLayer->addChild(stone, 0);

	mouse = Sprite::createWithSpriteFrameName("gem-mouse-0.png");
	Animate* mouseAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("mouseAnimation"));
	mouse->runAction(RepeatForever::create(mouseAnimate));
	mouse->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	mouseLayer = Layer::create();
	mouseLayer->ignoreAnchorPointForPosition(true);
    //mouseLayer->setPosition(0, visibleSize.height / 2);
	mouseLayer->addChild(mouse, 0);

	this->addChild(stoneLayer);
	this->addChild(mouseLayer);

	auto label0 = Label::createWithTTF("Shoot", "fonts/Marker Felt.ttf", 50);
	label0->setPosition(Vec2(700, 480));
	this->addChild(label0, 1);

	return true;
}

bool GameSence::onTouchBegan(Touch *touch, Event *unused_event) {

	auto location = touch->getLocation();
	if (650 <= location.x && location.x <= 750 && 460 <= location.y && location.y <= 500) {
		auto oldposition = mouse->getPosition();

		auto shootstone = Sprite::create("stone.png");
		shootstone->setPosition(stone->getPosition());
		auto fadeout = FadeOut::create(1.0f);
		auto moveto = MoveTo::create(0.5, oldposition);
		auto seq = Sequence::create(moveto, fadeout, nullptr);
		shootstone->runAction(seq);
		this->addChild(shootstone, 1);

		int leave_x = random(0, 900);
		int leave_y = random(0, 600);
		auto leave = MoveTo::create(0.5, Vec2(leave_x, leave_y));
		mouse->runAction(leave);

		auto diamond = Sprite::create("diamond.png");
		diamond->setPosition(oldposition);
		this->addChild(diamond, 1);
	}
	else {
		auto cheese = Sprite::create("cheese.png");
		cheese->setPosition(location);
		auto fadeout = FadeOut::create(1.0f);
		auto moveto = MoveTo::create(0.5, location);
		auto seq = Sequence::create(moveto, fadeout, nullptr);
		cheese->runAction(seq);
		this->addChild(cheese, 1);

		auto movet = MoveTo::create(0.5, location);
		mouse->runAction(movet);
	}
	return true;
}


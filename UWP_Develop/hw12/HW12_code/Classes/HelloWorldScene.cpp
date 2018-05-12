#include "HelloWorldScene.h"
#include "Monster.h"
#define database UserDefault::getInstance()
#pragma execution_character_set("utf-8")
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	if (!database->getBoolForKey("isExist")) {
		database->setBoolForKey("isExist", true);
		database->setIntegerForKey("value", 0);
	}
	killnum = database->getIntegerForKey("value");
	dtime = 180;
	blood = 100;

	//add touch listener
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegin, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

	//创建一张贴图
	auto texture = Director::getInstance()->getTextureCache()->addImage("$lucia_2.png");
	//从贴图中以像素单位切割，创建关键帧
	auto frame0 = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 113, 113)));
	//使用第一帧创建精灵
	player = Sprite::createWithSpriteFrame(frame0);
	player->setPosition(Vec2(origin.x + visibleSize.width / 2,
							origin.y + visibleSize.height/2));
	addChild(player, 3);

	//hp条
	Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));

	//使用hp条设置progressBar
	pT = ProgressTimer::create(sp);
	pT->setScaleX(90);
	pT->setAnchorPoint(Vec2(0, 0));
	pT->setType(ProgressTimerType::BAR);
	pT->setBarChangeRate(Point(1, 0));
	pT->setMidpoint(Point(0, 1));
	pT->setPercentage(blood);
	pT->setPosition(Vec2(origin.x+14*pT->getContentSize().width,origin.y + visibleSize.height - 2*pT->getContentSize().height));
	addChild(pT,2);
	sp0->setAnchorPoint(Vec2(0, 0));
	sp0->setPosition(Vec2(origin.x + pT->getContentSize().width, origin.y + visibleSize.height - sp0->getContentSize().height));
	addChild(sp0,1);

	// 静态动画
	idle.reserve(1);
	idle.pushBack(frame0);

	// 攻击动画
	attack.reserve(17);
	for (int i = 0; i < 17; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(113*i,0,113,113)));
		attack.pushBack(frame);
	}

	// 可以仿照攻击动画
	// 死亡动画(帧数：22帧，高：90，宽：79）
	auto texture2 = Director::getInstance()->getTextureCache()->addImage("$lucia_dead.png");
	dead.reserve(22);
	for (int i = 0; i < 22; i++) {
		auto frame2 = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(79 * i, 0, 79, 90)));
		dead.pushBack(frame2);
	}
    // Todo

	// 运动动画(帧数：8帧，高：101，宽：68）
	auto texture3 = Director::getInstance()->getTextureCache()->addImage("$lucia_forward.png");
	run.reserve(8);
	for (int i = 0; i < 8; i++) {
		auto frame3 = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(68 * i, 0, 68, 101)));
		run.pushBack(frame3);
	}
    // Todo

	//auto labelx = Label::createWithTTF("X", "fonts/arial.ttf", 36);
	//labelx->setPosition(Vec2(visibleSize.width - 30, 80));
	//this->addChild(labelx, 1);

	auto labely = Label::createWithTTF("Y", "fonts/arial.ttf", 36);
	labely->setPosition(Vec2(visibleSize.width - 60, 40));
	this->addChild(labely, 1);

	auto labela = Label::createWithTTF("A", "fonts/arial.ttf", 36);
	labela->setPosition(Vec2(30, 40));
	this->addChild(labela, 1);

	auto labels = Label::createWithTTF("S", "fonts/arial.ttf", 36);
	labels->setPosition(Vec2(80, 40));
	this->addChild(labels, 1);

	auto labeld = Label::createWithTTF("D", "fonts/arial.ttf", 36);
	labeld->setPosition(Vec2(130, 40));
	this->addChild(labeld, 1);

	auto labelw = Label::createWithTTF("W", "fonts/arial.ttf", 36);
	labelw->setPosition(Vec2(80, 80));
	this->addChild(labelw, 1);

	schedule(schedule_selector(HelloWorld::addMonster), 8.0f, kRepeatForever, 0);
	schedule(schedule_selector(HelloWorld::hitByMonster), 0.01f, kRepeatForever, 0);
	schedule(schedule_selector(HelloWorld::update), 0.01f, kRepeatForever, 0);

	TMXTiledMap* tmx = TMXTiledMap::create("map.tmx");
	tmx->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	tmx->setAnchorPoint(Vec2(0.5, 0.5));
	tmx->setScale(Director::getInstance()->getContentScaleFactor());
	this->addChild(tmx, 0);
	return true; 
}

bool HelloWorld::onTouchBegin(Touch *touch, Event *unused_event) {
	auto location = touch->getLocation();
	auto position = player->getPosition();
	if (20 <= location.x && location.x <= 40 && 30 <= location.y && location.y <= 60) { //  A
		auto animation = Animation::createWithSpriteFrames(run, 0.1f);
		auto animate = Animate::create(animation);
		if (40 <= position.x) {
			auto moveby = MoveBy::create(0.3, Vec2(-30, 0));
			auto spawn = Spawn::createWithTwoActions(animate, moveby);
			player->runAction(spawn);
		}
		else {
			player->runAction(animate);
		}
		player->setFlipX(true);
	}
	else if (70 <= location.x && location.x <= 90 && 30 <= location.y && location.y <= 60) {  //  S
		auto animation = Animation::createWithSpriteFrames(run, 0.1f);
		auto animate = Animate::create(animation);
		if (50 <= position.y ) {
			auto moveby = MoveBy::create(0.3, Vec2(0, -30));
			auto spawn = Spawn::createWithTwoActions(animate, moveby);
			player->runAction(spawn);
		}
		else {
			player->runAction(animate);
		}
	}
	else if (120 <= location.x && location.x <= 140 && 30 <= location.y && location.y <= 60) {  //  D
		auto animation = Animation::createWithSpriteFrames(run, 0.1f);
		auto animate = Animate::create(animation);
		if (position.x <= visibleSize.width - 40) {
			auto moveby = MoveBy::create(0.3, Vec2(30, 0));
			auto spawn = Spawn::createWithTwoActions(animate, moveby);
			player->runAction(spawn);
		}
		else {
			player->runAction(animate);
		}
		player->setFlipX(false);
	}
	else if (70 <= location.x && location.x <= 90 && 70 <= location.y && location.y <= 100) {  //  W
		auto animation = Animation::createWithSpriteFrames(run, 0.1f);
		auto animate = Animate::create(animation);
		if (position.y <= visibleSize.height - 50) {
			auto moveby = MoveBy::create(0.3, Vec2(0, 30));
			auto spawn = Spawn::createWithTwoActions(animate, moveby);
			player->runAction(spawn);
		}
		else {
			player->runAction(animate);
		}
	}
	else if (visibleSize.width - 40 <= location.x && location.x <= visibleSize.width - 20 && 70 <= location.y && location.y <= 100) {  //  X
		if (player->getNumberOfRunningActions() == 0) {
			auto animation = Animation::createWithSpriteFrames(dead, 0.1f);
			animation->setRestoreOriginalFrame(true);
			auto animate = Animate::create(animation);
			player->runAction(animate);
			if (blood > 0) blood -= 25;
		    pT->setPercentage(blood);
		}
	}
	else if (visibleSize.width - 70 <= location.x && location.x <= visibleSize.width - 50 && 30 <= location.y && location.y <= 60) {  //  Y
		if (player->getNumberOfRunningActions() == 0) {
			auto animation = Animation::createWithSpriteFrames(attack, 0.1f);
			auto animate = Animate::create(animation);
			player->runAction(animate);
			if (attackMonster()) {
				if (blood < 100) blood += 25;
				pT->setPercentage(blood);
			}
		}
	}
	return true;
}

void HelloWorld::update(float dt) {
	this->removeChild(time, 1);
	char s[12];
	sprintf(s, "%d", killnum);
	std::string str = s;
	time = Label::createWithTTF(str, "fonts/arial.ttf", 36);
	time->setPosition(Vec2(visibleSize.width/2, visibleSize.height - 50));
	this->addChild(time, 1);
}

void HelloWorld::addMonster(float time) {
	auto fac = Factory::getInstance();
	auto m = fac->createMonster();
	float x = random(origin.x, visibleSize.width);
	float y = random(origin.y, visibleSize.height);
	m->setPosition(x, y);
	this->addChild(m, 3);
	fac->moveMonster(player->getPosition(), 1.0f);
}

void HelloWorld::hitByMonster(float dt) {
	auto fac = Factory::getInstance();
	Sprite* collision = fac->collider(player->getBoundingBox());
	if (collision != NULL) {
		fac->removeMonster(collision);
		auto animation = Animation::createWithSpriteFrames(dead, 0.1f);
		animation->setRestoreOriginalFrame(true);
		auto animate = Animate::create(animation);
		player->runAction(animate);
		if (blood > 0) blood -= 25;
		pT->setPercentage(blood);
	}
}

bool HelloWorld::attackMonster() {
	Rect playerRect = player->getBoundingBox();
	Rect attackRect = Rect(playerRect.getMinX() - 40, playerRect.getMinY(), playerRect.getMaxX() -
		playerRect.getMinX() + 80, playerRect.getMaxY() - playerRect.getMinY());
	auto fac = Factory::getInstance();
	Sprite* collision = fac->collider(attackRect);
	if (collision != NULL) {
		fac->removeMonster(collision);
		killnum++;
		int value = killnum;
		database->setIntegerForKey("value", value);
		return true;
	}
		return false;
}

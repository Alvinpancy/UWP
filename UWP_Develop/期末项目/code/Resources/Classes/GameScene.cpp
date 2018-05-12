#include "GameScene.h"
#include <algorithm>
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace CocosDenshion;

Scene* GameScene::createScene() {

	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;

}

bool GameScene::init() {

	if (!Layer::init()) {
		return false;
	}

	isMove = false;

	visibleSize = Director::getInstance()->getVisibleSize();

	// 加载粒子
	loadParticle();

	// 预加载音乐
	preloadMusic();

	// 播放背景音乐
	playBgm();

	// 添加角色
	addPlayer();

	// 添加监听
	addListener();
	addTouchListener();

	// 调度器
	schedule(schedule_selector(GameScene::update), 0.04f, kRepeatForever, 0);
	
	// 添加背景
	auto bgsprite = Sprite::create("star.png");
	bgsprite->setPosition(visibleSize / 2);
	bgsprite->setScale(visibleSize.width / bgsprite->getContentSize().width,
		visibleSize.height / bgsprite->getContentSize().height);
	this->addChild(bgsprite, 0);

	return true;

}

void GameScene::update(float f) {

	if (isMove) {
		move(movekey);
	}

}

// 加载粒子特效
void GameScene::loadParticle() {

	auto paopao = ParticleSystemQuad::create("particle_texture2.plist");
	paopao->setPositionType(ParticleSystemQuad::PositionType::RELATIVE);
	paopao->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 120);
	paopao->setScale(0.6f);
	this->addChild(paopao, 1);

}

// 预加载音乐文件
void GameScene::preloadMusic() {

	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("music/bgm.mp3");
	audio->preloadEffect("music/move.wav");

}

// 播放背景音乐
void GameScene::playBgm() {

	auto audio = SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("music/bgm.mp3", true);

}

// 添加玩家
void GameScene::addPlayer() {

	player = Sprite::create("yun.png");
	player->setScale(0.3f);
	player->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 100);

	player1 = Sprite::create("player.png");
	player1->setScale(0.15f);
	player1->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 140);

	player2 = Sprite::create("yugan.png");
	player2->setScale(0.1f);
	player2->setPosition(visibleSize.width / 2 - 50, visibleSize.height / 2 + 140);

	fish_string = Sprite::create("string.png");
	fish_string->setScale(0.25f);
	fish_string->setScaleX(0.1f);
	fish_string->setAnchorPoint(Vec2(0.5, 1.0));
	fish_string->setPosition(visibleSize.width / 2 - 100, visibleSize.height / 2 + 200);

	/*yugou = Sprite::create("yugou1.png");
	yugou->setScale(0.15f);
    yugou->setAnchorPoint(Vec2(0.5, 1.0));
	yugou->setPosition(visibleSize.width / 2 - 100, visibleSize.height / 2);*/

	addChild(player1, 2);
	addChild(player2, 2);
	addChild(fish_string, 2);
	addChild(player,2);
	//addChild(yugou, 2);

}

void GameScene::movefish() {
	
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
	
	switch (code) {
		// 向左移动
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_CAPITAL_A:
		case EventKeyboard::KeyCode::KEY_A:
			movekey = 'A';
			isMove = true;
			break;

		// 向右移动
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_CAPITAL_D:
		case EventKeyboard::KeyCode::KEY_D:
			movekey = 'D';
			isMove = true;
			break;

	
		case EventKeyboard::KeyCode::KEY_S:
			movekey = 'S';
			isMove = true;
			break;

		default:
			break;
	}

}

void GameScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {

	switch (code) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
		case EventKeyboard::KeyCode::KEY_CAPITAL_A:
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
		case EventKeyboard::KeyCode::KEY_CAPITAL_D:
		case EventKeyboard::KeyCode::KEY_S:
			isMove = false;
			break;
	}

}

void GameScene::addListener() {

	addKeyBoardListener();

}

void GameScene::addKeyBoardListener() {

	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	this->getEventDispatcher()->addEventListenerWithFixedPriority(keyboardListener, 1);

}

void GameScene::move(char c) {

	auto position = player->getPosition();
	auto position1 = player1->getPosition();
	auto position2 = player2->getPosition();
	auto position3 = fish_string->getPosition();
	//auto position4 = yugou->getPosition();

	switch (c) {

	case 'A':
		if (position.x - 5 >= 0)
			player->setPosition(position.x - 5, position.y);
		    player1->setPosition(position1.x - 5, position1.y);
			player2->setPosition(position2.x - 5, position2.y);
			fish_string->setPosition(position3.x - 5, position3.y);
			//yugou->setPosition(position4.x - 5, position4.y);
		break;
	case 'D':
		if (position.x + 5 <= visibleSize.width)
			player->setPosition(position.x + 5, position.y);
		    player1->setPosition(position1.x + 5, position1.y);
			player2->setPosition(position2.x + 5, position2.y);
			fish_string->setPosition(position3.x + 5, position3.y);
			//yugou->setPosition(position4.x + 5, position4.y);
		break;
	case 'S':
		auto position = player->getPosition();
		auto x = position.x;
		float num = x / 1000;
		//fish_string->setAnchorPoint(Point(player2->getPosition().x, player2->getPosition().y));
		fish_string->setAnchorPoint(Point(0.5,1.0));
		fish_string->setScaleY(num);
		break;
	}

}

// 添加触摸事件监听器
void GameScene::addTouchListener() {
	// Todo
	EventListenerTouchOneByOne* touchlistener = EventListenerTouchOneByOne::create();
	touchlistener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	touchlistener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistener, this);
}

bool GameScene::onTouchBegan(Touch *touch, Event *event) {
	return true;
}

// 当鼠标按住飞船后可控制飞船移动 (加分项)
void GameScene::onTouchMoved(Touch *touch, Event *event) {
	// Todo
	auto loc = touch->getLocation();
	//auto pos = yugou->getPosition();
	//if ((pos.x - 40 <= loc.x&&loc.x <= pos.x + 40) && (pos.y - 40 <= loc.y&&loc.y <= pos.y + 40))
		//yugou->setPositionY(loc.y);
	auto x = visibleSize.height - loc.y;
	float num = x / 800;
	//fish_string->setAnchorPoint(Point(player2->getPosition().x, player2->getPosition().y));
	fish_string->setAnchorPoint(Point(0.5, 1.0));
	fish_string->setScaleY(num);
		
}
 
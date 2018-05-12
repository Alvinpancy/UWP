#pragma execution_character_set("utf-8")
#include "FriendShip.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#define database UserDefault::getInstance()

USING_NS_CC;
using namespace CocosDenshion;

void FriendShip::setPhysicsWorld(PhysicsWorld* world) { m_world = world; }

Scene* FriendShip::createScene() {
  srand((unsigned)time(NULL));
  auto scene = Scene::createWithPhysics();
  
  scene->getPhysicsWorld()->setAutoStep(false);

  // Debug 模式
  // 开启debug模式需要setAutoStep(true) 并注释掉update函数第一行
  //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

  scene->getPhysicsWorld()->setGravity(Vec2(0, -300.0f));
  auto layer = FriendShip::create();
  layer->setPhysicsWorld(scene->getPhysicsWorld());
  scene->addChild(layer);
  return scene;
}

// on "init" you need to initialize your instance
bool FriendShip::init() {
  //////////////////////////////
  // 1. super init first
  if (!Layer::init()) {
    return false;
  }
  visibleSize = Director::getInstance()->getVisibleSize();

  preloadMusic(); // 预加载音效

  addSprite();    // 添加背景和各种精灵
  addListener();  // 添加监听器 
  addPlayer();    // 添加玩家

  //添加粒子系统
  auto paopao = ParticleSystemQuad::create("particle_texture2.plist");
  paopao->setPositionType(ParticleSystemQuad::PositionType::RELATIVE);
  paopao->setPosition(visibleSize.width / 2, visibleSize.height / 2);
  paopao->setScale(0.6f);
  paopao->setScaleX(1.0f);
  this->addChild(paopao, 1);

  // 添加调度器
  schedule(schedule_selector(FriendShip::update), 0.01f, kRepeatForever, 0.1f);
  schedule(schedule_selector(FriendShip::boxFall), 4.0f, kRepeatForever, 0);
  schedule(schedule_selector(FriendShip::updateShip), 4.0f, kRepeatForever, 2.0f);
  
  deltaH = 0;    // 每次更新的高度变化量
  height = 0;    // 左右高度差
  LastTag = -1;
  counter = 0;
  holdbox1 = NULL;

  return true;
}

// 预加载音效
void FriendShip::preloadMusic() {
  auto sae = SimpleAudioEngine::getInstance();
  sae->preloadEffect("boxfall.mp3");
  sae->preloadEffect("gameover.mp3");
  sae->preloadEffect("jump.mp3");
  sae->preloadBackgroundMusic("bgm.mp3");
  
  sae->playBackgroundMusic("bgm.mp3", true);
}

// 添加背景和各种精灵
void FriendShip::addSprite() {
  // add background
  auto bgSprite = Sprite::create("bg.png");
  bgSprite->setPosition(visibleSize / 2);
  bgSprite->setScale(visibleSize.width / bgSprite->getContentSize().width, visibleSize.height / bgSprite->getContentSize().height);
  this->addChild(bgSprite, 0);

  // add ship
  ship = Sprite::create("ship.png");
  ship->setScale(visibleSize.width / ship->getContentSize().width * 0.97, 1.2f);
  ship->setPosition(visibleSize.width / 2, 110);
  auto shipbody = PhysicsBody::createBox(ship->getContentSize(), PhysicsMaterial(100.0f, 0.0f, 1.0f));
  shipbody->setCategoryBitmask(0xFFFFFFFF);
  shipbody->setCollisionBitmask(0xFFFFFFFF);
  shipbody->setContactTestBitmask(0xFFFFFFFF);
  shipbody->setDynamic(false);  // 静态刚体不受重力影响, 同时也无法发生倾斜了
  ship->setPhysicsBody(shipbody);
  ship->setAnchorPoint(Vec2(0.5, 0.5));
  this->addChild(ship, 1);

  // add sea
  auto seaSprite = Sprite::create("sea.png");
  seaSprite->setPosition(visibleSize.width / 2, 60);
  seaSprite->setScale(visibleSize.width / seaSprite->getContentSize().width, 120 / seaSprite->getContentSize().height);
  seaSprite->setOpacity(230);
  this->addChild(seaSprite, 3);

  // add sun and cloud
  auto sunSprite = Sprite::create("sun.png");
  sunSprite->setPosition(rand() % (int)(visibleSize.width - 200) + 100, 550);
  this->addChild(sunSprite);
  auto cloudSprite1 = Sprite::create("cloud.png");
  cloudSprite1->setPosition(rand() % (int)(visibleSize.width - 200) + 100, rand() % 100 + 450);
  this->addChild(cloudSprite1);
  auto cloudSprite2 = Sprite::create("cloud.png");
  cloudSprite2->setPosition(rand() % (int)(visibleSize.width - 200) + 100, rand() % 100 + 450);
  this->addChild(cloudSprite2);
}

// 添加监听器
void FriendShip::addListener() {
  auto keyboardListener = EventListenerKeyboard::create();
  keyboardListener->onKeyPressed = CC_CALLBACK_2(FriendShip::onKeyPressed, this);
  keyboardListener->onKeyReleased = CC_CALLBACK_2(FriendShip::onKeyReleased, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

  auto contactListener = EventListenerPhysicsContact::create();
  contactListener->onContactBegin = CC_CALLBACK_1(FriendShip::onConcactBegin, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

// 创建角色
void FriendShip::addPlayer() {
  IsPlayer1Jump = false;
  IsPlayer1Left = false;
  IsPlayer1Right = false;
  IsPlayer1Hold = false;
  LastPlayer1Press = 'D';

  //create player1
  auto texture1 = Director::getInstance()->getTextureCache()->addImage("player1.png");
  frame1 = SpriteFrame::createWithTexture(texture1, CC_RECT_PIXELS_TO_POINTS(Rect(0, 4, 32, 36)));
  player1 = Sprite::createWithSpriteFrame(frame1);
  int xpos = visibleSize.width / 2;
  player1->setPosition(Vec2(xpos, 185));

  // 设置角色刚体属性
  auto playerbody = PhysicsBody::createBox(player1->getContentSize(), PhysicsMaterial(10.0f, 0.0f, 1.0f));
  playerbody->setCategoryBitmask(0x0000FFFF);
  playerbody->setCollisionBitmask(0x0000FFFF);
  playerbody->setContactTestBitmask(0x0000FFFF);
  player1->setPhysicsBody(playerbody);

  player1->setName("player1");
  this->addChild(player1, 2);
  
  loadAnimation("player1"); // 加载人物动画
}

//加载玩家动画帧, 共8种 :)
void FriendShip::loadAnimation(string filepath) {
  Vector<SpriteFrame*> PlayerIdleWithBox;
  Vector<SpriteFrame*> PlayerIdleWithoutBox;
  Vector<SpriteFrame*> PlayerPutUp;
  Vector<SpriteFrame*> PlayerPutDown;
  Vector<SpriteFrame*> PlayerRunWithBox;
  Vector<SpriteFrame*> PlayerRunWithoutBox;
  Vector<SpriteFrame*> PlayerJumpWithBox;
  Vector<SpriteFrame*> PlayerJumpWithoutBox;

  auto PlayerImage = Director::getInstance()->getTextureCache()->addImage(filepath + ".png");

  PlayerIdleWithoutBox.reserve(3);
  for (int i = 0; i < 3; i++) {
    auto frame = SpriteFrame::createWithTexture(PlayerImage, CC_RECT_PIXELS_TO_POINTS(Rect(32 * i, 5, 32, 36)));
    PlayerIdleWithoutBox.pushBack(frame);
  }
  Animation* PlayerIdelWithoutBoxAnimation = Animation::createWithSpriteFrames(PlayerIdleWithoutBox, 0.1f);
  AnimationCache::getInstance()->addAnimation(PlayerIdelWithoutBoxAnimation, filepath + "IdleWithoutBoxAnimation");

  PlayerIdleWithBox.reserve(3);
  auto IdleWithBox = SpriteFrame::createWithTexture(PlayerImage, CC_RECT_PIXELS_TO_POINTS(Rect(96, 7, 32, 36)));
  PlayerIdleWithBox.pushBack(IdleWithBox);
  if (filepath == "player1") IdleWithBox1 = IdleWithBox;
  else IdleWithBox2 = IdleWithBox;
  for (int i = 1; i < 3; i++) {
    auto frame = SpriteFrame::createWithTexture(PlayerImage, CC_RECT_PIXELS_TO_POINTS(Rect(96 + 32 * i, 7, 32, 33)));
    PlayerIdleWithBox.pushBack(frame);
  }
  Animation* PlayerIdleWithBoxAnimation = Animation::createWithSpriteFrames(PlayerIdleWithBox, 0.1f);
  AnimationCache::getInstance()->addAnimation(PlayerIdleWithBoxAnimation, filepath + "IdleWithBoxAnimation");

  for (int i = 0; i < 3; i++) {
    auto frame = SpriteFrame::createWithTexture(PlayerImage, CC_RECT_PIXELS_TO_POINTS(Rect(96 + 32 * i, 4, 32, 36)));
    PlayerPutUp.pushBack(frame);
  }
  Animation* PlayerPutUpAnimation = Animation::createWithSpriteFrames(PlayerPutUp, 0.1f);
  AnimationCache::getInstance()->addAnimation(PlayerPutUpAnimation, filepath + "PutUpAnimation");

  for (int i = 0; i < 2; i++) {
    auto frame = SpriteFrame::createWithTexture(PlayerImage, CC_RECT_PIXELS_TO_POINTS(Rect(193 + 32 * i, 4, 32, 36)));
    PlayerPutDown.pushBack(frame);
  }
  if (filepath == "player1")
    PlayerPutDown.pushBack(frame1);
  else
    PlayerPutDown.pushBack(frame2);
  Animation* PlayerPutDownAnimation = Animation::createWithSpriteFrames(PlayerPutDown, 0.1f);
  AnimationCache::getInstance()->addAnimation(PlayerPutDownAnimation, filepath + "PutDownAnimation");

  PlayerRunWithoutBox.reserve(8);
  for (int i = 0; i < 8; i++) {
    auto frame = SpriteFrame::createWithTexture(PlayerImage, CC_RECT_PIXELS_TO_POINTS(Rect(32 * i, 41, 32, 36)));
    PlayerRunWithoutBox.pushBack(frame);
  }

  Animation* PlayerRunWithoutBoxAnimation = Animation::createWithSpriteFrames(PlayerRunWithoutBox, 0.1f);
  AnimationCache::getInstance()->addAnimation(PlayerRunWithoutBoxAnimation, filepath + "RunWithoutBoxAnimation");

  PlayerRunWithBox.reserve(8);
  for (int i = 0; i < 8; i++) {
    auto frame = SpriteFrame::createWithTexture(PlayerImage, CC_RECT_PIXELS_TO_POINTS(Rect(32 * i, 81, 32, 36)));
    PlayerRunWithBox.pushBack(frame);
  }
  Animation* PlayerRunWithBoxAnimation = Animation::createWithSpriteFrames(PlayerRunWithBox, 0.1f);
  AnimationCache::getInstance()->addAnimation(PlayerRunWithBoxAnimation, filepath + "RunWithBoxAnimation");

  PlayerJumpWithoutBox.pushBack(SpriteFrame::createWithTexture(PlayerImage, CC_RECT_PIXELS_TO_POINTS(Rect(64, 41, 32, 36))));
  if (filepath == "player1")
    PlayerJumpWithoutBox.pushBack(frame1);
  else
    PlayerJumpWithoutBox.pushBack(frame2);
  Animation* PlayerJumpWithoutBoxAnimation = Animation::createWithSpriteFrames(PlayerJumpWithoutBox, 1.0f);
  AnimationCache::getInstance()->addAnimation(PlayerJumpWithoutBoxAnimation, filepath + "JumpWithoutBoxAnimation");

  PlayerJumpWithBox.pushBack(SpriteFrame::createWithTexture(PlayerImage, CC_RECT_PIXELS_TO_POINTS(Rect(64, 81, 32, 36))));
  PlayerJumpWithBox.pushBack(IdleWithBox);
  Animation* PlayerJumpWithBoxAnimation = Animation::createWithSpriteFrames(PlayerJumpWithBox, 1.0);
  AnimationCache::getInstance()->addAnimation(PlayerJumpWithBoxAnimation, filepath + "JumpWithBoxAnimation");
}

void FriendShip::update(float dt) {
	this->getScene()->getPhysicsWorld()->step(1 / 100.0f);
	// 玩家1是否在举着箱子的情况下 左右移动
	if (IsPlayer1Left || IsPlayer1Right) {
		if (player1->getSpriteFrame() == frame1) {
			auto animation = RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("player1RunWithoutBoxAnimation")));
			animation->setTag(11);
			player1->runAction(animation);
		}
		else if (player1->getSpriteFrame() == IdleWithBox1) {
			auto animation = RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("player1RunWithBoxAnimation")));
			animation->setTag(11);
			player1->runAction(animation);
		}

		bool leftmove = true;
		bool rightmove = true;
		for (list<PhysicsBody*>::iterator it = boxes.begin(); it != boxes.end(); it++) {
			if (player1->getBoundingBox().intersectsRect((*it)->getNode()->getBoundingBox())) {
				if (holdbox1 != NULL && holdbox1->getPhysicsBody()->getTag() != (*it)->getTag()) {
					if ((*it)->getPosition().x < player1->getPhysicsBody()->getPosition().x) leftmove = false;
					else rightmove = false;
				}
				if (holdbox1 == NULL) {
					if ((*it)->getPosition().x < player1->getPhysicsBody()->getPosition().x) leftmove = false;
					else rightmove = false;
				}
			}
		}

		auto position = player1->getPosition();
		if (IsPlayer1Left && leftmove) {
			auto moveto = MoveTo::create(0.15f, Vec2(position.x - 1, position.y));
			player1->runAction(moveto);
			player1->setFlippedX(true);
			if (holdbox1 != NULL) {
				auto position1 = holdbox1->getPosition();
				auto movet = MoveTo::create(0.15f, Vec2(position1.x - 1, position1.y));
				holdbox1->runAction(movet);
			}
		}
		    if (IsPlayer1Right && rightmove) {
				auto moveto = MoveTo::create(0.15f, Vec2(position.x + 1, position.y));
				player1->runAction(moveto);
				player1->setFlippedX(false);
				if (holdbox1 != NULL) {
					auto position1 = holdbox1->getPosition();
					auto movet = MoveTo::create(0.15f, Vec2(position1.x + 1, position1.y));
					holdbox1->runAction(movet);
				}
		}
}

		if (IsPlayer1Hold && holdbox1 == NULL) {
			auto position = player1->getPosition();
			for (list<PhysicsBody*>::iterator it = boxes.begin(); it != boxes.end(); it++) {
				if (player1->getBoundingBox().intersectsRect((*it)->getNode()->getBoundingBox())) {
					joint1 = PhysicsJointDistance::construct(player1->getPhysicsBody(), (*it),\
						player1->getAnchorPoint(), (*it)->getNode()->getAnchorPoint());
					m_world->addJoint(joint1);
					holdbox1 = (*it)->getNode();
					auto rotate = holdbox1->getRotation();
					auto rotateby = RotateBy::create(0.01f, 360 - rotate);
					holdbox1->runAction(rotateby);
					holdbox1->setPosition(position.x, position.y + 35);
					player1->setSpriteFrame(IdleWithBox1);
					break;
				}
			}
		}  //  end if IsPlayer1Hold
		else if (!IsPlayer1Hold && holdbox1 != NULL) {
			auto pos = holdbox1->getPhysicsBody();
			if (LastPlayer1Press == 'D')
				holdbox1->getPhysicsBody()->applyImpulse(Vec2(1500000, 1500000), Vec2(0, 0));
			else if (LastPlayer1Press == 'A')
				holdbox1->getPhysicsBody()->applyImpulse(Vec2(-1500000, 1500000), Vec2(0, 0));
			holdbox1 = NULL;
			player1->setSpriteFrame(frame1);
			m_world->removeAllJoints();
		}
		player1->getPhysicsBody()->addMoment(9999999999999999999);
}

// 更新船的平衡和倾斜(加分项)
// Todo
void FriendShip::updateShip(float dt) {
	list<PhysicsBody*>::iterator it = boxes.begin();
	int rotate = 0;
	for (; it != boxes.end(); it++) {
		list<PhysicsBody*>::iterator temp = it;
		temp++;
		if (temp == boxes.end() && LastTag != (*it)->getTag()) {
			rotate = ((*it)->getNode()->getPosition().x - visibleSize.width / 2) / 100;
			LastTag = (*it)->getTag();
			break;
		}
	}
	ship->runAction(RotateBy::create(0.1f, rotate));
	int left = 0;
	int right = 0;
	for (list<PhysicsBody*>::iterator ite = boxes.begin(); ite != boxes.end(); ite++) {
		if ((*ite)->getNode()->getPosition().x < visibleSize.width / 2) left++;
		if ((*ite)->getNode()->getPosition().x > visibleSize.width / 2) right++;
		int differ = left - right;
		if (differ < -5 || differ > 5) GameOver();
	}
}


// 掉落箱子 
// 设置掩码使箱子掉落过程不会砸到玩家
// 为了增加难度，箱子将掉落在船重的一端
// Todo
void FriendShip::boxFall(float dt) {
  auto box = Sprite::create("box.png");
  // 为箱子设置刚体属性
  auto boxbody = PhysicsBody::createBox(box->getContentSize(), PhysicsMaterial(10.0f, 0.0f, 1.0f));
  boxbody->setCategoryBitmask(0xFFFF0000);
  boxbody->setCollisionBitmask(0xFFFF0000);
  boxbody->setContactTestBitmask(0xFFFF0000);
  boxbody->setTag(++counter);
  box->setPhysicsBody(boxbody);

  // 掉落在重的一侧
  if (height > 0)       // 左右高度差大于0(左边重)
    box->setPosition(rand() % (int)(visibleSize.width / 2 - 50) + 50, visibleSize.height);
  else if (height < 0)  // 左右高度差小于0(右边重)
    box->setPosition(rand() % (int)(visibleSize.width / 2 - 50) + visibleSize.width / 2, visibleSize.height);
  else
	  box->setPosition(rand() % (int)(visibleSize.width - 100) + 50, visibleSize.height);
  boxes.push_back(boxbody);
  this->addChild(box);
}


// 上左右 举起箱子 扔掉箱子
void FriendShip::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
	Action* animation;
	switch (code) {
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		// 左右移动
		if (code == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
			IsPlayer1Left = true;
			LastPlayer1Press = 'A';
		}
		else {
			IsPlayer1Right = true;
			LastPlayer1Press = 'D';
		}
		// Todo
		break;

	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
		// 跳
		player1->getPhysicsBody()->applyImpulse(Vec2(0, 5000000), Vec2(0, 0));
		// Todo
		break;

		// 使用固定距离关节
	case cocos2d::EventKeyboard::KeyCode::KEY_ENTER:
		// 举起或扔掉箱子
		IsPlayer1Hold = !IsPlayer1Hold;
		// Todo
		break;
	default:
		break;
	}
}

// 释放按键
void FriendShip::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		if (code == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
			IsPlayer1Left = false;
		else
			IsPlayer1Right = false;

		// 停止动画和运动
		player1->stopAllActionsByTag(11);
		player1->setSpriteFrame(frame1);
		// Todo
		break;

	default:
		break;
	}
}

// 箱子碰到船或者碰到其他箱子之后改变掩码，可以与玩家发生碰撞
// Todo
bool FriendShip::onConcactBegin(PhysicsContact & contact) {
	auto BodyA = contact.getShapeA()->getBody();
	auto BodyB = contact.getShapeB()->getBody();
	if (BodyA->getCategoryBitmask() != 0x0000FFFF && BodyB->getCategoryBitmask() != 0x0000FFFF) {
		BodyA->setCategoryBitmask(0x000FFFFF);
		BodyA->setCollisionBitmask(0x000FFFFF);
		BodyA->setContactTestBitmask(0x000FFFFF);
		BodyB->setCategoryBitmask(0x000FFFFF);
		BodyB->setCollisionBitmask(0x000FFFFF);
		BodyB->setContactTestBitmask(0x000FFFFF);
	}
  return true;
}

// 游戏结束, 小船说翻就翻
void FriendShip::GameOver() {
	unschedule(schedule_selector(FriendShip::updateShip));
	unschedule(schedule_selector(FriendShip::boxFall));
	//  unschedule(schedule_selector(FriendShip::update));
	SimpleAudioEngine::getInstance()->stopBackgroundMusic("bgm.mp3");
	SimpleAudioEngine::getInstance()->playEffect("gameover.mp3", false);

	auto label1 = Label::createWithTTF("单身狗独木舟说沉就沉", "fonts/STXINWEI.TTF", 60);
	label1->setColor(Color3B(0, 0, 0));
	label1->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(label1);

	auto label2 = Label::createWithTTF("重玩", "fonts/STXINWEI.TTF", 40);
	label2->setColor(Color3B(0, 0, 0));
	auto replayBtn = MenuItemLabel::create(label2, CC_CALLBACK_1(FriendShip::replayCallback, this));
	Menu* replay = Menu::create(replayBtn, NULL);
	replay->setPosition(visibleSize.width / 2 - 80, visibleSize.height / 2 - 100);
	this->addChild(replay);

	auto label3 = Label::createWithTTF("退出", "fonts/STXINWEI.TTF", 40);
	label3->setColor(Color3B(0, 0, 0));
	auto exitBtn = MenuItemLabel::create(label3, CC_CALLBACK_1(FriendShip::exitCallback, this));
	Menu* exit = Menu::create(exitBtn, NULL);
	exit->setPosition(visibleSize.width / 2 + 90, visibleSize.height / 2 - 100);
	this->addChild(exit);
}

// 继续或重玩按钮响应函数
void FriendShip::replayCallback(Ref * pSender) {
	Director::getInstance()->replaceScene(FriendShip::createScene());
}

// 退出
void FriendShip::exitCallback(Ref * pSender) {
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

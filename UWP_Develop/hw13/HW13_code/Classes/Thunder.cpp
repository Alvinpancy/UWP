#include "Thunder.h"
#include <algorithm>

USING_NS_CC;

using namespace CocosDenshion;

Scene* Thunder::createScene() {
  // 'scene' is an autorelease object
  auto scene = Scene::create();

  // 'layer' is an autorelease object
  auto layer = Thunder::create();

  // add layer as a child to scene
  scene->addChild(layer);

  // return the scene
  return scene;
}

bool Thunder::init() {
  if (!Layer::init()) {
    return false;
  }
  row = 3;
  isMove = false;  // 是否点击飞船
  visibleSize = Director::getInstance()->getVisibleSize();

  // 创建背景
  auto bgsprite = Sprite::create("bg.jpg");
  bgsprite->setPosition(visibleSize / 2);
  bgsprite->setScale(visibleSize.width / bgsprite->getContentSize().width,
                     visibleSize.height / bgsprite->getContentSize().height);
  this->addChild(bgsprite, 0);

  // 创建飞船
  player = Sprite::create("player.png");
  player->setAnchorPoint(Vec2(0.5, 0.5));
  player->setPosition(visibleSize.width / 2, player->getContentSize().height);
  player->setName("player");
  this->addChild(player, 1);

  // 显示陨石和子弹数量
  enemysNum = Label::createWithTTF("enemys: 0", "fonts/arial.TTF", 20);
  enemysNum->setColor(Color3B(255, 255, 255));
  enemysNum->setPosition(50, 60);
  this->addChild(enemysNum, 3);
  bulletsNum = Label::createWithTTF("bullets: 0", "fonts/arial.TTF", 20);
  bulletsNum->setColor(Color3B(255, 255, 255));
  bulletsNum->setPosition(50, 30);
  this->addChild(bulletsNum, 3);

  addEnemy(5);        // 初始化陨石
  preloadMusic();     // 预加载音乐
  playBgm();          // 播放背景音乐
  explosion();        // 创建爆炸帧动画

  // 添加监听器
  addTouchListener();
  addKeyboardListener();
  addCustomListener();

  // 调度器
  schedule(schedule_selector(Thunder::update), 0.04f, kRepeatForever, 0);

  return true;
}

//预加载音乐文件
void Thunder::preloadMusic() {
  // Todo
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("music/bgm.mp3");
	audio->preloadEffect("music/explore.mp3");
	audio->preloadEffect("music/fire.mp3");
}

//播放背景音乐
void Thunder::playBgm() {
  // Todo
	auto audio = SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("music/bgm.mp3", true);
}

//初始化陨石
void Thunder::addEnemy(int n) {
  enemys.clear();
  for (unsigned i = 0; i < 3; ++i) {
    char enemyPath[20];
    sprintf(enemyPath, "stone%d.png", 3 - i);
    double width = visibleSize.width / (n + 1.0),
           height = visibleSize.height - (50 * (i + 1));
    for (int j = 0; j < n; ++j) {
      auto enemy = Sprite::create(enemyPath);
      enemy->setAnchorPoint(Vec2(0.5, 0.5));
      enemy->setScale(0.5, 0.5);
      enemy->setPosition(width * (j + 1), height);
      enemys.push_back(enemy);
      addChild(enemy, 1);
    }
  }
}

// 陨石向下移动并生成新的一行(加分项)
void Thunder::newEnemy() {
  // Todo
	int picnum = 1 + row % 3;
	row++;
	list<Sprite*>::iterator it = enemys.begin();
	for (; it != enemys.end(); it++) {
		auto x = (*it)->getPosition().x;
		auto y = (*it)->getPosition().y - 50;
		(*it)->setPosition(x, y);
	}
	char enemyPath[20];
	sprintf(enemyPath, "stone%d.png", picnum);
	double width = visibleSize.width / 6, height = visibleSize.height - 50;
	for (int i = 0; i < 5; i++) {
		auto enemy = Sprite::create(enemyPath);
		enemy->setAnchorPoint(Vec2(0.5, 0.5));
		enemy->setScale(0.5, 0.5);
		enemy->setPosition(width * (i + 0.5), height);
		enemys.push_back(enemy);
		addChild(enemy, 1);
	}
}

// 移动飞船
void Thunder::movePlane(char c) {
  // Todo
	auto position = player->getPosition();
	if (c == 'A') {
		if (position.x >= 40) {
			auto movet = MoveBy::create(0.08, Vec2(-20, 0));
			player->runAction(movet);
		}
	}
	else if (c == 'D') {
		if (position.x <= visibleSize.width - 40) {
			auto movet = MoveBy::create(0.08, Vec2(20, 0));
			player->runAction(movet);
		}
	}
}

//发射子弹
void Thunder::fire() {
  auto bullet = Sprite::create("bullet.png");
  bullet->setAnchorPoint(Vec2(0.5, 0.5));
  bullets.push_back(bullet);
  bullet->setPosition(player->getPosition());
  addChild(bullet, 1);
  SimpleAudioEngine::getInstance()->playEffect("music/fire.wav", false);

  // 移除飞出屏幕外的子弹
  // Todo
  auto seq = Sequence::create(MoveBy::create(1.0f, Vec2(0, visibleSize.height)), CallFunc::create([&, bullet]() {
	  bullets.remove(bullet);
  }), RemoveSelf::create(), nullptr);
  bullet->runAction(seq);
}

// 切割爆炸动画帧
void Thunder::explosion() {
  // Todo
	auto texture = Director::getInstance()->getTextureCache()->addImage("explosion.png");
	explore.reserve(8);
	for (int i = 0; i < 5; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0 * i, 212, 210, 180)));
		explore.pushBack(frame);
	}
	for (int i = 5; i < 8; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0 * i, 0, 210, 180)));
		explore.pushBack(frame);
	}
}

void Thunder::update(float f) {
	// 实时更新页面内陨石和子弹数量(不得删除)
	// 要求数量显示正确(加分项)
	char str[15];
	sprintf(str, "enemys: %d", enemys.size());
	enemysNum->setString(str);
	sprintf(str, "bullets: %d", bullets.size());
	bulletsNum->setString(str);

	// 飞船移动
	if (isMove)
		this->movePlane(movekey);

	static int ct = 0;
	static int dir = 4;
	++ct;
	if (ct == 120)
		ct = 40, dir = -dir;
	else if (ct == 80) {
		dir = -dir;
		newEnemy();  // 陨石向下移动并生成新的一行(加分项)
	}
	else if (ct == 20)
		ct = 40, dir = -dir;

	//陨石左右移动
	for (Sprite* s : enemys) {
		if (s != NULL) {
			s->setPosition(s->getPosition() + Vec2(dir, 0));
		}
	}
	// 分发自定义事件
	EventCustom e("meet");
	_eventDispatcher->dispatchEvent(&e);
}

// 自定义碰撞事件
void Thunder::meet(EventCustom * event) {
  // 判断子弹是否打中陨石并执行对应操作
  // Todo
    list<Sprite*>::iterator it1 = bullets.begin();
    list<Sprite*>::iterator it2 = enemys.begin();
	bool flag = false;
	for (; it1 != bullets.end();) {
		for (; it2 != enemys.end();) {
			if ((*it1)->getPosition().getDistance((*it2)->getPosition()) < 50) {
				SimpleAudioEngine::getInstance()->playEffect("music/explore.wav", false);
				Sprite* temp = (*it2);
				Sprite* bullet = (*it1);
				(*it2)->runAction(
					Sequence::create(
						Animate::create(Animation::createWithSpriteFrames(explore, 0.05f, 1)),
						CallFunc::create([temp] {temp->removeFromParentAndCleanup(true);}),
						nullptr)
				);  //  end of runAction
				(*it1)->runAction(
						CallFunc::create([bullet] {bullet->removeFromParentAndCleanup(true); })
				);  //  end of runAction
				it2 = enemys.erase(it2);
				it1 = bullets.erase(it1);
				flag = true;
				break;
			}
			else it2++;
		}
		if (!flag) it1++;
		flag = false;
	}
  // 判断游戏是否结束并执行对应操作
	list<Sprite*>::iterator it = enemys.begin();
	for (; it != enemys.end(); it++) {
		if ((*it)->getPosition().y < player->getPosition().y + 40) {
			SimpleAudioEngine::getInstance()->playEffect("music/explore.wav", false);
			Sprite* temp2 = player;
			player->runAction(
				Sequence::create(
					Animate::create(Animation::createWithSpriteFrames(explore, 0.05f, 1)),
					FadeOut::create(0.001f),
					nullptr)
			);  //  end of runAction
			auto gameover = Sprite::create("gameOver.png");
			gameover->setPosition(visibleSize.width / 2, visibleSize.height / 2);
			this->addChild(gameover, 1);
			unschedule(schedule_selector(Thunder::update));
			_eventDispatcher->removeAllEventListeners();
			break;
		}
	}
  // Todo
}

// 添加自定义监听器
void Thunder::addCustomListener() {
  // Todo
	auto meetListener = EventListenerCustom::create("meet", CC_CALLBACK_1(Thunder::meet, this));
	_eventDispatcher->addEventListenerWithFixedPriority(meetListener, 1);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(meetListener, this);
}

// 添加键盘事件监听器
void Thunder::addKeyboardListener() {
  // Todo
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(Thunder::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(Thunder::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void Thunder::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
  switch (code) {
  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
  case EventKeyboard::KeyCode::KEY_CAPITAL_A:
  case EventKeyboard::KeyCode::KEY_A:
    movekey = 'A';
    isMove = true;
    break;
  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
  case EventKeyboard::KeyCode::KEY_CAPITAL_D:
  case EventKeyboard::KeyCode::KEY_D:
    movekey = 'D';
    isMove = true;
    break;
  case EventKeyboard::KeyCode::KEY_SPACE:
    fire();
    break;
  }
}

void Thunder::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
  switch (code) {
  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
  case EventKeyboard::KeyCode::KEY_A:
  case EventKeyboard::KeyCode::KEY_CAPITAL_A:
  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
  case EventKeyboard::KeyCode::KEY_D:
  case EventKeyboard::KeyCode::KEY_CAPITAL_D:
    isMove = false;
    break;
  }
}

// 添加触摸事件监听器
void Thunder::addTouchListener() {
  // Todo
	EventListenerTouchOneByOne* touchlistener = EventListenerTouchOneByOne::create();
	touchlistener->onTouchBegan = CC_CALLBACK_2(Thunder::onTouchBegan, this);
	touchlistener->onTouchMoved = CC_CALLBACK_2(Thunder::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistener, this);
}

// 鼠标点击发射炮弹
bool Thunder::onTouchBegan(Touch *touch, Event *event) {
  fire();
  return true;
}

void Thunder::onTouchEnded(Touch *touch, Event *event) {
  isClick = false;
}

// 当鼠标按住飞船后可控制飞船移动 (加分项)
void Thunder::onTouchMoved(Touch *touch, Event *event) {
  // Todo
	auto loc = touch->getLocation();
	auto pos = player->getPosition();
	if ((pos.x - 40 <= loc.x&&loc.x <= pos.x + 40)&&(pos.y - 40 <= loc.y&&loc.y <= pos.y + 40))
	player->setPositionX(loc.x);
}

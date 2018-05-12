#include "GameScene.h"
#include "MenuScene.h"
#include <algorithm>
#include "SimpleAudioEngine.h"
#include <ctime>
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <regex>
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;

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
	visibleSize = Director::getInstance()->getVisibleSize();
	isMove = false;
	victimFish = NULL;
	score = 0;
	dtime = 50;
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
	// 加载动画资源
	loadAnimation();
	// 调度器
	schedule(schedule_selector(GameScene::update), 0.04f, kRepeatForever, 0);
	schedule(schedule_selector(GameScene::clocker), 1.0f, kRepeatForever, 0);

	rank_field = TextField::create("", "fonts/mango.ttf", 40);
	rank_field->setPosition(Size(visibleSize.width / 4 * 3, visibleSize.height / 2));
	this->addChild(rank_field, 2);
	auto bg = Sprite::create("star.png");
	bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	bg->setScale(1.0f);
	addChild(bg, 0);

	return true;

}

void GameScene::update(float f) {

	if (isMove) {
		move(movekey);
	}

	if (victimFish != NULL)
		victimFish->setPosition(yugou->getPosition());

	// 添加鱼
	static int ct = 0;
	static int dir = 4;
	++ct;
	if (ct == 120)
		ct = 40, dir = -dir;
	else if (ct == 80) {
		dir = -dir;
		addFish();
	}
	else if (ct == 20)
		ct = 40, dir = -dir;

	this->removeChild(scoretext, 1);
	this->removeChild(scoreshow, 1);
	char s[12];
	sprintf(s, "%d", score);
	std::string str = s;
	scoretext = Label::createWithTTF(str, "fonts/mango.ttf", 36);
	scoretext->setPosition(Vec2(visibleSize.width / 5 * 4 + 70, visibleSize.height - 50));
	this->addChild(scoretext, 1);
	scoreshow = Label::createWithTTF("Score: ", "fonts/mango.ttf", 36);
	scoreshow->setPosition(Vec2(visibleSize.width / 5 * 4, visibleSize.height - 50));
	this->addChild(scoreshow, 1);
	// 监视场景中的鱼，是否有游出界面的
	guardFishes();

}

// 加载粒子特效
void GameScene::loadParticle() {

	auto paopao = ParticleSystemQuad::create("particle_texture.plist");
	paopao->setPositionType(ParticleSystemQuad::PositionType::RELATIVE);
	paopao->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 120);
	paopao->setScale(0.6f);
	paopao->setScaleX(1.0f);
	this->addChild(paopao, 2);

}

// 加载动画
void GameScene::loadAnimation() {

	Vector<SpriteFrame*> FishMove1;
	Vector<SpriteFrame*> FishMove2;
	Vector<SpriteFrame*> FishMove3;
	Vector<SpriteFrame*> FishMove4;
	Vector<SpriteFrame*> FishMove5;
	Vector<SpriteFrame*> FishMove6;

	auto fishImage = Director::getInstance()->getTextureCache()->addImage("images/fish1.png");
	FishMove1.reserve(4);
	for (int i = 0; i < 4; i++) {
		auto frame = SpriteFrame::createWithTexture(fishImage, CC_RECT_PIXELS_TO_POINTS(Rect(0, 256 + 65 * i, 75, 36)));
		FishMove1.pushBack(frame);
	}
	Animation* FishAnimation1 = Animation::createWithSpriteFrames(FishMove1, 0.2f);
	AnimationCache::getInstance()->addAnimation(FishAnimation1, "FishMove1");

	fishImage = Director::getInstance()->getTextureCache()->addImage("images/fish2.png");
	FishMove2.reserve(4);
	for (int i = 0; i < 10; i++) {
		auto frame = SpriteFrame::createWithTexture(fishImage, CC_RECT_PIXELS_TO_POINTS(Rect(0, 150 * i, 92, 115)));
		FishMove2.pushBack(frame);
	}
	Animation* FishAnimation2 = Animation::createWithSpriteFrames(FishMove2, 0.2f);
	AnimationCache::getInstance()->addAnimation(FishAnimation2, "FishMove2");

	fishImage = Director::getInstance()->getTextureCache()->addImage("images/fish3.png");
	FishMove3.reserve(8);
	for (int i = 0; i < 8; i++) {
		auto frame = SpriteFrame::createWithTexture(fishImage, CC_RECT_PIXELS_TO_POINTS(Rect(0, 122 * i, 100, 92)));
		FishMove3.pushBack(frame);
	}
	Animation* FishAnimation3 = Animation::createWithSpriteFrames(FishMove3, 0.2f);
	AnimationCache::getInstance()->addAnimation(FishAnimation3, "FishMove3");

	fishImage = Director::getInstance()->getTextureCache()->addImage("images/fish4.png");
	FishMove4.reserve(12);
	for (int i = 0; i < 12; i++) {
		auto frame = SpriteFrame::createWithTexture(fishImage, CC_RECT_PIXELS_TO_POINTS(Rect(0, 79 * i, 102, 73)));
		FishMove4.pushBack(frame);
	}
	Animation* FishAnimation4 = Animation::createWithSpriteFrames(FishMove4, 0.2f);
	AnimationCache::getInstance()->addAnimation(FishAnimation4, "FishMove4");

	fishImage = Director::getInstance()->getTextureCache()->addImage("images/fish5.png");
	FishMove5.reserve(12);
	for (int i = 0; i < 12; i++) {
		auto frame = SpriteFrame::createWithTexture(fishImage, CC_RECT_PIXELS_TO_POINTS(Rect(10, 126 * i, 160, 115)));
		FishMove5.pushBack(frame);
	}
	Animation* FishAnimation5 = Animation::createWithSpriteFrames(FishMove5, 0.2f);
	AnimationCache::getInstance()->addAnimation(FishAnimation5, "FishMove5");

	fishImage = Director::getInstance()->getTextureCache()->addImage("images/fish6.png");
	FishMove6.reserve(8);
	for (int i = 0; i < 8; i++) {
		auto frame = SpriteFrame::createWithTexture(fishImage, CC_RECT_PIXELS_TO_POINTS(Rect(0, 183 * i, 168, 180)));
		FishMove6.pushBack(frame);
	}
	Animation* FishAnimation6 = Animation::createWithSpriteFrames(FishMove6, 0.2f);
	AnimationCache::getInstance()->addAnimation(FishAnimation6, "FishMove6");

}

// 预加载音乐文件
void GameScene::preloadMusic() {
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("music/bgm.mp3");
	audio->preloadEffect("music/move.wav");
	audio->preloadEffect("music/happy.wav");
	audio->preloadEffect("music/sad.wav");

}

// 播放背景音乐
void GameScene::playBgm() {
	auto audio = SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("music/bgm.mp3", true);

}

// 添加玩家
void GameScene::addPlayer() {

	player = Sprite::create("yun.png");
	player->setScale(0.25f);
	player->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 100);

	player1 = Sprite::create("player.png");
	player1->setScale(0.15f);
	player1->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 130);

	player2 = Sprite::create("yugan.png");
	player2->setScale(0.06f);
	player2->setPosition(visibleSize.width / 2 - 50, visibleSize.height / 2 + 140);

	fish_string = Sprite::create("string.png");
	fish_string->setScale(0.25f);
	fish_string->setScaleX(0.1f);
	fish_string->setAnchorPoint(Vec2(0.5, 1.0));
	fish_string->setPosition(visibleSize.width / 2 - 75, visibleSize.height / 2 + 176);

	yugou = Sprite::create("yugou1.png");
	yugou->setScale(  0.1f);
	yugou->setAnchorPoint(Vec2(0.5, 1.0));
	auto strin = fish_string->getPosition();
	auto yscale = fish_string->getScaleY();
	int y = strin.y - fish_string->getContentSize().height * yscale;
	yugou->setPosition(Vec2(visibleSize.width / 2 - 77, y + 1));

	addChild(player1, 2);
	addChild(player2, 2);
	addChild(fish_string, 2);
	addChild(player, 2);
	addChild(yugou, 2);
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
		isMove = false;
		break;
	}

}

void GameScene::addListener() {

	addKeyBoardListener();
	addTouchListener();

}

void GameScene::addKeyBoardListener() {

	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	this->getEventDispatcher()->addEventListenerWithFixedPriority(keyboardListener, 1);

}

// 添加触摸事件监听器
void GameScene::addTouchListener() {

	EventListenerTouchOneByOne* touchlistener = EventListenerTouchOneByOne::create();
	touchlistener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	touchlistener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistener, this);

}

bool GameScene::onTouchBegan(Touch *touch, Event *event) {
	return true;
}

void GameScene::onTouchMoved(Touch *touch, Event *event) {

	auto loc = touch->getLocation();
	auto pos = yugou->getPosition();
	auto strin = fish_string->getPosition();
	auto x = visibleSize.height - loc.y;
	float num = x / 800;
	fish_string->setAnchorPoint(Point(0.5, 1.0));
	fish_string->setScaleY(num);
	auto yscale = fish_string->getScaleY();
	int y = strin.y - fish_string->getContentSize().height * yscale;
	yugou->setPosition(Vec2(strin.x - 2, y + 1));

	if (loc.y >= visibleSize.height / 2 + 80 && victimFish != NULL)
		record(victimFish);

}

void GameScene::move(char c) {

	auto position = player->getPosition();
	auto position1 = player1->getPosition();
	auto position2 = player2->getPosition();
	auto position3 = fish_string->getPosition();
	auto position4 = yugou->getPosition();
	switch (c) {

	case 'A':
		if (position.x - 5 >= 0) {
			player->setPosition(position.x - 5, position.y);
			player1->setPosition(position1.x - 5, position1.y);
			player2->setPosition(position2.x - 5, position2.y);
			fish_string->setPosition(position3.x - 5, position3.y);
			yugou->setPosition(position4.x - 5, position4.y);
		}
		break;
	case 'D':
		if (position.x + 5 <= visibleSize.width) {
			player->setPosition(position.x + 5, position.y);
			player1->setPosition(position1.x + 5, position1.y);
			player2->setPosition(position2.x + 5, position2.y);
			fish_string->setPosition(position3.x + 5, position3.y);
			yugou->setPosition(position4.x + 5, position4.y);
		}
		break;

	}

}

// 添加鱼
void GameScene::addFish() {

	/*六种鱼:
	fish1, fish2, fish3, fish4的scale设为2.0f就可以，fish5, fish6设为1.0f即可
	*/

	if (fishes.size() == 10) // 在场的鱼已经够多
		return;

	int type, difficulty;
	int x, y;
	float speed, size;

	srand((unsigned)time(0));
	type = rand() % 6;
	difficulty = rand() % 3;
	x = rand() % 2;
	y = rand() % 5 + 1;

	auto fish = Sprite::create();
	fishes.push_back(fish);

	switch (difficulty) {
	case 0:
		speed = 10.0f;
		size = 1.0f;
		fish->setTag(5);
		break;
	case 1:
		speed = 15.0f;
		size = 0.75f;
		fish->setTag(10);
		break;
	case 2:
		speed = 20.0f;
		size = 0.5f;
		fish->setTag(15);
		break;
	}

	// 设置初始位置并给鱼添加移动
	if (x == 0) {
		fish->setPosition(Vec2(0, y * 50 + 40));
		auto moveTo = MoveTo::create(speed, Vec2(visibleSize.width + 50, y * 50 + 40));
		fish->runAction(moveTo);
	}
	else {
		fish->setPosition(Vec2(visibleSize.width, y * 50 + 40));
		// 转向
		fish->setFlipX(true);
		auto moveTo = MoveTo::create(speed, Vec2(-50, y * 50 + 40));
		fish->runAction(moveTo);
	}


	if (type == 0) {
		auto animation = RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("FishMove1")));
		fish->runAction(animation);
	}
	else if (type == 1) {
		auto animation = RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("FishMove2")));
		fish->runAction(animation);
		fish->setTag(-5);
	}
	else if (type == 2) {
		auto animation = RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("FishMove3")));
		fish->runAction(animation);
	}
	else if (type == 3) {
		auto animation = RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("FishMove4")));
		fish->runAction(animation);
	}
	else if (type == 4) {
		auto animation = RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("FishMove5")));
		fish->runAction(animation);
	}
	else {
		auto animation = RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("FishMove6")));
		fish->runAction(animation);
	}

	if (type == 4 || type == 5) {
		fish->setScale(0.8f);
	}
	else {
		fish->setScale(size);
	}

	addChild(fish, 2);

}

void GameScene::guardFishes() {

	for (list<Sprite*>::iterator iter = fishes.begin(); iter != fishes.end(); iter++) {

		// 对每条鱼监听，如果游出界面则反向游
		auto position = (*iter)->getPosition();
		if (position.x >= visibleSize.width + 50) {
			(*iter)->setFlippedX(true);
			auto moveTo = MoveTo::create(10, Vec2(-50, position.y));
			(*iter)->runAction(moveTo);
		}
		else if (position.x <= -50) {
			(*iter)->setFlippedX(false);
			auto moveTo = MoveTo::create(10, Vec2(visibleSize.width + 50, position.y));
			(*iter)->runAction(moveTo);
		}

		Rect stringRect = fish_string->getBoundingBox();
		Rect fishRect = (*iter)->getBoundingBox();
		if (stringRect.intersectsRect(fishRect) && (*iter)->getTag() == -5) { // 是水母!
			record(*iter);
			fishes.remove((*iter));
			sad();
			break;
		}

		// 是否和钩子接触，接触则判定为被钓起来了
		if (victimFish == NULL) {
			if (yugou->getPosition().getDistance((*iter)->getPosition()) < 25) { // 被钓起来了
				if ((*iter)->getTag() == -5) {
					record(*iter);
					sad();
				}
				else {
					victimFish = (*iter);
				}
				fishes.remove((*iter));
				break;
			}
		}
	}

}

// 记录钓起来的鱼的分数(已经确定鱼钩有鱼)
void GameScene::record(Sprite* fish) {

	score += fish->getTag();
	if (fish->getTag() != -5) {
		victimFish = NULL;
		laugh();
	}
	fish->removeFromParentAndCleanup(true);

}

void GameScene::laugh() {

	// 切换成笑的图片
	player1->setTexture("images/laugh.png");
	player1->setScale(0.35f);

	// 播放音效
	// TODO
	SimpleAudioEngine::getInstance()->playEffect("music/happy.wav", false);

	// 启动自定义调度器，在合适的时候把图片换回来
	schedule(schedule_selector(GameScene::removeMood), 2.0f, kRepeatForever, 0);

}

void GameScene::removeMood(float dt) {

	player1->setTexture("player.png");
	player1->setScale(0.15f);

}

void GameScene::sad() {

	player1->setTexture("images/sad.png");

	// 播放音效
	// TODO
	SimpleAudioEngine::getInstance()->playEffect("music/sad.wav", false);

	// 启动自定义调度器，在合适的时候把图片换回来
	schedule(schedule_selector(GameScene::removeMood), 1.0f, kRepeatForever, 0);

	score -= 5;

}

void GameScene::clocker(float dt) {
	submit();
	this->removeChild(timeclocker, 1);
	this->removeChild(timetext, 1);
	char s[12];
	sprintf(s, "%d", dtime);
	std::string str = s;
	timeclocker = Label::createWithTTF(str, "fonts/mango.ttf", 36);
	timeclocker->setPosition(Vec2(visibleSize.width / 5, visibleSize.height - 50));
	this->addChild(timeclocker, 1);
	timetext = Label::createWithTTF("Time: ", "fonts/mango.ttf", 36);
	timetext->setPosition(Vec2(visibleSize.width / 5 - 50, visibleSize.height - 50));
	this->addChild(timetext, 1);
	if (dtime == 0) rank();
	else dtime -= 1;
}

void GameScene::gameover() {
	auto gameover = Sprite::create("gameOver.png");
	gameover->setPosition(visibleSize.width / 4, visibleSize.height / 2);
	gameover->setScale(0.1f);
	this->addChild(gameover, 1);

	auto button = Button::create();
	auto playagain = Label::createWithTTF("Play again", "fonts/mango.ttf", 36);
	playagain->setPosition(Size(visibleSize.width / 5, visibleSize.height / 5));
	button->setTitleLabel(playagain);
	button->addClickEventListener(CC_CALLBACK_0(GameScene::playagain, this));
	button->setPosition(Size(visibleSize.width / 5, visibleSize.height / 5));
	this->addChild(button, 2);

	auto button2 = Button::create();
	auto exit = Label::createWithTTF("Exit", "fonts/mango.ttf", 36);
	exit->setPosition(Size(visibleSize.width / 5, visibleSize.height / 5 - 50));
	button2->setTitleLabel(exit);
	button2->addClickEventListener(CC_CALLBACK_0(GameScene::exit, this));
	button2->setPosition(Size(visibleSize.width / 5, visibleSize.height / 5 - 50));
	this->addChild(button2, 2);

	unschedule(schedule_selector(GameScene::update));
	unschedule(schedule_selector(GameScene::clocker));
}

void GameScene::submit() {
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8080/submit");
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(GameScene::onHttpRequestCompleted, this));

	//setHeader
	std::vector<std::string> headers;
	headers.push_back("Cookie: GAMESESSIONID=" + Global::gameSessionId);
	request->setHeaders(headers);//将sessionID加入到http请求的头部
	request->setTag("POST submit");//设置http请求的tag

								   // write the post data 
	char s[12];
	sprintf(s, "%d", score);
	std::string strr = s;
	string str = "score=" + strr;
	const char* postData = str.c_str();
	request->setRequestData(postData, strlen(postData));

	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void GameScene::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response) {
	if (!response)
	{
		return;
	}
	int statusCode = response->getResponseCode();
	log("response code: %d", statusCode);
	if (!response->isSucceed())
	{
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}
	std::vector<char> *buffer = response->getResponseData();
	for (unsigned int i = 0; i < buffer->size(); i++) {
		printf("%c", (*buffer)[i]);
	}
	printf("\n");
}

void GameScene::rank() {
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8080/rank?top=5");
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(GameScene::onHttpRequestCompletedrank, this));

	//setHeader
	std::vector<std::string> headers;
	headers.push_back("Cookie: GAMESESSIONID=" + Global::gameSessionId);
	request->setHeaders(headers);//将sessionID加入到http请求的头部
	request->setTag("GET rank");//设置http请求的tag

	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
	gameover();
}

void GameScene::onHttpRequestCompletedrank(HttpClient *sender, HttpResponse *response) {
	std::vector<char> *databuffer = response->getResponseData();
	string rank = Global::toString(databuffer);
	rapidjson::Document d;
	d.Parse<0>(rank.c_str());
	if (d.HasParseError()) {
		rank_field->setText("Error\n");
	}
	if (d.IsObject() && d.HasMember("info")) {
		string temp = d["info"].GetString();
		string member = "";
		string allmember = "Rank: \n";
		for (int i = 1; i < temp.length(); i++) {
			if (temp[i] != '|') {
				member += temp[i];
			}
			else {
				allmember += member;
				allmember += '\n';
				member = "";
			}
		}
		rank_field->setText(allmember);
	}
}

void GameScene::playagain() {
	auto scene = MenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
}

void GameScene::exit() {
	_eventDispatcher->removeAllEventListeners();
	Director::getInstance()->end();
}

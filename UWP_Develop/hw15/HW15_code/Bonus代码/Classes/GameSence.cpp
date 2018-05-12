#include "GameSence.h"
#include <stdlib.h>
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "ui/CocosGUI.h"
#include "json/stringbuffer.h"
#include <regex>
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;
using namespace cocos2d::ui;

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
	counter = 0;
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

	//
	rank_field = TextField::create("", "Arial", 30);
	rank_field->setPosition(Size(visibleSize.width / 4 * 3, visibleSize.height / 4 * 3.5));
	this->addChild(rank_field, 2);

	rank_button = Button::create();
	rank_button->setTitleText("Rank");
	rank_button->setTitleFontSize(30);
	rank_button->setPosition(Size(visibleSize.width / 4 * 3, visibleSize.height / 4));
	rank_button->addClickEventListener(CC_CALLBACK_0(GameSence::rank, this));
	this->addChild(rank_button, 1);

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
		++counter;
		auto cheese = Sprite::create("cheese.png");
		cheese->setPosition(location);
		auto fadeout = FadeOut::create(1.0f);
		auto moveto = MoveTo::create(0.5, location);
		auto seq = Sequence::create(moveto, fadeout, nullptr);
		cheese->runAction(seq);
		this->addChild(cheese, 1);
		auto movet = MoveTo::create(0.5, location);
		mouse->runAction(movet);
		HttpRequest* request = new HttpRequest();
		request->setUrl("http://localhost:8080/submit");
		request->setRequestType(HttpRequest::Type::POST);
		request->setResponseCallback(CC_CALLBACK_2(GameSence::onHttpRequestCompleted, this));

		//setHeader
		std::vector<std::string> headers;
		headers.push_back("Cookie: GAMESESSIONID=" + Global::gameSessionId);
		request->setHeaders(headers);
		request->setTag("POST submit");
		char strr[50];
		snprintf(strr, sizeof(strr), "%d", counter);
		string strcounter;
		for (int i = 0; i < 50; i++) {
			strcounter += strr[i];
			if (strr[i] == '\0') break;
		}
		string str = "score=" + strcounter;
		const char* postData = str.c_str();
		request->setRequestData(postData, strlen(postData));
		cocos2d::network::HttpClient::getInstance()->send(request);
		request->release();
	}
	return true;
}

void GameSence::rank() {
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8080/rank?top=3");
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(GameSence::onHttpRequestCompletedrank, this));

	//setHeader
	std::vector<std::string> headers;
	headers.push_back("Cookie: GAMESESSIONID=" + Global::gameSessionId);
	request->setHeaders(headers);
	request->setTag("GET rank");

	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void GameSence::onHttpRequestCompletedrank(HttpClient *sender, HttpResponse *response) {
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
		string allmember = "";
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

void GameSence::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response) {
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


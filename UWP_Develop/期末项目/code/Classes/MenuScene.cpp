#include "MenuScene.h"
#include "GameScene.h"
#include "cocostudio/CocoStudio.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "SimpleAudioEngine.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "Global.h"
#include "GameScene.h"
#include <regex>
using std::to_string;
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;
using namespace CocosDenshion;

USING_NS_CC;

Scene* MenuScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MenuScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{

	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//添加背景
	auto bg = Sprite::create("star.png");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y ));
	this->addChild(bg, 0);

	preloadMusic();
	playBgm();

	//添加粒子系统
	auto paopao = ParticleSystemQuad::create("particle_texture2.plist");
	paopao->setPositionType(ParticleSystemQuad::PositionType::RELATIVE);
	paopao->setPosition(visibleSize.width / 2, visibleSize.height / 2 );
	paopao->setScale(0.6f);
	paopao->setScaleX(1.0f);
	this->addChild(paopao,1);

	// login
	loginField = TextField::create("Player Name", "fonts/mango.ttf", 30);
	loginField->setPosition(Size(visibleSize.width / 2, visibleSize.height / 3));
	this->addChild(loginField, 2);

	// login button
	auto button = Button::create();
	auto logintext = Label::createWithTTF("Login", "fonts/mango.ttf", 36);
	logintext->setPosition(Size(visibleSize.width / 2, visibleSize.height / 4));
	button->setTitleLabel(logintext);
	button->addClickEventListener(CC_CALLBACK_0(MenuScene::login, this));
	button->setPosition(Size(visibleSize.width / 2, visibleSize.height / 4  ));
	this->addChild(button, 2);
	
	//  游戏规则
	auto button1 = Button::create();
	auto howto = Label::createWithTTF("How To Play", "fonts/mango.ttf", 30);
	howto->setPosition(Size(visibleSize.width / 4 , visibleSize.height / 5 * 4));
	button1->setTitleLabel(howto);
	button1->addClickEventListener(CC_CALLBACK_0(MenuScene::howtoplay, this));
	button1->setPosition(Size(visibleSize.width / 4, visibleSize.height / 5 * 4));
	this->addChild(button1, 2);

	return true;
}

void MenuScene::howtoplay() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	string str = "                              1 Don't touch the jellyfish, or your scores will be deducted \n\n\
                              2 Press the left and right keys (or A, D keys) to move,so that you can catch fish as soon as possible\n\n\
                              3 You will have 50 seconds every time, the game will show top5 in the end\n\n\
                              4 Remeber to login with your name, or you won't join in the rank in the end\n\n";
	auto playField = Label::createWithTTF(str, "fonts/mango.ttf", 20);
	playField->setPosition(Size(visibleSize.width / 3, visibleSize.height / 5 * 3 - 50));
	this->addChild(playField, 2);
}

// 预加载音乐文件
void MenuScene::preloadMusic() {
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("music/river.mp3");
	audio->preloadEffect("music/move.wav");

}

// 播放背景音乐
void MenuScene::playBgm() {
	auto audio = SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("music/river.mp3", true);

}

void MenuScene::login() {
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8080/login");
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(MenuScene::onHttpRequestCompleted, this));

	//写入数据
	string str = "username=" + loginField->getString();
	const char* postData = str.c_str();
	request->setRequestData(postData, strlen(postData));
	request->setTag("POST login");

	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();

	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(2, scene));

}

void MenuScene::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
	if (!response)
	{
		return;
	}
	if (!response->isSucceed())
	{
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}
	// dump data 
	std::vector<char> *databuffer = response->getResponseData();
	std::vector<char> *headerbuffer = response->getResponseHeader();
	string datatemp = Global::toString(databuffer);
	string headertemp = Global::toString(headerbuffer);
	Global::gameSessionId = Global::getSessionIdFromHeader(headertemp);
}


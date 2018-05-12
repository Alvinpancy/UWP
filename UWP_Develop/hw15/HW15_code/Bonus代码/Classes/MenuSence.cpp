#include "MenuSence.h"
#include "GameSence.h"
#include "cocostudio/CocoStudio.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "Global.h"
#include <regex>
USING_NS_CC;

Scene* MenuSence::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = MenuSence::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MenuSence::init()
{

    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg_sky = Sprite::create("menu-background-sky.jpg");
	bg_sky->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 150));
	this->addChild(bg_sky, 0);

	auto title = Sprite::create("gold-miner-text.png");
	title->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 200));
	this->addChild(title, 1);

	auto bg = Sprite::create("menu-background.png");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 60));
	this->addChild(bg, 0);

	auto miner = Sprite::create("menu-miner.png");
	miner->setPosition(Vec2(150 + origin.x, visibleSize.height / 2 + origin.y - 60));
	this->addChild(miner, 1);

	auto leg = Sprite::createWithSpriteFrameName("miner-leg-0.png");
	Animate* legAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("legAnimation"));
	leg->runAction(RepeatForever::create(legAnimate));
	leg->setPosition(110 + origin.x, origin.y + 102);
	this->addChild(leg, 1);

	auto starMenuItem = MenuItemImage::create(
		"start-0.png",
		"start-1.png", CC_CALLBACK_1(MenuSence::startMenuCallback, this));
	starMenuItem->setPosition(Vec2(origin.x + visibleSize.width - starMenuItem->getContentSize().width, 200));
	auto starMenu = Menu::create(starMenuItem, NULL);
	starMenu->setPosition(Vec2::ZERO);
	this->addChild(starMenu, 1);

	auto gold = Sprite::create("menu-start-gold.png");
	gold->setPosition(Vec2(origin.x + visibleSize.width - starMenuItem->getContentSize().width, 150));
	this->addChild(gold, 0);

	textField = TextField::create("Player name", "Arial", 30);
	textField->setPosition(Size(visibleSize.width / 2, visibleSize.height / 4 * 2));
	this->addChild(textField, 2);

    return true;
}

void MenuSence::startMenuCallback(cocos2d::Ref* pSender) {
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8080/login");
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(MenuSence::onHttpRequestCompleted, this));

	//聽write聽the聽post聽data聽
	string str = "username=" + textField->getString();
	const char* postData = str.c_str();
	request->setRequestData(postData, strlen(postData));
	request->setTag("POST login");

	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
		auto scene = GameSence::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
}

void MenuSence::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response) {
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
	std::vector<char> *databuffer = response->getResponseData();
	std::vector<char> *headerbuffer = response->getResponseHeader();
	string datatemp = Global::toString(databuffer);
	string headertemp = Global::toString(headerbuffer);
	Global::gameSessionId = Global::getSessionIdFromHeader(headertemp);
}



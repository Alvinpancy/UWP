#include "GameScene.h"
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

cocos2d::Scene* GameScene::createScene() {
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
    if (!Layer::init())
    {
        return false;
    }

    Size size = Director::getInstance()->getVisibleSize();
    visibleHeight = size.height;
    visibleWidth = size.width;

    score_field = TextField::create("Score", "Arial", 30);
    score_field->setPosition(Size(visibleWidth / 4, visibleHeight / 4 * 3));
    this->addChild(score_field, 2);

    submit_button = Button::create();
    submit_button->setTitleText("Submit");
    submit_button->setTitleFontSize(30);
    submit_button->setPosition(Size(visibleWidth / 4, visibleHeight / 4));
	submit_button->addClickEventListener(CC_CALLBACK_0(GameScene::submit, this));
    this->addChild(submit_button, 2);

    rank_field = TextField::create("", "Arial", 30);
    rank_field->setPosition(Size(visibleWidth / 4 * 3, visibleHeight / 4 * 3));
    this->addChild(rank_field, 2);

    rank_button = Button::create();
    rank_button->setTitleText("Rank");
    rank_button->setTitleFontSize(30);
    rank_button->setPosition(Size(visibleWidth / 4 * 3, visibleHeight / 4));
	rank_button->addClickEventListener(CC_CALLBACK_0(GameScene::rank, this));
    this->addChild(rank_button, 2);

    return true;
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
	string str = "score=" + score_field->getString();
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

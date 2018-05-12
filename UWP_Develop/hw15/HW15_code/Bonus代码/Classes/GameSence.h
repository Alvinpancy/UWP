#ifndef __GAME_SENCE_H__
#define __GAME_SENCE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>
#include "network/HttpClient.h"
#include "Global.h"
USING_NS_CC;
using namespace cocos2d::network;
using namespace cocos2d::ui;
using std::string;

class GameSence : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	void rank();
	void onHttpRequestCompletedrank(HttpClient *sender, HttpResponse *response);
	void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
	//virtual void shootMenuCallback(Ref* pSender);

	CREATE_FUNC(GameSence);

private:
	Sprite* mouse;
	Sprite* stone;
	Layer* mouseLayer;
	TextField* rank_field;
	Button* rank_button;
	Layer* stoneLayer;
	int counter;
};

#endif // __GAME_SENCE_H__


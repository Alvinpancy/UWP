#ifndef __MENU_SEBCE_H__
#define __MENU_SEBCE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "network/HttpClient.h"
#include <string>
#include <vector>
using std::vector;
using std::string;
using namespace cocos2d::ui;
using namespace cocos2d::network;

class MenuScene : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();
	void login();
	void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
	CREATE_FUNC(MenuScene);

private:
	TextField * loginField;
};

#endif // __MENU_SEBCE_H__

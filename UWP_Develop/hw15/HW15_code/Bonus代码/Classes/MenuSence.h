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

class MenuSence : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
    // a selector callback
    void startMenuCallback(cocos2d::Ref* pSender);
	void login();
	
    // implement the "static create()" method manually
    CREATE_FUNC(MenuSence);

private:
	TextField * textField;
};

#endif // __MENU_SEBCE_H__

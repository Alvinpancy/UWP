#ifndef __GAME_SEBCE_H__
#define __GAME_SEBCE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "Global.h"
#include "network/HttpClient.h"
using namespace cocos2d::network;
using namespace cocos2d::ui;

USING_NS_CC;
using namespace std;

class GameScene : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();

	const int MaxNumber = 10;

	virtual bool init();

	void submit();

	void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);

	void rank();

	void onHttpRequestCompletedrank(HttpClient *sender, HttpResponse *response);

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

private:

	// load the ParticleSystem
	void loadParticle();

	// preload the music
	void preloadMusic();

	// play the Bgm
	void playBgm();

	// add the player;
	void addPlayer();

	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);

	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);

	void onTouchMoved(Touch *touch, Event *event);

	bool onTouchBegan(Touch *touch, Event * event);

	void update(float f);

	void clocker(float f);

	void move(char c);

	// ��¼����������ķ���
	void record(Sprite*);

	void addListener();

	void addTouchListener();

	// ��Ӽ���
	void addKeyBoardListener();

	// ���ض���
	void loadAnimation();

	// �����
	void addFish();

	// ������
	void guardFishes();

	// ���౦���������㣬ЦһЦ
	void laugh();

	void removeMood(float dt);

	void sad();

	void gameover();

	void playagain();

	void exit();

	Size visibleSize;

	bool isMove;

	int score;

	int dtime;

	char movekey;

	Label* timeclocker;

	Label* timetext;

	Label* scoretext;

	Label* scoreshow;

	TextField * rank_field;

	Sprite* player;

	list<Sprite*> fishes; // �����ε���

	Sprite* victimFish; // �㹳�ϵ���

	Sprite* player1;

	Sprite* player2;

	Sprite* fish_string;

	Sprite* yugou;

};

#endif // __GAME_SEBCE_H__

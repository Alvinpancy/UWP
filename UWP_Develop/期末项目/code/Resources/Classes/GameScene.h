#ifndef __GAME_SEBCE_H__
#define __GAME_SEBCE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace std;
using namespace cocos2d;

class GameScene : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();

	virtual bool init();



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

	void movefish();

	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);

	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);

	void onTouchMoved(Touch *touch, Event *event);

	bool onTouchBegan(Touch *touch, Event *event);

	void update(float f);

	void move(char c);

	void addListener();

	void addTouchListener();

	// Ìí¼Ó¼àÌý
	void addKeyBoardListener();

	Size visibleSize;

	bool isMove;

	char movekey;

	Sprite* player;

	Sprite* player1;

	Sprite* player2;

	Sprite* fish_string;

	Sprite* yugou;


	cocos2d::Vector<SpriteFrame*> fish7vector;
};

#endif // __GAME_SEBCE_H__
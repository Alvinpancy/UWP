#ifndef __FRIEND_SHIP_H__
#define __FRIEND_SHIP_H__

#include "cocos2d.h"
using namespace std;
USING_NS_CC;
class FriendShip : public Layer {
public:
  static PhysicsWorld* world;
  static cocos2d::Scene* createScene();
  void setPhysicsWorld(PhysicsWorld * world);
  virtual bool init();

  void addSprite();   // ��ӱ����͸��־���
  void addPlayer();   // ������
  void addListener(); // ��Ӽ�����

  void update(float dt);
  void updateShip(float dt);  // ���´���ƽ�����
  void boxFall(float dt);  // ��������

  void onKeyPressed(EventKeyboard::KeyCode code, Event * event);
  void onKeyReleased(EventKeyboard::KeyCode code, Event * event);
  bool onConcactBegin(PhysicsContact & contact);

  void preloadMusic();                   // Ԥ��������
  void GameOver();                      // ��Ϸ����
  void loadAnimation(string filepath); // ���ض���

  // ������˳���ť��Ӧ����
  void replayCallback(Ref * pSender);
  void exitCallback(Ref * pSender);

  // implement the "static create()" method manually
  CREATE_FUNC(FriendShip);

private:
  PhysicsWorld* m_world;
  Size visibleSize;

  Sprite* ship;
  list<PhysicsBody*> boxes;

  Node* edgeNode;
  Node* holdbox1;
  Node* holdbox2;

  Sprite* player1;
  Sprite* player2;

  SpriteFrame* frame1;
  SpriteFrame* frame2;
  SpriteFrame* IdleWithBox1;
  SpriteFrame* IdleWithBox2;

  // �̶�����ؽ�, ���ھ�������
  PhysicsJointDistance * joint1;

  char LastPlayer1Press;

  bool IsPlayer1Hold;
  bool IsPlayer1Jump;
  bool IsPlayer1Left;
  bool IsPlayer1Right;

  double deltaH;  // ÿ�θ��µĸ߶ȱ仯��
  double height;  // ���Ҹ߶Ȳ�
  int LastTag;
  int counter;
};

#endif // __FRIEND_SHIP_H__

#pragma once
#include "CObject.h"
#include "d3dUtility.h"

/*
player Ŭ������ ���� ������
�� ���� �ȸ���� ������ ������ �Ǵ���
�����Ǵ��� �÷��̾ �ϴ°��� �����ϴ°� �ƴϴ�
���� ���⼭ ������ �߻��Ѱ� virtual lego���� ���� ������ ������ red red yellow whit�� �ϴµ� �ű⼭ ������ ���� �κ��������� ����
red1 red �̷������� �������� Ȥ�� ��ģ�� ������ Ȯ�� �ٶ� 
�� ������ �����ϸ�
1.������� ��Ȳ�� �����ĳ��ϱ����Ͽ�
2.������ ���� ���� ������ red1 red ������ ������ ���� ������ �Ǵ��ϱ� ���Ͽ� 
3. ���ϰ� �÷��̳ʴ� �ٸ���
 4. �÷��̾�� ���ǼӼ��� �ο��ϰ� �� 
*/
class CPlayer  {
public:
	CPlayer();

	//CPlayer(char* name);																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																													
	CPlayer(std::string name);
	CPlayer(std::string name, int goalScore);

//	CPlayer(char* name,int goalScore);
	~CPlayer();

	void setHitBall(std::string ballname);
	void setHitBall(std::string ball1 ,std::string ball2); 

	void decideScore(); //decide Score + 0 - ? 
	void create(std::string name);
	inline bool changeTurn() {		return !(myTurn);	};
	inline int getScore() {		return myScore; 	}; //
	inline void setMyball(std::string color) {		this->myBall = color;	}
	inline bool getMyTurn() {		return this->myTurn;	}
	inline void resetHit() {
		hitRed1 = false;
		hitRed2 = false;
		hitball = false;

	}
private:

	//char* myBall;

	std::string myBall;

	int myScore;
	int goalScore; //��ǥ����  �� goal Score ������ �ϸ� �ȵ� �ѹ� setting�Ǹ�

	bool hitRed1;
	bool hitRed2;
	bool hitball;
	//bool hitWall; //������� ���� ?
	bool myTurn;

};
/*

hitBy()

//�˻�
if ((this->name == 'y' && ball.name == 'w') || (this->name == 'y' && ball.name == 'w')) this->failFlag = true; // ����� �����, ������� ����� �ε����� ��� ����
else if (this->name == 'y' || this->name == 'w'){ // �ƴ� ��� �� ���� ���� ���� �ε����� ��
if (ball.name = 'r') // ù��° ������(r)�϶�
switch (ball.redAttack){
case 0: ball.redAttack = 1;
break;
case 2: ball.redAttack = 3;
ball.sucFlag = true;
break;
}
else if (ball.name = 's') // �ι�° ������(s)�϶�
switch (ball.redAttack){
case 0: ball.redAttack = 2;
break;
case 1: ball.redAttack = 3;
ball.sucFlag = true;
break;
}

}

*virtualLego.cpp

//����Ȯ���ϴ� �Լ� �߰�

void scoring(CSphere ball){

if (ball.name == 'w')
{ // ����� ���̸�
if (!(ball.failFlag && ball.sucFlag)); // ������ ������ �ƴϸ� ����
else if (ball.failFlag) ball.score += 10;// �����ϰ�� ������ ����
else ball.score -= 10; // �����ϰ�� ����

ball.initializeScore(); //���� ��� ���߰� ������ ���� �����̹Ƿ� �ش� ���� ����(���� ģ ����)�� �ʱ�ȭ

}
else
{ // ������� ���̸�
if (!(ball.failFlag && ball.sucFlag)); // ������ ������ �ƴϸ� ����
else if (ball.failFlag) ball.score += 10; // �����ϰ�� ������ ����
else ball.score -= 10;// �����ϰ�� ����

ball.initializeScore();//���� ��� ���߰� ������ ���� �����̹Ƿ� �ش� ���� ����(���� ģ ����)�� �ʱ�ȭ
}

*/
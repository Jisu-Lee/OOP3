#include "CPlayer.h"
#include <d3d9.h>

//���� hit by override �� ��ġ�� ���ϰ� player�ϰ� ��ĥ�������Ͱ���
// �����丵�ϴ� ������ ����� ���� ^^ 
//�ڡڡڡڡ��ڷ��� �����丵 * char* �� ��Ʈ������ �ٲپ��� ^^ �ȳ� !! �ڡڡڡڡڡڡ�
//���� ��������� ���� �� ���� ���� �籸 ������� ���� �˱�δ� �ڱ� ��Ÿ? ���� ģ���ϸ�
//�������� �����ϴ°ɷξ˰� ��ǥ���� (�� �ľ��� �������� + ) 
// goal ������ ����°͵���������
//��� ��ǥ���� �̻�Ѿ�� �����Ǵ°Ŵϱ� 
//��ǥ ���� 
//�� decide win �ϴ� �Լ� ? 
//decide win..? decideWinne 

CPlayer::CPlayer() {

	myBall = "NULL";
	myScore = 0;
	goalScore = 0;
	hitRed1 = false;
	hitRed2 = false;
	hitball = false;
	//bool hitWall; //������� ���� ?
	myTurn = true;
	
}

CPlayer::CPlayer(std::string name) {
	myBall = name;
	//this->myBall = name;
	myScore = 0;
	goalScore = 100;
	hitRed1 = false;
	hitRed2 = false;
	hitball = false; //���� �ƴ��� 
					 //bool hitWall; //������� ���� ?
	myTurn = true;
}

CPlayer::CPlayer(std::string name, int goalScore) {
	myBall = name;
	//this->myBall = name;
	myScore = 0;
	goalScore = goalScore;
	hitRed1 = false;
	hitRed2 = false;
	hitball = false; //���� �ƴ��� 
	//bool hitWall; //������� ���� ?
	myTurn = true;
	}

CPlayer::~CPlayer() {}


void CPlayer::setHitBall(std::string ballname) {
	if (ballname.compare("Red1"))
		this->hitRed1 = true;
	else if (ballname.compare("Red2"))
		this->hitRed2 = true;
	/*else if (this -> ballname.compare(ballname))
		return;
	*/
	else this->hitball = true;
}

void CPlayer::setHitBall(std::string ball1, std::string ball2) {
	this->setHitBall(ball1);
	this->setHitBall(ball2);

}
void CPlayer::decideScore() {
	myTurn = false;
	if (hitball || (!(hitRed1) && !(hitRed2)) ) //��Ÿ 
		myScore -= 10; //�켱 10�� ���� �� 
	else if (hitRed1 && hitRed2) {
		myScore += 10;
		myTurn = true;
	}

	resetHit();
}


void CPlayer::create(std::string name) {
	this->myBall = name;
}
#pragma once
#include "CObject.h"
#include <d3dx9.h>

/*
Refactoring
�� Ŭ������ ���� ����.
cue, light, wall, sphere���� ���������� �� ����Լ��� ������ �ִ�.
virtual:

destroy
->destroys ipd3dxmesh

get/set position(float x, y, z)

variables:
d3dxmatrix m_mLocal;
Ipd3dxmesh

wall, sphere�� objectŬ������ ����ϰ� �����Ƿ� object�� �� ���ο� Ŭ������ ���/������ ����

light�� �� �� Ŭ������ �ٷ� �����.

�ڵ� ���� �� �ٰ� ����

object Ŭ���� �̸��� primitive�� ������ ����

object Ŭ������ �������̽��� create(sphere�� ���� parameter�� d3dxcolor�� �پ������� ���� setcolor�� �����°� ������)

�ζ��� �Լ��δ� get/setLocalTransform, 

object Ŭ�������� hitby, hasIntersected �޼ҵ� �̾Ƴ��� �̰͵� ���� �������̽� Ŭ����(ICollidable)�� ���� ����.
�׷��� Collision �κ��� �� �𸣰ھ ������߰ڴ�..
�������̽��� �̾Ƴ��� �籸�ǿ� Ȧ���� �浹�ϴ� ��ü�� ���ܵ� �����ϰ� �ڵ� Ȯ���� ������ �� ������..

new object(primitive) class{
	
	destroy
	->calls base destroy and destroy m_effect, shadercode, texture

	pure virtual draw(device, world, view)

	get/set local transform

	pure virutal create(pdevice)

	private:

	load shader(device, fileName)
	load texture(device, fileName)
	struct vertex{};

	variable:
	shaderCode
	effect
	texture

}
new ICollidable class{
	pure virtual hitby(Icollidable)
	pure virtual HitProcess()<-�ڽ� Ŭ�������� private���� public�� hitby ���� ó���� ���δ� �޼ҵ�
	pure virtual hasIntersected(Icollidable)
	pure virtual intersectectedProcess()<-hitProcess�� ����

}


*/

class CCue : public CObject{
public:
	CCue();
	~CCue();
	bool create(IDirect3DDevice9* pDevice);//will be moved to parent class
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld, const D3DXMATRIX& mView, float time);
	const inline D3DXMATRIX& getLocalTransform() const{ return m_mLocal; }//will be moved to parent class
	const inline D3DXVECTOR3 getPosition()const{//will be moved to parent class
		return D3DXVECTOR3(m_x, m_y, m_z);
	}
	void playHit();
	bool IsAnimationEnded();
private:
	void playAnimation(float time);
	bool animationInit;
	float moveDistance;
	bool pulled;
	bool animationEnded;
	LPD3DXMESH convertMesh(IDirect3DDevice9* pDevice, LPD3DXMESH& mesh) override;
	//to be overrided






};
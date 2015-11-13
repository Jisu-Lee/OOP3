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
	bool create(IDirect3DDevice9* pDevice, CObject::Shape shape);//will be moved to parent class
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld, const D3DXMATRIX& mView) override;
	const inline D3DXMATRIX& getLocalTransform() const{ return m_mLocal; }//will be moved to parent class
	const inline D3DXVECTOR3 getPosition()const{//will be moved to parent class
		return D3DXVECTOR3(m_x, m_y, m_z);
	}
	void playHit();
private:

	LPD3DXMESH convertMesh(IDirect3DDevice9* pDevice, const LPD3DXMESH& input);//to be overrided
	LPD3DXEFFECT LoadShader(IDirect3DDevice9* pDevice, const char* fileName);//will be moved to parent class
	LPDIRECT3DTEXTURE9 LoadTexture(IDirect3DDevice9* pDevice, const char* fileName);//will be moved to parent class





};
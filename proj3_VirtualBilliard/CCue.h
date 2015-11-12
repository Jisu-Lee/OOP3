#pragma once
#include <d3dx9.h>
#define FILENAME "filename"

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

class CCue{
public:
	CCue();
	~CCue();
	bool create(IDirect3DDevice9* pDevice);
	void setPosition(float x, float y, float z);
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld, const D3DXMATRIX& mView);
	void destroy();
	void inline setLocalTransform(const D3DXMATRIX& mLocal){ m_mLocal = mLocal; }
	const inline D3DXMATRIX& getLocalTransform() const{ return m_mLocal; }
private:
#define FVF_VERTEX D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1
	struct VERTEX
	{
		D3DXVECTOR3 pos;     // vertex position
		D3DXVECTOR3 norm;    // vertex normal
		float tu;            // texture coordinates
		float tv;
	};

	//LPD3DXMESH createMesh(IDirect3DDevice9* pDevice);
	//LPD3DXEFFECT LoadShader(IDirect3DDevice9* pDevice, const char* fileName);
	//LPDIRECT3DTEXTURE9 LoadTexture(IDirect3DDevice9* pDevice, const char* fileName);
//	LPD3DXMESH createMesh(IDirect3DDevice9* pDevice, float rad, UINT slices, UINT stacks);

	float			m_x;
	float			m_y;
	float			m_z;
	LPD3DXMESH		m_pMesh;
	D3DXMATRIX		m_mLocal;
	ID3DXEffect*	m_effect;
	LPD3DXBUFFER	m_shaderCode;
	LPDIRECT3DTEXTURE9		m_texture;

};
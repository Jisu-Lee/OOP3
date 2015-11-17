#pragma once
#include "CObject.h"
#include "ICollidable.h"
#include "d3dUtility.h"
class CWall : public CObject, public ICollidable{
public:
	CWall();
	~CWall();
	enum Type{
		Edge,
		Plane
	};
	bool create(IDirect3DDevice9* pDevice,
				float iwidth,
				float iheight,
				float idepth,
				Type type);

	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld,
		const D3DXMATRIX& mView,
		const D3DXVECTOR4& camPos);
	void destroy() override;
	bool hasIntersected(CSphere& ball) override;
	bool hitBy(CSphere& ball) override;
	const D3DXVECTOR3 inline getPosition(){ return D3DXVECTOR3(m_x, m_y, m_z); }
	inline float getHeight() const { return M_HEIGHT; }

private:
	using CObject::setLocalTransform;
	float                   m_width;
	float                   m_depth;
	float					m_height;
	LPDIRECT3DTEXTURE9		m_specular;
	LPDIRECT3DTEXTURE9		m_normal;
	char*					m_specularFile;
	char*					m_normalFile;
	LPD3DXMESH convertMesh(IDirect3DDevice9* pDevice, LPD3DXMESH& input) override;

};
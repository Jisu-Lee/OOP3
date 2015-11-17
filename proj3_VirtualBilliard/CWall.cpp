#include "CWall.h"
#include "CSphere.h"
#include <d3dx9math.h>
#include "ConstVariable.h"
CWall::CWall(){
	D3DXMatrixIdentity(&m_mLocal);
	m_width = 0;
	m_depth = 0;
	m_pMesh = NULL;
}
CWall::~CWall(){}

bool CWall::create(IDirect3DDevice9* pDevice,
	float iwidth,
	float iheight,
	float idepth,
	Type type){
	m_width = iwidth;
	m_depth = idepth;
	m_height = iheight;
	if (FAILED(D3DXCreateBox(pDevice, iwidth, iheight, idepth, &m_pMesh, 0))) return false;

	LPD3DXMESH newMesh = convertMesh(pDevice, m_pMesh);
	if (newMesh == nullptr){
		m_pMesh->Release();
		return false;
	}
	switch (type){
	case Plane:
		textureFile = PLANE_TEXTURE;
		m_normalFile = PLANE_NORM;
		m_specularFile = PLANE_SPEC;
		effectFile = PLANE_EFFECT;
		break;
	case Edge:
		effectFile = EDGE_EFFECT;
		textureFile = EDGE_TEXTURE;
		m_normalFile = PLANE_NORM;
		m_specularFile = PLANE_SPEC;
		break;
	}
	m_texture = LoadTexture(pDevice, textureFile);
	m_specular = LoadTexture(pDevice, m_specularFile);
	m_normal = LoadTexture(pDevice, m_normalFile);

	m_effect = LoadShader(pDevice, effectFile);


	if (m_texture == nullptr || m_effect == nullptr || m_specular == nullptr || m_normal == nullptr)
		return false;
	m_pMesh->Release();
	m_pMesh = newMesh;

	return true;
}

void CWall::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld,
	const D3DXMATRIX& mView,
	const D3DXVECTOR4& camPos){
	if (NULL == pDevice)
		return;
	//pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	//pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
	//m_pMesh->DrawSubset(0);
	//return;

	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	D3DXMATRIX proj;
	pDevice->GetTransform(D3DTS_PROJECTION, &proj);
	pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);

	D3DXMATRIX matWorldView, matWorldViewProjection;
	D3DXMatrixMultiply(&matWorldView, &m_mLocal, &mWorld);
	D3DXMatrixMultiply(&matWorldView, &mWorld, &mView);
	D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &proj);


	m_effect->SetMatrix("gWorldMatrix", &mWorld);
	m_effect->SetMatrix("gWorldViewProjection", &matWorldViewProjection);

	m_effect->SetVector("gWorldCameraPosition", &camPos);
	m_effect->SetVector("gWorldLightPosition", &(D3DXVECTOR4(0, 500, 0, 1)));
	m_effect->SetVector("gLightColor", &(D3DXVECTOR4(100, 100, 100, 1)));
	
	m_effect->SetTexture("DiffuseMap", m_texture);
	m_effect->SetTexture("NormalMap", m_normal);
	m_effect->SetTexture("SpecularMap", m_specular);
	//m_effect->

	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_effect->SetTechnique("ColorShader");
	UINT numPass = 0;
	m_effect->Begin(&numPass, NULL);
	{
		for (UINT i = 0; i < numPass; ++i)
		{
			m_effect->BeginPass(i);
			m_pMesh->DrawSubset(0);
			m_effect->EndPass();
		}
	}
	m_effect->End();

}
//Determines whether the ball has intersected the wall
bool CWall::hasIntersected(CSphere& ball){
	// There are two ways to determine intersection
	// |z - z'| <= r		|		|x - x'| <= r
	// |z + vzt - z'| <= r	|		|x + vxt - x'| <= r
	// After Collision
	// z = z' - r | x = x' - r
	
	D3DXVECTOR3 cord = ball.getCenter();
	
	if (cord.x >= (4.5 - M_RADIUS))  return true;
		//cord.x = 4.5 - M_RADIUS;
	if (cord.x <= (-4.5 + M_RADIUS)) return true;
		//cord.x = -4.5 + M_RADIUS;
	if (cord.z <= (-3 + M_RADIUS))	 return true;
		//cord.z = -3 + M_RADIUS;
	if (cord.z >= (3 - M_RADIUS))	 return true;
		//cord.z = 3 - M_RADIUS;
	
	return false;
}
//changes x-velocity and z-velocity of the ball after collision
bool CWall::hitBy(CSphere& ball){
	
	if (!this->hasIntersected(ball)) return false;

	D3DXVECTOR3 cord = ball.getCenter();

	if (cord.x >= (4.5 - M_RADIUS)) // 공의 X값이 우측으로 치우친 경우 ( 우측 벽에 부딪힌 경우 )
	{
		ball.setCenter(4.5 - M_RADIUS, cord.y, cord.z);
		ball.setPower(ball.getVelocity().x*(-0.5), ball.getVelocity().z); // 방향 전환
	}
	if (cord.x <= (-4.5 + M_RADIUS)) // 공의 X값이 좌측으로 치우친 경우 ( 좌측 벽에 부딪힌 경우 )
	{
		ball.setCenter(-4.5 + M_RADIUS, cord.y, cord.z);
		ball.setPower(ball.getVelocity().x*(-0.5), ball.getVelocity().z); // 방향 전환
	}
	if (cord.z <= (-3 + M_RADIUS)) // 공의 Z값이 아래로 치우친 경우 ( 하측 벽에 부딪힌 경우 )
	{
		ball.setCenter(cord.x, cord.y, -3 + M_RADIUS);
		ball.setPower(ball.getVelocity().x, ball.getVelocity().z*(-0.5)); // 방향 전환
	}
	if (cord.z >= (3 - M_RADIUS))  // 공의 Z값이 위로 치우친 경우 ( 상측 벽에 부딪힌 경우 )
	{
		ball.setCenter(cord.x, cord.y, 3 - M_RADIUS);
		ball.setPower(ball.getVelocity().x, ball.getVelocity().z*(-0.5)); // 방향 전환
	}

	return true;
}
void CWall::destroy(){
	CObject::destroy();
	if (m_specular != nullptr){
		m_specular->Release();
		m_specular = nullptr;
	}
	if (m_normal != nullptr){
		m_normal->Release();
		m_normal = nullptr;
	}
}
LPD3DXMESH CWall::convertMesh(IDirect3DDevice9* pDevice, LPD3DXMESH& mesh){

	LPD3DXMESH newMesh = nullptr;
	VERTEX* pVerts;
	HRESULT result = mesh->CloneMesh(D3DXMESH_SYSTEMMEM, decl, pDevice, &newMesh);

	if (FAILED(result)) return nullptr;
	float u = 0;
	float v = 0;
	bool reverse = false;
	if (SUCCEEDED(newMesh->LockVertexBuffer(0, (LPVOID*)&pVerts))){
		int numVerts = newMesh->GetNumVertices();
		for (int i = 0; i < numVerts; i++){
			pVerts->tu = u;
			pVerts->tv = v;
		
			if (u == 0 && v==0){
				if (reverse)
					u++;
				else
					v++;
				
			}
			else if (v == 1 && u == 0){
				u++;
			
			}
			else if (v == 0 && u == 1){
				v++;
			
			}
			else{
				if (reverse)
					reverse = false;
				else reverse = true;
				u = 0;
				v = 0;
			}
			pVerts++;
		}
		newMesh->UnlockVertexBuffer();
		//temporary uv generator
		return newMesh;
	}
	else{
		return nullptr;
	}
}
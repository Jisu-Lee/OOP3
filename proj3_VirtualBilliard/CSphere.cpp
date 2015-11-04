#include "CSphere.h"
#include <d3d9.h>
CSphere::CSphere(){
	D3DXMatrixIdentity(&m_mLocal);
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));
	m_radius = 0;
	m_velocity_x = 0;
	m_velocity_z = 0;
	m_pMesh = NULL;
}
CSphere::~CSphere(){}
bool CSphere::create(IDirect3DDevice9* pDevice, D3DXCOLOR color){
	if (!CObject::create(pDevice, color))
		return false;/*
	m_texture = LoadTexture(pDevice, SPHERE_TEXTURE);
	if (!m_texture)
		return false;

	m_effect = LoadShader(pDevice, SPHERE_VS_NAME);
	if (!m_effect)
		return false;*/

	m_pMesh = LoadModel(pDevice, SPHERE_MODEL);
	if (!m_pMesh)
		return false;


	return true;
}
void CSphere::destroy(){

	if (m_pMesh != NULL) {
		m_pMesh->Release();
		m_pMesh = NULL;
	}
	if (m_effect != NULL){
		m_effect->Release();
		m_effect = NULL;
	}
	if (m_texture != NULL){
		m_texture->Release();
		m_texture = NULL;
	}
}
void CSphere::draw(IDirect3DDevice9* pDevice,
	const D3DXMATRIX& mWorld,
	const D3DXMATRIX& mView,
	const D3DXMATRIX& mProj)
{
	if (NULL == pDevice)
		return;
	
	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
	pDevice->SetMaterial(&m_mtrl);
	m_pMesh->DrawSubset(0);
	/*
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	m_effect->SetMatrix("gWorld", &mWorld);
	m_effect->SetMatrix("gView", &mView);
	m_effect->SetMatrix("gProj", &mProj);
	m_effect->SetTexture("DiffuseMap", m_texture);
	//pDevice->SetTexture(0, m_texture);
	UINT numPass = 0;
	D3DXHANDLE hTech = m_effect->GetTechniqueByName("SphereVS");
	m_effect->SetTechnique(hTech);
	
	m_effect->Begin(&numPass, NULL);
	{
		for (UINT i = 0; i < numPass; ++i)
		{
			m_effect->BeginPass(i);
			{
				m_pMesh->DrawSubset(0);
			}
			m_effect->EndPass();
		}
	}
	m_effect->End();
	*/

}
bool CSphere::hasIntersected(CSphere& ball)
{
	// Insert your code here.
	return false;
}
void CSphere::hitBy(CSphere& ball)
{
	// Insert your code here.
}
void CSphere::ballUpdate(float timeDiff)
{
	const float TIME_SCALE = 3.3;
	D3DXVECTOR3 cord = this->getCenter();
	double vx = abs(this->getVelocity_X());
	double vz = abs(this->getVelocity_Z());

	if (vx > 0.01 || vz > 0.01)
	{
		float tX = cord.x + TIME_SCALE*timeDiff*m_velocity_x;
		float tZ = cord.z + TIME_SCALE*timeDiff*m_velocity_z;

		//correction of position of ball
		// Please uncomment this part because this correction of ball position is necessary when a ball collides with a wall
		if (tX >= (4.5 - M_RADIUS))
			tX = 4.5 - M_RADIUS;
		else if (tX <= (-4.5 + M_RADIUS))
			tX = -4.5 + M_RADIUS;
		else if (tZ <= (-3 + M_RADIUS))
			tZ = -3 + M_RADIUS;
		else if (tZ >= (3 - M_RADIUS))
			tZ = 3 - M_RADIUS;

		this->setCenter(tX, cord.y, tZ);
	}
	else { this->setPower(0, 0); }
	//this->setPower(this->getVelocity_X() * DECREASE_RATE, this->getVelocity_Z() * DECREASE_RATE);
	double rate = 1 - (1 - DECREASE_RATE)*timeDiff * 400;
	if (rate < 0)
		rate = 0;
	this->setPower(getVelocity_X() * rate, getVelocity_Z() * rate);
}
void CSphere::setCenter(float x, float y, float z){
	setPosition(x, y, z);
}
void CSphere::setPosition(float x, float y, float z){
	D3DXMATRIX m;
	m_x = x;	m_y = y;	m_z = z;
	D3DXMatrixTranslation(&m, x, y, z);
	setLocalTransform(m);
}
LPD3DXEFFECT CSphere::LoadShader(IDirect3DDevice9* pDevice, const char* fileName){
	LPD3DXEFFECT ret = NULL;
	LPD3DXBUFFER pError = NULL;
	DWORD dwShaderFlags = 0;
#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif
	//D3DXCompileShaderFromFile(fileName, 0, 0, "main", "vs_3_0", dwShaderFlags, &m_effect, &pError, NULL);
	D3DXCreateEffectFromFile(pDevice, fileName, NULL, NULL, dwShaderFlags, NULL, &ret, &pError);
	if (!ret && pError){
		int size = pError->GetBufferSize();
		void* ack = pError->GetBufferPointer();
		if (ack){
			char* str = new char[size];
			sprintf(str, (const char*)ack, size);
			OutputDebugString(str);
			delete[] str;
		}
	}
	return ret;

}
LPDIRECT3DTEXTURE9 CSphere::LoadTexture(IDirect3DDevice9* pDevice, const char* fileName){
	LPDIRECT3DTEXTURE9 ret = NULL;
	if (FAILED(D3DXCreateTextureFromFile(pDevice, fileName, &ret))){
		OutputDebugString("Failed to load texture: ");
		OutputDebugString(fileName);
		OutputDebugString("\n");
	}
	return ret;
}
LPD3DXMESH CSphere::LoadModel(IDirect3DDevice9* pDevice, const char* fileName){
	LPD3DXMESH ret = NULL;
	if (FAILED(D3DXLoadMeshFromX(fileName, D3DXMESH_SYSTEMMEM, pDevice, NULL, NULL, NULL, NULL, &ret))){
		OutputDebugString("Failed to load Model: ");
		OutputDebugString(fileName);
		OutputDebugString("\n");
	}
	return ret;
}

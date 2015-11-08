#include "CSphere.h"

#include <d3d9.h>



CSphere::CSphere() {

	D3DXMatrixIdentity(&m_mLocal);
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));
	m_radius = 0;
	m_velocity_x = 0;
	m_velocity_z = 0;
	m_pMesh = NULL;
}

CSphere::~CSphere() {}

bool CSphere::create(IDirect3DDevice9* pDevice, D3DXCOLOR color) {

	if (!CObject::create(pDevice, color))
		return false;
	/*
	m_effect = LoadShader(pDevice, SPHERE_VS_NAME);
	if (!m_effect)
	return false;
	m_texture = LoadTexture(pDevice, SPHERE_TEXTURE);
	if (!m_texture)
	return false;
	*/
	if (FAILED(D3DXCreateSphere(pDevice, getRadius(), 50, 50, &m_pMesh, NULL)))
		return false;
	return true;
}
void CSphere::destroy() {

	if (m_pMesh != NULL) {
		m_pMesh->Release();
		m_pMesh = NULL;
	}
	if (m_effect != NULL) {
		m_effect->Release();
		m_effect = NULL;
	}
	if (m_texture != NULL) {
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
	/*
	m_effect->SetMatrix("gWorld", &mWorld);
	m_effect->SetMatrix("gView", &mView);
	m_effect->SetMatrix("gProj", &mProj);
	m_effect->SetTexture("DiffuseSampler", m_texture);

	UINT numPass = 0;
	m_effect->Begin(&numPass, NULL);
	{
	for (UINT i = 0; i < numPass; ++i){
	//m_effect->BeginPass(i)
	{
	m_pMesh->DrawSubset(0);
	}
	//m_effect->
	}
	}
	m_effect->End();
	*/
	m_pMesh->DrawSubset(0);

}



bool CSphere::hasIntersected(CSphere& ball)
{
	float distance = 0;
	distance = sqrtf((pow(this->getCenter().x - ball.getCenter().x, 2)) + (pow((this->getCenter().z - ball.getCenter().z), 2)));
	//������ ��ġ �̿��� ��� 
	if (distance < 2 * (this->getRadius())) return true; //��ģ�Ŵϱ� 
	else									return false;
}

//need to use const &
float dotproduct(CSphere a, CSphere b)
{
	return a.getVelocity_X() * b.getVelocity_X() + a.getVelocity_Z()*b.getVelocity_Z();
}

float dotproduct(float ax, float az, float bx, float bz)
{
	return ax* bx + az*bz;
}

//calculate absolute value of a given vector( = |u| )
float abs_vector(float x, float y){
	return sqrtf(pow(x, 2) + pow(y, 2));
}

//separate both ball_a and ball_b
void separate(CSphere& ball_a, CSphere& ball_b)
{
	//get coordinates for both balls
	D3DXVECTOR3 cord_a = ball_a.getCenter();
	D3DXVECTOR3 cord_b = ball_b.getCenter();
	// 1. define vector v and v_a, v_b( ball_b�� �߽���ǥ - ball_a�� �߽���ǥ�� v�� ����ϰ� �� a�� b�� ���͸� ���� )
	float v_x = cord_b.x - cord_a.x;
	float v_z = cord_b.z - cord_a.z;
	// 2. find direction of vector( ����v�� ��ġ���Ͱ� 1, 2, 3, 4��и� �߿��� ��� ��и鿡 ���ϴ��� �Ǵ� )
	float direction_x, direction_z;
	v_x > 0 ? direction_x = 1 : direction_x = -1;
	v_z > 0 ? direction_z = 1 : direction_z = -1;
	// 3. calculate theta with vector v( ���� v�� x��� �̷�� ���� ��Ÿtheta ��� )
	float theta = atan(v_z/v_x);	
	// 4. calculate distance a ball has to move( ���� �������� �ϴ� �Ÿ� ��� )
	float length = (sqrtf(pow(v_x, 2) + pow(v_z, 2))); // �߽ɰŸ�
	float distance = M_RADIUS - (length / 2);
	// 5. find ��x, ��z using theta( ��Ÿ�� �̿��ؼ� ��Ÿ x�� ��Ÿ z�� ���Ѵ� )
	float delta_x = abs(distance*cos(theta));
	float delta_z = abs(distance*sin(theta));
	// 6. get new position of both balls using ��x, ��z and vector u( ��Ÿ x, z�� ���� ���� u�� �̿��� ���� ���� ��ǥ�� ����Ѵ� )
	ball_a.setCenter(cord_a.x + delta_x*(-1)*direction_x, cord_a.y, cord_a.z + delta_z*(-1)*direction_z);
	ball_b.setCenter(cord_b.x + delta_x*direction_x, cord_b.y, cord_b.z + delta_z*direction_z);
}

void CSphere::hitBy(CSphere& ball){
	if (hasIntersected(ball))
	{
		CSphere v1, v2, a, b;			//���� ����. v1�� �� this�� �ӵ�����, v2�� �� ball�� �ӵ�����
										//(����� ������ ������ �� �ƴϰ� ���ͷ� ������ �������� �����Ѱ��̴�)

		float v1_x, v1_z, v2_x, v2_z;	//���� ���� v1�� v2�� x, z����
		v1_x = this->getVelocity_X();	//�� ���� ���п� �� ����
		v1_z = this->getVelocity_Z();
		v2_x = ball.getVelocity_X();
		v2_z = ball.getVelocity_Z();

		float v_x, v_z;					//���� v�� x���а� z ����. �� ���� �߽� ��ġ ���̸� �̿��� ���� v ���
		v_x = ball.getCenter().x - this->getCenter().x;		
		v_z = ball.getCenter().z - this->getCenter().z;
		
		float a_x, a_z;					 //���� ���� a�� x, z����
		a_x = v_x / abs_vector(v_x, v_z);//���� v�� �̿��� ���� ���� a���
		a_z = v_z / abs_vector(v_x, v_z);
		float b_x, b_z;					 //���� ���� b�� x, z����
		b_x = a_z;						 //���� a�� �̿��� ���� b ���
		b_z = -a_x;

		v1.setPower(v1_x, v1_z);		 //ó���� ������ ���� v1, v2, a, b�� �� x, z ��ҵ��� �־���
		v2.setPower(v2_x, v2_z);
		a.setPower(a_x, a_z);
		b.setPower(b_x, b_z);
		
		//���� ���� v1', v2'�� ����
		this->setPower(dotproduct(v2, a) * a_x + dotproduct(v1, b) * b_x,
			 dotproduct(v2, a) * a_z + dotproduct(v1, b) * b_z);
		ball.setPower(dotproduct(v1, a) * a_x + dotproduct(v2, b) * b_x,
			 dotproduct(v1, a) * a_z + dotproduct(v2, b) * b_x);

		//���� ������´�
		separate(*this, ball);
	}
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
		/* Please uncomment this part because this correction of ball position is necessary when a ball collides with a wall
		if (tX >= (4.5 - M_RADIUS))
			tX = 4.5 - M_RADIUS;
		else if (tX <= (-4.5 + M_RADIUS))
			tX = -4.5 + M_RADIUS;
		else if (tZ <= (-3 + M_RADIUS))
			tZ = -3 + M_RADIUS;
		else if (tZ >= (3 - M_RADIUS))
			tZ = 3 - M_RADIUS;
		*/
		this->setCenter(tX, cord.y, tZ);
	}
	else { this->setPower(0, 0); }
	//this->setPower(this->getVelocity_X() * DECREASE_RATE, this->getVelocity_Z() * DECREASE_RATE);
	double rate = 1 - (1 - DECREASE_RATE)*timeDiff * 400;
	if (rate < 0)
		rate = 0;
	this->setPower(getVelocity_X() * rate, getVelocity_Z() * rate);
}

void CSphere::setCenter(float x, float y, float z) {

	setPosition(x, y, z);

}

void CSphere::setPosition(float x, float y, float z) {
	D3DXMATRIX m;
	m_x = x;	m_y = y;	m_z = z;
	D3DXMatrixTranslation(&m, x, y, z);
	setLocalTransform(m);
}

LPD3DXEFFECT CSphere::LoadShader(IDirect3DDevice9* pDevice, const char* fileName) {
	LPD3DXEFFECT ret = NULL;
	LPD3DXBUFFER pError = NULL;
	DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;

#endif
	D3DXCreateEffectFromFile(pDevice, fileName, NULL, NULL, dwShaderFlags, NULL, &ret, &pError);
	if (!ret && pError) {
		int size = pError->GetBufferSize();
		void* ack = pError->GetBufferPointer();
		if (ack) {
			char* str = new char[size];
			sprintf(str, (const char*)ack, size);
			OutputDebugString(str);
			delete[] str;
		}
	}
	return ret;

}

LPDIRECT3DTEXTURE9 CSphere::LoadTexture(IDirect3DDevice9* pDevice, const char* fileName) {
	LPDIRECT3DTEXTURE9 ret = NULL;
	if (FAILED(D3DXCreateTextureFromFile(pDevice, fileName, &ret))) {
		OutputDebugString("Failed to load texture: ");
		OutputDebugString(fileName);
		OutputDebugString("\n");
	}
	return ret;
}

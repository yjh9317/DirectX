#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	:CComponent(COMPONENT_TYPE::TRANSFORM)
	,m_vRelativeScale(Vec3(0.5f,0.5f,0.5f))	//��İ��� �� 0�̸� ũ�Ⱑ 0�� ������ �ǹǷ� ���� 1�� ����
{
}

CTransform::~CTransform()
{
}

void CTransform::finalupdate()
{
	//�������ϱ� ������ ������ ����� finalupdate���� ���� ,�������ϱ� ������ ����

	//DirectX �Լ��� ��İ� ����
	Matrix matScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z); 
	Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);
	Matrix matRotX = XMMatrixRotationX(m_vRelativeRot.x);
	Matrix matRotY = XMMatrixRotationY(m_vRelativeRot.y);
	Matrix matRotZ = XMMatrixRotationZ(m_vRelativeRot.z);

	Matrix matRotation = matRotX * matRotY * matRotZ; //���� ����

	// World ��� �����
	// Local Mesh�� World ����� ���ؼ� Wolrd�� ��ġ
	
	// ũ�� x ȸ��(����) x �̵� ������ ��İ� (�����߿�)
	m_matWorld = matScale * matRotation * matTranslation;

	

	// ȸ�� ��� (���� �������� ȸ���ϹǷ� ���� ��ǥ���� �ٲ����ʰ� ������ ��ǥ���� �ٲ��)
	// ������ �ﰢ�Լ��� ���������� �̿��ؼ� ��Ÿ��.

	// x�� ȸ�� ���
	// (  1     0	    0  )
	// (  0    cos��   sin�� )
	// (  0    -sin��  cos�� )
	// 
	// y�� ȸ�� ���
	// ( cos��   0	  sin�� )
	// (  0     1      0   )
	// ( -sin��  0     sin�� )
	// 
	// z�� ȸ�� ���
	// ( cos��   sin��   0  )
	// ( -sin��  cos��   0  )
	// (  0      0     1  )

	// ����� ���������� ��ȯ��Ģ�� �������� �����Ƿ� ȸ��,�̵�,ũ�� ����� ���� �� 
	// ũ��-> ȸ�� -> �̵� ������ ���ؾ��Ѵ�
}



void CTransform::UpdateData()
{
	// ��ǥ������ �������Ǳ� ������ b0�������Ϳ� ������
	CConstBuffer* pBuffer= CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);	// Transform ������Ʈ�� ������۸� ��������
	pBuffer->SetData(&m_matWorld, sizeof(Matrix));							// Ư�� �������Ϳ� ������
	pBuffer->UpdateData();

}


#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	:CComponent(COMPONENT_TYPE::TRANSFORM)
	,m_vRelativeScale(Vec3(1.f,1.f,1.f))	//��İ��� �� 0�̸� ũ�Ⱑ 0�� ������ �ǹǷ� ���� 1�� ����
{
}

CTransform::~CTransform()
{
}

void CTransform::finalupdate()
{
	//�������ϱ� ������ ������ ����� finalupdate���� ���� ,�������ϱ� ������ ����

	//���� ��ķ� ����
	m_matWorld._11 = m_vRelativeScale.x;	m_matWorld._12 = 0.f;					m_matWorld._13 = 0.f;						m_matWorld._14 = 0.f;
	m_matWorld._21 = 0.f;					m_matWorld._22 = m_vRelativeScale.y;	m_matWorld._23 = 0.f;						m_matWorld._24 = 0.f;
	m_matWorld._31 = 0.f;					m_matWorld._32 = 0.f;					m_matWorld._33 = m_vRelativeScale.z;		m_matWorld._34 = 0.f;
	m_matWorld._41 = m_vRelativePos.x;		m_matWorld._42 = m_vRelativePos.y;		m_matWorld._43 = m_vRelativePos.z;			m_matWorld._44 = 1.f;
	
	/* ũ��.x		0		0		0
	*	0		ũ��.y		0		0
	*	0			0	ũ��.z		0
	*  ��ġ.x	��ġ.y	��ġ.z		1
	*/
}



void CTransform::UpdateData()
{
	// ��ǥ������ �������Ǳ� ������ b0�������Ϳ� ������
	CConstBuffer* pBuffer= CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);	// Transform ������Ʈ�� ������۸� ��������
	pBuffer->SetData(&m_matWorld, sizeof(Matrix));							// Ư�� �������Ϳ� ������
	pBuffer->UpdateData();

}


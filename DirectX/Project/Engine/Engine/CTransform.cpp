#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(Vec3(1.f, 1.f, 1.f))
{
}

CTransform::~CTransform()
{
}

void CTransform::finalupdate()
{
	//�������ϱ� ������ ������ ����� finalupdate���� ���� ,�������ϱ� ������ ����

	//DirectX ��� �Լ�
	Matrix matScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);
	Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

	Matrix matRotX = XMMatrixRotationX(m_vRelativeRot.x);
	Matrix matRotY = XMMatrixRotationY(m_vRelativeRot.y);
	Matrix matRotZ = XMMatrixRotationZ(m_vRelativeRot.z);
	Matrix matRotation = matRotX * matRotY * matRotZ; //���� ����

	// ũ�� x ȸ��(����) x �̵� ������ ��İ� (�����߿�)
	m_matWorld = matScale * matRotation * matTranslation;
}

void CTransform::UpdateData()
{
	g_transform.matWorld = m_matWorld;

	// ��ǥ������ �������Ǳ� ������ b0�������Ϳ� ������
	CConstBuffer* pBuffer = CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);// Transform ������Ʈ�� ������۸� ��������
	pBuffer->SetData(&g_transform, sizeof(tTransform));					  	// Ư�� �������Ϳ� ������
	pBuffer->UpdateData();
}

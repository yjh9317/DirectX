#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(Vec3(1.f, 1.f, 1.f))
	, m_bIgnoreParentScale(false)
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


	// �ڽ��� ũ��,ȸ��,�̵��� �θ� �������� ��������� ����ȴ�.
	// (�ڽ��� m_matWorld) * (�θ��� m_matWorld)
	// �θ��� ũ�Ⱑ 100�̰� �ڽ��� ũ�Ⱑ 1�̸� �ڽ��� ũ��� 100,

	// �ڽ��� (ũ��,ȸ��,�̵�) * (�θ��� ũ�� �����) * �θ��� (ũ�� ,ȸ�� ,�̵�)
	// ==> ũ�⸦ ������ �θ��� ȸ��,�̵��� ����
	
	if (GetOwner()->GetParent())
	{
		Matrix matParentWorld = GetOwner()->GetParent()->Transform()->GetWorldMat();

		if (m_bIgnoreParentScale)
		{
			Vec3 vParentWorldScale = GetOwner()->GetParent()->Transform()->GetWorldScale();
			Matrix matParentScale = XMMatrixScaling(vParentWorldScale.x, vParentWorldScale.y, vParentWorldScale.z);
			Matrix MatParentScaleInv = XMMatrixInverse(nullptr, matParentScale);

			m_matWorld = m_matWorld * MatParentScaleInv * matParentWorld;
		}
		else
		{
			m_matWorld *= matParentWorld;
		}
		
	}
}

// ������Ʈ�� ��� �θ���ĸ� ����ؼ� ũ�⸦ �޾ƿ´�.
Vec3 CTransform::GetWorldScale()
{
	Vec3 vWorldScale = m_vRelativeScale;

	CGameObject* pParent = GetOwner()->GetParent();
	
	while (pParent)
	{
		vWorldScale *= pParent->Transform()->GetScale();

		pParent = pParent->GetParent();
	}

	// ������ ���� ũ��
	return vWorldScale;
}

void CTransform::UpdateData()
{
	g_transform.matWorld = m_matWorld;
	g_transform.matWV = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;

	// ��ǥ������ �������Ǳ� ������ b0�������Ϳ� ������
	CConstBuffer* pBuffer = CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);// Transform ������Ʈ�� ������۸� ��������
	pBuffer->SetData(&g_transform, sizeof(tTransform));					  	// Ư�� �������Ϳ� ������
	pBuffer->UpdateData();
}

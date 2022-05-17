#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(Vec3(1.f, 1.f, 1.f))
	, m_arrRelativeDir{}
	, m_arrWorldDir{}
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
	Matrix matRotation = matRotX * matRotY * matRotZ; //���� ����,���� ȸ�� ����

	// ũ�� x ȸ��(����) x �̵� ������ ��İ� (�����߿�)
	m_matWorld = matScale * matRotation * matTranslation;

	// RelativeDir ���ϱ�
	static Vec3 vAxis[(UINT)DIR_TYPE::END] = { Vec3::Right,Vec3::Up,Vec3::Front };


	for (int i = 0; i < (int)DIR_TYPE::END; ++i)
	{
		// vAxis[i](ȸ������ �ʾ��� ���� ������) * ȸ�����
		// �θ� �����ٸ� ��� ȸ������ �� ����
		m_arrWorldDir[i] = m_arrRelativeDir[i] = XMVector3TransformNormal(vAxis[i], matRotation);

	}


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
			Matrix matParentScaleInv = XMMatrixInverse(nullptr, matParentScale);

			m_matWorld = m_matWorld * matParentScaleInv * matParentWorld;
		}
		else
		{
			m_matWorld *= matParentWorld;
		}

		// World Dir ���ϱ�
		for (int i = 0; i < (int)DIR_TYPE::END; ++i)
		{
			// �θ� ������ �θ���ı��� ���Ѵ�.
			m_arrWorldDir[i] = XMVector3TransformNormal(m_arrRelativeDir[i], matParentWorld);
			m_arrWorldDir[i].Normalize(); // ���̰� �ٲ���� �����Ƿ� �븻������
		}
	}


}

void CTransform::active()
{
	CComponent::active();
	assert(nullptr);
}

void CTransform::deactive()
{
	CComponent::deactive();
	assert(nullptr);
}

// ������Ʈ�� ��� �θ���ĸ� ����ؼ� ũ�⸦ �޾ƿ´�.
Vec3 CTransform::GetWorldScale()
{
	Vec3 vWorldScale = m_vRelativeScale;

	CGameObject* pParent = GetOwner()->GetParent();
	if (m_bIgnoreParentScale)	// �θ� ������Ʈ ũ�⸦ ����
		pParent = nullptr;

	while (pParent)
	{
		vWorldScale *= pParent->Transform()->GetRelativeScale();

		bool bIgnoreParentScale = pParent->Transform()->m_bIgnoreParentScale;
		pParent = pParent->GetParent();

		if (bIgnoreParentScale)
			pParent = nullptr;
	}

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

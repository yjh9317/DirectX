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
	//렌더링하기 직전에 전달할 행렬을 finalupdate에서 생성 ,렌더링하기 직전에 전달

	//DirectX 행렬 함수
	Matrix matScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);
	Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

	Matrix matRotX = XMMatrixRotationX(m_vRelativeRot.x);
	Matrix matRotY = XMMatrixRotationY(m_vRelativeRot.y);
	Matrix matRotZ = XMMatrixRotationZ(m_vRelativeRot.z);
	Matrix matRotation = matRotX * matRotY * matRotZ; //라디안 단위

	// 크기 x 회전(자전) x 이동 순서로 행렬곱 (순서중요)
	m_matWorld = matScale * matRotation * matTranslation;


	// 자식의 크기,회전,이동은 부모를 기준으로 상대적으로 적용된다.
	// (자식의 m_matWorld) * (부모의 m_matWorld)
	// 부모의 크기가 100이고 자식의 크기가 1이면 자식의 크기는 100,

	// 자식의 (크기,회전,이동) * (부모의 크기 역행렬) * 부모의 (크기 ,회전 ,이동)
	// ==> 크기를 제외한 부모의 회전,이동만 적용
	
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

// 오브젝트의 모든 부모행렬를 계산해서 크기를 받아온다.
Vec3 CTransform::GetWorldScale()
{
	Vec3 vWorldScale = m_vRelativeScale;

	CGameObject* pParent = GetOwner()->GetParent();
	
	while (pParent)
	{
		vWorldScale *= pParent->Transform()->GetScale();

		pParent = pParent->GetParent();
	}

	// 본인의 최종 크기
	return vWorldScale;
}

void CTransform::UpdateData()
{
	g_transform.matWorld = m_matWorld;
	g_transform.matWV = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;

	// 좌표정보가 렌더링되기 직전에 b0레지스터에 보내짐
	CConstBuffer* pBuffer = CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);// Transform 컴포넌트의 상수버퍼를 가져오고
	pBuffer->SetData(&g_transform, sizeof(tTransform));					  	// 특정 레지스터에 보낸다
	pBuffer->UpdateData();
}

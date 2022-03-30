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
}

void CTransform::UpdateData()
{
	g_transform.matWorld = m_matWorld;

	// 좌표정보가 렌더링되기 직전에 b0레지스터에 보내짐
	CConstBuffer* pBuffer = CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);// Transform 컴포넌트의 상수버퍼를 가져오고
	pBuffer->SetData(&g_transform, sizeof(tTransform));					  	// 특정 레지스터에 보낸다
	pBuffer->UpdateData();
}

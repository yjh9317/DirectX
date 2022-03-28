#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	:CComponent(COMPONENT_TYPE::TRANSFORM)
	,m_vRelativeScale(Vec3(1.f,1.f,1.f))	//행렬곱할 때 0이면 크기가 0이 고정이 되므로 전부 1로 설정
{
}

CTransform::~CTransform()
{
}

void CTransform::finalupdate()
{
	//렌더링하기 직전에 전달할 행렬을 finalupdate에서 생성 ,렌더링하기 직전에 전달

	//값을 행렬로 저장
	m_matWorld._11 = m_vRelativeScale.x;	m_matWorld._12 = 0.f;					m_matWorld._13 = 0.f;						m_matWorld._14 = 0.f;
	m_matWorld._21 = 0.f;					m_matWorld._22 = m_vRelativeScale.y;	m_matWorld._23 = 0.f;						m_matWorld._24 = 0.f;
	m_matWorld._31 = 0.f;					m_matWorld._32 = 0.f;					m_matWorld._33 = m_vRelativeScale.z;		m_matWorld._34 = 0.f;
	m_matWorld._41 = m_vRelativePos.x;		m_matWorld._42 = m_vRelativePos.y;		m_matWorld._43 = m_vRelativePos.z;			m_matWorld._44 = 1.f;
	
	/* 크기.x		0		0		0
	*	0		크기.y		0		0
	*	0			0	크기.z		0
	*  위치.x	위치.y	위치.z		1
	*/
}



void CTransform::UpdateData()
{
	// 좌표정보가 렌더링되기 직전에 b0레지스터에 보내짐
	CConstBuffer* pBuffer= CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);	// Transform 컴포넌트의 상수버퍼를 가져오고
	pBuffer->SetData(&m_matWorld, sizeof(Matrix));							// 특정 레지스터에 보낸다
	pBuffer->UpdateData();

}


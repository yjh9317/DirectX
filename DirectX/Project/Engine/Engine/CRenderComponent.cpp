#include "pch.h"
#include "CRenderComponent.h"



CRenderComponent::CRenderComponent(COMPONENT_TYPE _type)
	:CComponent(_type)
	, m_pMesh(nullptr)
	, m_pMtrl(nullptr)
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& _origin)
	:CComponent(_origin)
	, m_pMesh(_origin.m_pMesh)
	, m_pMtrl(nullptr)
	, m_pSharedMtrl(_origin.m_pMtrl)
	, m_pDynamicMtrl(nullptr)
{
	if (nullptr == _origin.m_pDynamicMtrl)
	{
		GetDynamicMaterial();
	}
	else if (nullptr != m_pSharedMtrl)
	{
		SetSharedMaterial(m_pSharedMtrl);
	}
}

CRenderComponent::~CRenderComponent()
{
	if (nullptr != m_pDynamicMtrl)
		delete m_pDynamicMtrl.Get();
}


void CRenderComponent::SetSharedMaterial(Ptr<CMaterial> _pMtrl)
{
	m_pSharedMtrl = _pMtrl;
	m_pMtrl = m_pSharedMtrl;
}

Ptr<CMaterial> CRenderComponent::GetSharedMaterial()
{
	m_pMtrl = m_pSharedMtrl;

	return m_pMtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial()
{
	// 공유 매터리얼을 클론해서 동적 매터리얼로 사용 (자기만의 전용 매터리얼로 사용)
	if (nullptr == m_pDynamicMtrl)
	{
		m_pDynamicMtrl = m_pSharedMtrl->Clone();
	}

	m_pMtrl = m_pDynamicMtrl;
	return m_pMtrl;
}

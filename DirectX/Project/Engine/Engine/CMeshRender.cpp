#include "pch.h"
#include "CMeshRender.h"

#include "CMesh.h"
#include "CMaterial.h"

#include "CTransform.h"
#include "CAnimator2D.h"

CMeshRender::CMeshRender()
	: CComponent(COMPONENT_TYPE::MESHRENDER)
	, m_pMesh(nullptr)
	, m_pMtrl(nullptr)
{
}

CMeshRender::CMeshRender(const CMeshRender& _meshrender)
	:CComponent(COMPONENT_TYPE::MESHRENDER)
	, m_pMesh(_meshrender.m_pMesh)
	, m_pMtrl(nullptr)
	, m_pSharedMtrl(_meshrender.m_pMtrl)
	, m_pDynamicMtrl(nullptr)
{
	if (nullptr == _meshrender.m_pDynamicMtrl)
	{
		GetDynamicMaterial();
	}
	else if (nullptr != m_pSharedMtrl)
	{
		SetSharedMaterial(m_pSharedMtrl);
	}
}

CMeshRender::~CMeshRender()
{
	if (nullptr != m_pDynamicMtrl)
		delete m_pDynamicMtrl.Get();
}

void CMeshRender::SetSharedMaterial(Ptr<CMaterial> _pMtrl)
{

	m_pSharedMtrl = _pMtrl;	
	m_pMtrl = m_pSharedMtrl;
}

Ptr<CMaterial> CMeshRender::GetSharedMaterial()
{
	// 공유 재질 사용
	m_pMtrl = m_pSharedMtrl;

	return m_pMtrl;
}

Ptr<CMaterial> CMeshRender::GetDynamicMaterial()
{
	// 공유 매터리얼을 클론해서 동적 매터리얼로 사용 (자기만의 전용 매터리얼로 사용)
	if (nullptr == m_pDynamicMtrl)
	{
		m_pDynamicMtrl = m_pSharedMtrl->Clone();
	}

	m_pMtrl = m_pDynamicMtrl;
	return m_pMtrl;
}

void CMeshRender::finalupdate()
{
}

void CMeshRender::render()
{
	if (nullptr == m_pMesh || nullptr == m_pMtrl)
		return;

	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}

	Transform()->UpdateData();
	m_pMtrl->UpdateData();	     // 재질(매터리얼) 업데이트
	m_pMesh->render();		     // 메쉬 업데이트(렌더)

	// 애니메이션이 끝나고 상수버퍼 클리어
	if (Animator2D())
	{
		CAnimator2D::Clear();
	}
}

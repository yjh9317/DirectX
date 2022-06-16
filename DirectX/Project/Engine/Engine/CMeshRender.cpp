#include "pch.h"
#include "CMeshRender.h"

#include "CMesh.h"
#include "CMaterial.h"

#include "CTransform.h"
#include "CAnimator2D.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)

{
}


CMeshRender::~CMeshRender()
{

}


void CMeshRender::finalupdate()
{
}

void CMeshRender::render()
{
	if (nullptr == GetMesh() || nullptr == GetMaterial())
		return;

	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}

	Transform()->UpdateData();
	GetMaterial()->UpdateData();		 // 재질(매터리얼) 업데이트
	GetMesh()->render();			 // 메쉬 업데이트(렌더)

		// 애니메이션이 끝나고 상수버퍼 클리어
	if (Animator2D())
	{
		CAnimator2D::Clear();
	}
}

void CMeshRender::SaveToScene(FILE* _pFile)
{
	CRenderComponent::SaveToScene(_pFile);

}

void CMeshRender::LoadFromScene(FILE* _pFile)
{
	CRenderComponent::LoadFromScene(_pFile);

}



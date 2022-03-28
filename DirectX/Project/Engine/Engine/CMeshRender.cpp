#include "pch.h"
#include "CMeshRender.h"

#include "CMesh.h"
#include "CGraphicsShader.h"

#include "CTransform.h"

CMeshRender::CMeshRender()
	:CComponent(COMPONENT_TYPE::MESHRENDER)
	,m_pMesh(nullptr)
	,m_pShader(nullptr)
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
	if (nullptr == m_pMesh || nullptr == m_pShader)	//메쉬,셰이더가 없으면 종료(있어야 그릴 수 있으니까)
		return;

	Transform()->UpdateData();

	m_pShader->UpdateData();	// 셰이더 업데이트
	m_pMesh->render();			// 메쉬 업데이트(렌더)
}


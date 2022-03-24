#include "pch.h"
#include "CMeshRender.h"

#include "CMesh.h"
#include "CGraphicsShader.h"


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
	if (nullptr == m_pMesh || nullptr == m_pShader)	//�޽�,�������� ������ ����(�־�� �׸� �� �����ϱ�)
		return;

	m_pShader->UpdateData();	// ���̴� ������Ʈ
	m_pMesh->render();			// �޽� ������Ʈ(����)
}


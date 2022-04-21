#include "pch.h"
#include "CTileMap.h"

#include "CTransform.h"

#include "CResMgr.h"

CTileMap::CTileMap()
	:CRenderComponent(COMPONENT_TYPE::TILEMAP)
	, m_iImgIdx(0)
	, m_iRowCount(0)
	, m_iColCount(0)
{
	// �޽� , ����
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TileMapMtrl"));
}

CTileMap::~CTileMap()
{
}


void CTileMap::finalupdate()
{
	if (nullptr == m_pAtlasTex)
		return;

	// ��Ʋ���� �� ���� ���μ��� UV
	m_vSliceUV = m_vSlicePixel / Vec2(m_pAtlasTex->Width(),m_pAtlasTex->Height());

	// ��Ʋ�󽺿��� Ÿ���� ��, �� ���� ���ϱ�
	m_iColCount = (UINT)m_pAtlasTex->Width()/ (UINT)m_vSlicePixel.x;
	m_iRowCount = (UINT)m_pAtlasTex->Height()/ (UINT)m_vSlicePixel.y;

	int iCol = m_iImgIdx / m_iColCount; // ��
	int iRow = m_iImgIdx % m_iColCount; // ��

	m_vLTUV = Vec2(m_vSliceUV.x* iCol, m_vSliceUV.y* iRow); //�»�� UV��
}

void CTileMap::UpdateData()
{
	if (nullptr == m_pAtlasTex)
		return;


	// ��Ʋ�� ����
	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_pAtlasTex);

	// ������ۿ� LT,Slice �� ����
	
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, &m_vLTUV);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_1, &m_vSliceUV);
}



void CTileMap::render()
{

	if (nullptr == GetMesh() || nullptr == GetMaterial() || nullptr == m_pAtlasTex)
		return;

	UpdateData(); // ������ ������ ��Ʋ�󽺸� ����

	Transform()->UpdateData();
	GetMaterial()->UpdateData();		 // ����(���͸���) ������Ʈ
	GetMesh()->render();			 // �޽� ������Ʈ(����)

	
}


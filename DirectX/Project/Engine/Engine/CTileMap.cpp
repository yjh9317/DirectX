#include "pch.h"
#include "CTileMap.h"

#include "CTransform.h"

#include "CResMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTileMap::CTileMap()
	:CRenderComponent(COMPONENT_TYPE::TILEMAP)
	, m_iRowCount(0)
	, m_iColCount(0)
	, m_iTileCountX(4)
	, m_iTileCountY(4)
{
	// �޽� , ����
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TileMapMtrl"));

	m_vecTileData.resize(m_iTileCountX * m_iTileCountY);
}

CTileMap::~CTileMap()
{
}


void CTileMap::finalupdate()
{		
}

void CTileMap::UpdateData()
{
	if (nullptr == m_pAtlasTex)
		return;


	// ��Ʋ�� ����
	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_pAtlasTex);

	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &m_iTileCountX);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, &m_iTileCountY);
	
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, &m_vSliceUV);

	//��� Ÿ�� �����͸� ������۸� ���� �������ͷ� ���ε�
	static CConstBuffer* pCB = CDevice::GetInst()->GetCB(CB_TYPE::TILEMAP);
	pCB->SetData(m_vecTileData.data(), sizeof(tTileData) * m_iTileCountX * m_iTileCountY); //������۷� ����
	pCB->UpdateData();// �������Ϳ� ���ε�
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


void CTileMap::SetTileMapCount(UINT _iCountX, UINT _iCountY)
{
	m_iTileCountX = _iCountX;
	m_iTileCountY = _iCountY;

	// Ÿ�� ������ �ٲ����Ƿ� clear
	ClearTileData();
}


void CTileMap::SetTileData(int _iTileIdx, int _iImgIdx)
{
	if (nullptr == m_pAtlasTex)
	{
		return;
	}

	m_vecTileData[_iTileIdx].iImgIdx = _iImgIdx;

	// ��Ʋ�󽺿��� Ÿ���� ��, �� ���� ���ϱ�
	m_iColCount = (UINT)m_pAtlasTex->Width() / (UINT)m_vSlicePixel.x;
	m_iRowCount = (UINT)m_pAtlasTex->Height() / (UINT)m_vSlicePixel.y;

	int iRow = m_vecTileData[_iTileIdx].iImgIdx / m_iColCount;
	int iCol = m_vecTileData[_iTileIdx].iImgIdx % m_iColCount;

	m_vecTileData[_iTileIdx].vLTUV = Vec2(m_vSliceUV.x * iCol, m_vSliceUV.y * iRow);
}

void CTileMap::ClearTileData()
{
	vector<tTileData> vecTileData;
	m_vecTileData.swap(vecTileData);

	m_vecTileData.resize(m_iTileCountX * m_iTileCountY);
}



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
	// 메쉬 , 재질
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

	// 아틀라스의 한 블럭의 가로세로 UV
	m_vSliceUV = m_vSlicePixel / Vec2(m_pAtlasTex->Width(),m_pAtlasTex->Height());

	// 아틀라스에서 타일의 행, 열 개수 구하기
	m_iColCount = (UINT)m_pAtlasTex->Width()/ (UINT)m_vSlicePixel.x;
	m_iRowCount = (UINT)m_pAtlasTex->Height()/ (UINT)m_vSlicePixel.y;

	int iCol = m_iImgIdx / m_iColCount; // 행
	int iRow = m_iImgIdx % m_iColCount; // 열

	m_vLTUV = Vec2(m_vSliceUV.x* iCol, m_vSliceUV.y* iRow); //좌상단 UV값
}

void CTileMap::UpdateData()
{
	if (nullptr == m_pAtlasTex)
		return;


	// 아틀라스 설정
	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_pAtlasTex);

	// 상수버퍼에 LT,Slice 값 전달
	
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, &m_vLTUV);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_1, &m_vSliceUV);
}



void CTileMap::render()
{

	if (nullptr == GetMesh() || nullptr == GetMaterial() || nullptr == m_pAtlasTex)
		return;

	UpdateData(); // 렌더링 직전에 아틀라스를 설정

	Transform()->UpdateData();
	GetMaterial()->UpdateData();		 // 재질(매터리얼) 업데이트
	GetMesh()->render();			 // 메쉬 업데이트(렌더)

	
}


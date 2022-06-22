#include "pch.h"
#include "CLight2D.h"

#include "CRenderMgr.h"
#include "CTransform.h"

CLight2D::CLight2D()
	: CComponent(COMPONENT_TYPE::LIGHT2D)
	, m_iLightIdx(-1)
{
}

CLight2D::~CLight2D()
{
}

void CLight2D::update()
{
}

void CLight2D::finalupdate()
{
	m_LightInfo.vWorldPos = Transform()->GetWorldPos();	// 광원의 타입이 POINT (ex 전구)라면 광원의 위치도 중요하므로 WorldPos를 가져온다.

	m_iLightIdx = CRenderMgr::GetInst()->RegisterLight2D(this);
}

void CLight2D::SaveToScene(FILE* _pFile)
{
}

void CLight2D::LoadFromScene(FILE* _pFile)
{
}


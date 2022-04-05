#include "pch.h"
#include "CScene.h"

#include "CLayer.h"
#include "CGameObject.h"



CScene::CScene()
	: m_arrLayer{}
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i] = new CLayer;		// 먼저 모든 레이어 생성
	}
}

CScene::~CScene()
{
	Safe_Del_Arr(m_arrLayer);
}


void CScene::start()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->start();
	}
}

void CScene::update()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->update();
	}
}

void CScene::lateupdate()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->lateupdate();
	}
}

void CScene::finalupdate()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->finalupdate();
	}
}

void CScene::render()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->render();
	}


	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->Clear();
	}
}

void CScene::SetLayerName(int _iLayerIdx, const wstring& _strName)
{
	assert(0 <= _iLayerIdx && _iLayerIdx < MAX_LAYER); //레이어의 범위가 아니라면 assert
	m_arrLayer[_iLayerIdx]->SetName(_strName);
}


int CScene::GetLayerIdxFromName(const wstring& _strName)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		if (m_arrLayer[i]->GetName() == _strName)
		{
			return i;
		}
	}

	return -1;
}

void CScene::AddObject(CGameObject* _pObj, const wstring& _strLayerName)
{
	int iLayerIdx = GetLayerIdxFromName(_strLayerName);

	assert(iLayerIdx != -1);	    // 없는 레이어였다면 assert
	assert(!_pObj->m_pParent);		// 부모가 있는 오브젝트였다면 assert,최상위 오브젝트만 가능

	AddObject(_pObj, iLayerIdx);
}

void CScene::AddObject(CGameObject* _pObj, int _iLayerIdx)
{
	assert(0 <= _iLayerIdx && _iLayerIdx < MAX_LAYER);
	assert(!_pObj->m_pParent);

	m_arrLayer[_iLayerIdx]->AddObject(_pObj);

	_pObj->m_iLayerIdx = _iLayerIdx;
}
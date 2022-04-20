#include "pch.h"
#include "CLayer.h"

#include "CGameObject.h"

CLayer::CLayer()
	: m_iLayerIdx(-1)
{
}

CLayer::~CLayer()
{
	Safe_Del_Vec(m_vecRoot);
}

void CLayer::start()
{
	for (size_t i = 0; i < m_vecRoot.size(); ++i)
	{
		m_vecRoot[i]->start();
	}
}

void CLayer::update()
{
	for (size_t i = 0; i < m_vecRoot.size(); ++i)
	{
		m_vecRoot[i]->update();
	}
}

void CLayer::lateupdate()
{
	for (size_t i = 0; i < m_vecRoot.size(); ++i)
	{
		m_vecRoot[i]->lateupdate();
	}
}

void CLayer::finalupdate()
{
	vector<CGameObject*>::iterator iter = m_vecRoot.begin();

	for (; iter != m_vecRoot.end(); )
	{
		(*iter)->finalupdate();

		if ((*iter)->IsDead())
		{
			iter = m_vecRoot.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}


void CLayer::AddObject(CGameObject* _pObj)
{
	m_vecRoot.push_back(_pObj);
}

void CLayer::DeregisterObject(CGameObject* _pObj)
{
	//최상위 오브젝트가 아니라면 반환
	if (_pObj->GetParent())
		return;


	// 레이어가 등록되어 있었다면 해당 레이어에서 오브젝트를 제거
	vector<CGameObject*>::iterator iter = m_vecRoot.begin();
	for (; iter != m_vecRoot.end(); ++iter)
	{
		if ((*iter) == _pObj)
		{
			m_vecRoot.erase(iter);
			_pObj->m_iLayerIdx = -1;
			return;
		}
	}
}


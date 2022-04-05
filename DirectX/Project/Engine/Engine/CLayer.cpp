#include "pch.h"
#include "CLayer.h"

#include "CGameObject.h"

CLayer::CLayer()
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

void CLayer::render()
{
	// 최상위 오브젝트가 아닌 레이어에 있는 모든 오브젝트를 렌더
	for (size_t i = 0; i < m_vecObj.size(); ++i)
	{
		m_vecObj[i]->render();
	}	
}

void CLayer::AddObject(CGameObject* _pObj)
{
	m_vecRoot.push_back(_pObj);
}


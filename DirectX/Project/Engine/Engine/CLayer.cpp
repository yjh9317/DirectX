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
	for (size_t i = 0; i < m_vecRoot.size(); ++i)
	{
		m_vecRoot[i]->finalupdate();
	}
}

void CLayer::render()
{
	for (size_t i = 0; i < m_vecRoot.size(); ++i)
	{
		m_vecRoot[i]->render();
	}
}

void CLayer::AddObject(CGameObject* _pObj)
{
	m_vecRoot.push_back(_pObj);
}

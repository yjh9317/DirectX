#include "pch.h"
#include "CScene.h"

#include "CLayer.h"
#include "CGameObject.h"



CScene::CScene()
	: m_arrLayer{}
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i] = new CLayer;		// ���� ��� ���̾� ����
		m_arrLayer[i]->m_iLayerIdx = i; // ���̾� �ε��� 
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
	assert(0 <= _iLayerIdx && _iLayerIdx < MAX_LAYER); //���̾��� ������ �ƴ϶�� assert
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

void CScene::AddObject(CGameObject* _pRootObj, const wstring& _strLayerName)
{
	int iLayerIdx = GetLayerIdxFromName(_strLayerName);

	assert(iLayerIdx != -1);	    // ���� ���̾�ٸ� assert
	assert(!_pRootObj->m_pParent);	// �θ� �ִ� ������Ʈ���ٸ� assert,�ֻ��� ������Ʈ�� ����

	AddObject(_pRootObj, iLayerIdx);
}

void CScene::AddObject(CGameObject* _pRootObj, int _iLayerIdx)
{
	assert(0 <= _iLayerIdx && _iLayerIdx < MAX_LAYER);
	assert(!_pRootObj->m_pParent);

	m_arrLayer[_iLayerIdx]->AddObject(_pRootObj);

	//�ڽ� ������Ʈ �鵵 �ش� ���̾��� �ε����� �˷��ش�.
	list<CGameObject*> queue;
	
	queue.push_back(_pRootObj);

	// �θ� ������Ʈ ����, �ڽĵ� ��� �ش� ���̾��� �ε����� �˷��ش� (Ư�� ���̾� �Ҽ��� �ƴѰ��)
	while (!queue.empty())
	{
		CGameObject* pTargetObj = queue.front();
		queue.pop_front();
		
		// ���̾ ���Ҽ� �̿��� ���
		if(-1 == pTargetObj->m_iLayerIdx)
		pTargetObj->m_iLayerIdx = _iLayerIdx;

		const vector<CGameObject*>& vecChild=pTargetObj->GetChild();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}
	}
}
CLayer* CScene::GetLayer(const wstring& _strLayerName)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (_strLayerName == m_arrLayer[i]->GetName())
		{
			return m_arrLayer[i];
		}
	}

	return nullptr;
}

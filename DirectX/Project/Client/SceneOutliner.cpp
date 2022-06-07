#include "pch.h"
#include "SceneOutliner.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

#include "TreeUI.h"
#include "CImGuiMgr.h"
#include "InspectorUI.h"

SceneOutliner::SceneOutliner()
	: UI("SceneOutliner")
{
	m_TreeUI = new TreeUI(true);					 // 생성자에서 트리UI 생성
	m_TreeUI->SetTitle("SceneOutliner");	 // 트리UI의 타이틀
	AddChild(m_TreeUI);						 // SceneOutlinerUI의 TreeUI
	// Clicked Delegate 등록
	m_TreeUI->SetClickedDelegate(this, (CLICKED)&SceneOutliner::ObjectClicked);

	Reset();
}

SceneOutliner::~SceneOutliner()
{
}



void SceneOutliner::update()
{

	UI::update();
}

void SceneOutliner::render_update()
{

}

void SceneOutliner::Reset()
{
	// 현재 Scene 을 가져온다.
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pCurScene->GetLayer(i);

		vector<CGameObject*>& vecRoots = pLayer->GetRootObjects();

		for (size_t i = 0; i < vecRoots.size(); ++i)
		{
			AddGameObjectToTree(vecRoots[i], m_TreeUI->GetDummyNode());
		}
	}
}

void SceneOutliner::ObjectClicked(DWORD_PTR _dw)
{
	TreeNode* pNode = (TreeNode*)_dw;

	string strKey = pNode->GetName();
	CGameObject* pObject = (CGameObject*)pNode->GetData();

	assert(pObject);

	// InspectorUI 를 얻어옴
	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	pInspectorUI->SetTargetObject(pObject);
}

void SceneOutliner::AddGameObjectToTree(CGameObject* _pObject, TreeNode* _pDestNode)	// 자식노드한테 자식노드가 있을수 있으므로 재귀로 모든 노드 처리
{
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
		, string(_pObject->GetName().begin(), _pObject->GetName().end())
		, (DWORD_PTR)_pObject);

	const vector<CGameObject*>& vecChild = _pObject->GetChild();

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		AddGameObjectToTree(vecChild[i], pNode);
	}
}

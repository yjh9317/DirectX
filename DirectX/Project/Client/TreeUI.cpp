#include "pch.h"
#include "TreeUI.h"


// ========
// TreeNode
// ========
TreeNode::TreeNode()
	: m_pParent(nullptr)
	, m_bLeaf(true)
	, m_dwData(0)
{
}

TreeNode::TreeNode(const string& _strName, DWORD_PTR _dwData)
	: m_pParent(nullptr)
	, m_bLeaf(true)
	, m_strName(_strName)
	, m_dwData(_dwData)
{
}

TreeNode::~TreeNode()
{
	Safe_Del_Vec(m_vecChild);
}


void TreeNode::update()
{
	// update 작성


	// child update
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->update();
	}
}

void TreeNode::render_update()
{
	// ImGuiTreeNodeFlags_	
	ImGuiTreeNodeFlags eFlag = ImGuiTreeNodeFlags_None;

	if (m_bLeaf)
		eFlag |= ImGuiTreeNodeFlags_Leaf;
	if (nullptr == m_pParent || m_pParent->m_strName == "DummyRoot")
		eFlag |= ImGuiTreeNodeFlags_Framed;

	if (ImGui::TreeNodeEx(m_strName.c_str(), eFlag))
	{
		for (size_t i = 0; i < m_vecChild.size(); ++i)
		{
			m_vecChild[i]->render_update();
		}

		ImGui::TreePop();
	}
}



// ======
// TreeUI
// ======
TreeUI::TreeUI(bool _bDummyRoot)
	: UI("##TreeUI")
	, m_pRootNode(nullptr)
	, m_bUseDummyRoot(_bDummyRoot)
	, m_bShowDummy(false)
{
	if (m_bUseDummyRoot)
	{
		AddTreeNode(nullptr, "DummyRoot");
	}
}

TreeUI::~TreeUI()
{
	SAFE_DELETE(m_pRootNode);
}

void TreeUI::update()
{
	if (nullptr == m_pRootNode)
		return;

	// 트리UI 가 부착된 부모 UI 의 사이즈를 받아온다.
	SetSize(GetParentUI()->GetSize());

	m_pRootNode->update();

	// 자식 UI 업데이트
	UI::update();
}

void TreeUI::render_update()
{
	if (nullptr == m_pRootNode)
		return;

	if (m_bUseDummyRoot)
	{
		if (m_bShowDummy)
			m_pRootNode->render_update();
		else
		{
			for (size_t i = 0; i < m_pRootNode->m_vecChild.size(); ++i)
			{
				m_pRootNode->m_vecChild[i]->render_update();
			}
		}
	}

	else
	{
		m_pRootNode->render_update();
	}
}

TreeNode* TreeUI::AddTreeNode(TreeNode* _pParentNode, const string& _strName, DWORD_PTR _dwData)
{
	TreeNode* pNewNode = new TreeNode(_strName, _dwData);

	// 부모를 지정함
	if (nullptr != _pParentNode)
	{
		_pParentNode->AddChild(pNewNode);
	}
	// 부모가 지정되지 않고
	else if (nullptr == _pParentNode)
	{
		// 루트노드가 존재함
		if (nullptr != m_pRootNode)
			m_pRootNode->AddChild(pNewNode);

		// 루트노드가 존재하지 않음
		else
			m_pRootNode = pNewNode;
	}

	return pNewNode;
}


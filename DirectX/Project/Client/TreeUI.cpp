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
	// update �ۼ�


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

	// Ʈ��UI �� ������ �θ� UI �� ����� �޾ƿ´�.
	SetSize(GetParentUI()->GetSize());

	m_pRootNode->update();

	// �ڽ� UI ������Ʈ
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

	// �θ� ������
	if (nullptr != _pParentNode)
	{
		_pParentNode->AddChild(pNewNode);
	}
	// �θ� �������� �ʰ�
	else if (nullptr == _pParentNode)
	{
		// ��Ʈ��尡 ������
		if (nullptr != m_pRootNode)
			m_pRootNode->AddChild(pNewNode);

		// ��Ʈ��尡 �������� ����
		else
			m_pRootNode = pNewNode;
	}

	return pNewNode;
}


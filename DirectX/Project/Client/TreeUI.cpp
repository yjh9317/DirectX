#include "pch.h"
#include "TreeUI.h"


// ========
// TreeNode
// ========
TreeNode::TreeNode()
	: m_pTreeUI(nullptr)
	, m_pParent(nullptr)
	, m_bLeaf(true)
	, m_bSelected(false)
	, m_dwData(0)
{
}

TreeNode::TreeNode(const string& _strName, DWORD_PTR _dwData)
	: m_pTreeUI(nullptr)
	, m_pParent(nullptr)
	, m_bLeaf(true)
	, m_bSelected(false)
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
	ImGuiTreeNodeFlags eFlag = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;

	if (m_bLeaf)
		eFlag |= ImGuiTreeNodeFlags_Leaf;	//�ܸ����
	if (m_pTreeUI->m_bUseFrame && (nullptr == m_pParent || m_pParent->m_strName == "DummyRoot"))
		eFlag |= ImGuiTreeNodeFlags_Framed;
	if (m_bSelected)	//���õǸ�
		eFlag |= ImGuiTreeNodeFlags_Selected;

	if (ImGui::TreeNodeEx(m_strName.c_str(), eFlag))
	{
		if (ImGui::IsItemClicked())
		{
			m_pTreeUI->SetSelectedNode(this);
		}

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
	, m_pSelectedNode(nullptr)
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
	Vec2 vSize = GetParentUI()->GetSize();
	vSize.y -= 39.f;
	SetSize(vSize);

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
	pNewNode->m_pTreeUI = this;

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


void TreeUI::SetSelectedNode(TreeNode* _pNode)
{
	if (nullptr != m_pSelectedNode)
	{
		m_pSelectedNode->m_bSelected = false;
	}

	m_pSelectedNode = _pNode;
	m_pSelectedNode->m_bSelected = true;


	// ��������Ʈ ȣ��
	if (nullptr != m_pCInst && nullptr != m_CFunc)
	{
		(m_pCInst->*m_CFunc)((DWORD_PTR)m_pSelectedNode);
	}
}
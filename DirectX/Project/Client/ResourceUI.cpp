#include "pch.h"
#include "ResourceUI.h"

#include <Engine/CResMgr.h>

#include "CImGuiMgr.h"
#include "InspectorUI.h"
#include "TreeUI.h"

ResourceUI::ResourceUI()
	: UI("Resource")
{
	m_TreeUI = new TreeUI(true);
	m_TreeUI->SetTitle("Resource");
	AddChild(m_TreeUI);

	// Clicked Delegate ���
	m_TreeUI->SetClickedDelegate(this, (CLICKED)&ResourceUI::ItemClicked);

	Reset();
}

ResourceUI::~ResourceUI()
{
}

void ResourceUI::update()
{


	UI::update();
}

void ResourceUI::render_update()
{

}

void ResourceUI::Reset()
{
	for (int i = 0; i < (int)RES_TYPE::END; ++i)
	{
		// TreeUI �� DummyRoot �� ����ϱ� ������, ���ҽ� �׸� ������ ���� �ڽ����� ��
		TreeNode* pResNode = m_TreeUI->AddTreeNode(nullptr, ToString((RES_TYPE)i));

		const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResList((RES_TYPE)i);

		for (const auto& pair : mapRes)
		{
			// �� ���ҽ� ������ �ش� ���ҽ� �׸� �ڽ����� ��
			m_TreeUI->AddTreeNode(pResNode, string(pair.first.begin(), pair.first.end()), (DWORD_PTR)pair.second);
		}
	}
}

void ResourceUI::ItemClicked(DWORD_PTR _dwNode)
{
	TreeNode* pNode = (TreeNode*)_dwNode;

	string strKey = pNode->GetName();
	CRes* pResource = (CRes*)pNode->GetData();

	// ������ ��尡 ���ȴٸ� �ƹ��ϵ� ����.
	if (nullptr == pResource)
		return;

	// InspectorUI �� ����
	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	pInspectorUI->SetTargetResource(pResource);
}


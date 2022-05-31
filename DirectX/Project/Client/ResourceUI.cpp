#include "pch.h"
#include "ResourceUI.h"

#include <Engine/CResMgr.h>

#include "TreeUI.h"

ResourceUI::ResourceUI()
	: UI("Resource")
{
	m_TreeUI = new TreeUI(true);
	m_TreeUI->SetTitle("Resource");
	AddChild(m_TreeUI);

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


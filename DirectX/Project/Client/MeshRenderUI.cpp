#include "pch.h"
#include "MeshRenderUI.h"

#include "CImGuiMgr.h"
#include "ListUI.h"

#include <Engine/CResMgr.h>
#include <Engine/CMeshRender.h>
#include <Engine/CMesh.h>
#include <Engine/CMaterial.h>

MeshRenderUI::MeshRenderUI()
	: ComponentUI("MeshRender", COMPONENT_TYPE::MESHRENDER)
{
	SetSize(Vec2(0.f, 200.f));
}

MeshRenderUI::~MeshRenderUI()
{

}

void MeshRenderUI::update()
{
	ComponentUI::update();

}

void MeshRenderUI::render_update()
{
	ComponentUI::render_update();

	ImGui::PushItemWidth(200);

	CMeshRender* pMeshRender = GetTargetObject()->MeshRender();
	Ptr<CMesh> pMesh = pMeshRender->GetMesh();
	Ptr<CMaterial> pMtrl = pMeshRender->GetMaterial();

	string strMeshName = string(pMesh->GetKey().begin(), pMesh->GetKey().end());	// string �����ڴ� 2����Ʈ�� 1����Ʈ�� �ٲ��ִ� �����ڰ� �ִ�.
	string strMtrlName = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());

	ImGui::Text("Mesh");
	ImGui::SameLine(86.f);	//SameLine(����)�� ���� ���ڸ�ŭ�� ������ �����ټ�����.
	ImGui::InputText("##MeshName", (char*)strMeshName.c_str(), strMeshName.capacity(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("##MeshListBtn", Vec2(15, 15)))
	{
		// ListUI Ȱ��ȭ�Ѵ�.
		const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResList(RES_TYPE::MESH);
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
		pListUI->Clear();
		pListUI->SetTitle("Mesh List");

		for (const auto& pair : mapRes)
		{
			pListUI->AddList(string(pair.first.begin(), pair.first.end()));
		}

		pListUI->Activate();
		pListUI->SetDBCEvent(this, (DBCLKED)&MeshRenderUI::MeshSelect);
	}


	ImGui::Text("Material");
	ImGui::SameLine(86.f);
	ImGui::InputText("##MaterialName", (char*)strMtrlName.c_str(), strMtrlName.capacity(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("##MaterialListBtn", Vec2(15, 15)))
	{
		// ListUI Ȱ��ȭ�Ѵ�.
		const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResList(RES_TYPE::MATERIAL);
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
		pListUI->Clear();
		pListUI->SetTitle("Material List");

		for (const auto& pair : mapRes)
		{
			pListUI->AddList(string(pair.first.begin(), pair.first.end()));	//���ҽ��� �̸��� 1����Ʈ ���ڿ��� �ٲٰ� �߰�
		}

		pListUI->Activate();
		pListUI->SetDBCEvent(this, (DBCLKED)&MeshRenderUI::MtrlSelect);
	}
}

void MeshRenderUI::MeshSelect(DWORD_PTR _param)	// ����Ŭ���Ҷ� ������ �̺�Ʈ �Լ�������
{
	string strSelectedName = (char*)_param;
	wstring strMeshKey = wstring(strSelectedName.begin(), strSelectedName.end());

	Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(strMeshKey);
	assert(pMesh.Get());

	CMeshRender* pMeshRender = GetTargetObject()->MeshRender();
	pMeshRender->SetMesh(pMesh);
}


void MeshRenderUI::MtrlSelect(DWORD_PTR _param)
{
	string strSelectedName = (char*)_param;	// ���ڿ�(char*)�� �޾Ƽ� string(1Byte)�� ����
	wstring strMtrlKey = wstring(strSelectedName.begin(), strSelectedName.end()); // 1Byte ���ڿ��� 2Byte ���ڿ��� ��ȯ

	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(strMtrlKey);	// ���ҽ� ã��
	assert(pMtrl.Get());

	CMeshRender* pMeshRender = GetTargetObject()->MeshRender();
	pMeshRender->SetSharedMaterial(pMtrl);
}


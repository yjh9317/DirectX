#include "pch.h"
#include "MeshRenderUI.h"

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

	ImGui::PushItemWidth(200);	//�ڽ� ����

	CMeshRender* pMeshRender = GetTargetObject()->MeshRender();
	Ptr<CMesh> pMesh = pMeshRender->GetMesh();
	Ptr<CMaterial> pMtrl = pMeshRender->GetMaterial();

	string strMeshName = string(pMesh->GetKey().begin(), pMesh->GetKey().end());	// string �����ڴ� 2����Ʈ�� 1����Ʈ�� �ٲ��ִ� �����ڰ� �ִ�.
	string strMtrlName = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());

	//SameLine(����)�� ���� ���ڸ�ŭ�� ������ �����ټ�����.
	ImGui::Text("Mesh");	ImGui::SameLine(86.f); ImGui::InputText("##MeshName", (char*)strMeshName.c_str(), strMeshName.capacity(), ImGuiInputTextFlags_ReadOnly);
	ImGui::Text("Material"); ImGui::SameLine(86.f); ImGui::InputText("##MaterialName", (char*)strMtrlName.c_str(), strMtrlName.capacity(), ImGuiInputTextFlags_ReadOnly);
}



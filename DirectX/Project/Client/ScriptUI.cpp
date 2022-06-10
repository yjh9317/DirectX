#include "pch.h"
#include "ScriptUI.h"

ScriptUI::ScriptUI()
	: UI("Script")
	, m_pTargetObject(nullptr)
	, m_pTargetScript(nullptr)
	, m_eComType(COMPONENT_TYPE::SCRIPT)
	, m_bActive(false)
{
}

ScriptUI::~ScriptUI()
{
}

void ScriptUI::update()
{

}

void ScriptUI::render_update()
{
	// ��� Script �̸�
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));

	ImGui::Button(typeid(m_pTargetScript).name());

	ImGui::PopStyleColor(3);
	ImGui::PopID();
}

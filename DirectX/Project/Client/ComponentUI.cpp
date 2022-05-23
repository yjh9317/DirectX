#include "pch.h"
#include "ComponentUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CComponent.h>

ComponentUI::ComponentUI(const string& _strName, COMPONENT_TYPE _eComType)
	: UI(_strName)
	, m_eComType(_eComType)
	, m_bActive(false)
{
}

ComponentUI::~ComponentUI()
{
}

void ComponentUI::update()
{
	// Component�� UI���� update�� �� �θ��� ComponentUI update�� ��������Ѵ�.
	if (nullptr == m_pTargetObject)	// UI�� ��� ������Ʈ�� ���ٸ� return
		return;


	CComponent* pComponent = m_pTargetObject->GetComponent(m_eComType);
	m_bActive = pComponent->IsActive();
}

void ComponentUI::render_update()
{
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
	ImGui::Button(ToString(m_eComType));	// Component Ÿ���� ���� �迭�� ���� �ε����� ���� ���ڿ��� �޾Ƽ� ����Ѵ�.
	ImGui::PopStyleColor(3);
	ImGui::PopID();
	// �����迭�� �޾� ����ϱ� ������ ComponentUI�� �ڽ�Ŭ���������� �θ���(ComponentUI)�� render_update�� ȣ�⸸ ���ָ� ������ �ȴ�.
}

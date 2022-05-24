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
	// Component의 UI들은 update할 때 부모인 ComponentUI update를 돌려줘야한다.
	if (nullptr == m_pTargetObject)	// UI로 띄울 오브젝트가 없다면 return
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
	ImGui::Button(ToString(m_eComType));	// Component 타입을 전역 배열로 만들어서 인덱스에 따른 문자열을 받아서 사용한다.
	ImGui::PopStyleColor(3);
	ImGui::PopID();
	// 전역배열로 받아 사용하기 때문에 ComponentUI의 자식클래스에서는 부모쪽(ComponentUI)의 render_update를 호출만 해주면 세팅이 된다.


	// Component 활성화 체크
	CComponent* pComponent = m_pTargetObject->GetComponent(m_eComType);

	if (pComponent->GetType() != COMPONENT_TYPE::TRANSFORM)	//Transform은 항상 Active상태여야 하므로 제외
	{
		bool IsActive = pComponent->IsActive();
		ImGui::SameLine();
		ImGui::Checkbox("##ComponentActive", &IsActive);

		if (pComponent->IsActive() != IsActive)
		{
			if (IsActive)
				pComponent->Activate();
			else
				pComponent->Deactivate();
		}
	}
}

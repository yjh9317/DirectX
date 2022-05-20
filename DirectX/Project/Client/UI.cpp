#include "pch.h"
#include "UI.h"


UI::UI(const string& _strName)
	: m_strName(_strName)
	, m_pParentUI(nullptr)
{
}

UI::~UI()
{
}

void UI::render()
{
	if (nullptr == m_pParentUI)	// 만약 부모 UI가 없다면 이 UI는 최상위 UI이므로 Begin로 시작
	{
		ImGui::Begin(m_strName.c_str());

		render_update();		// UI자식클래스에서 오버라이딩한 render_update

		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->render();
		}

		ImGui::End();
	}

	else                          // 부모 UI가 있다면 이 UI는 자식UI이므로 BeginChild로 시작
	{
		ImGui::BeginChild(m_strName.c_str());

		render_update();

		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->render();
		}

		ImGui::EndChild();
	}
}


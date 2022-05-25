#include "pch.h"
#include "UI.h"



UI::UI(const string& _strName)
	: m_strName(_strName)
	, m_pParentUI(nullptr)
	, m_bOpen(true)
{
}

UI::~UI()
{
	Safe_Del_Vec(m_vecChildUI);
}

void UI::render()
{
	if (!(m_vPos.x == 0.f && m_vPos.y == 0.f))	// 설정된 위치가 아니라면 고정된 위치에 UI가 뜬다
	{
		ImGui::SetNextWindowPos(m_vPos, ImGuiCond_Always);
	}

	if (nullptr == m_pParentUI)
	{
		if (m_bOpen)		// ImGui창의 x버튼을 누른다면 m_bOpen값이 false로 변환.
		{
			string strName = m_strTitle + m_strName;
			ImGui::Begin(strName.c_str(), &m_bOpen);

			// 사이즈 재확인
			ImVec2 vSize = ImGui::GetWindowSize();
			m_vSize.x = vSize.x;
			m_vSize.y = vSize.y;

			render_update();

			for (size_t i = 0; i < m_vecChildUI.size(); ++i)
			{
				m_vecChildUI[i]->render();
				ImGui::Separator();	//한줄이 나타나는 함수
			}

			ImGui::End();
		}
	}

	else
	{
		if (m_bOpen)
		{
			ImGui::BeginChild(m_strName.c_str(), m_vSize);

			// 사이즈 재확인
			ImVec2 vSize = ImGui::GetWindowSize();
			m_vSize.x = vSize.x;
			m_vSize.y = vSize.y;

			render_update();

			for (size_t i = 0; i < m_vecChildUI.size(); ++i)
			{
				m_vecChildUI[i]->render();
				ImGui::Separator();
			}

			ImGui::EndChild();
		}
	}
}




Vec2::operator ImVec2() const
{
	return ImVec2(x, y);
}

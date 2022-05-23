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
	if (nullptr == m_pParentUI)
	{
		if (m_bOpen)	// ImGuiâ�� x��ư�� �����ٸ� m_bOpen���� false�� ��ȯ.
		{
			ImGui::Begin(m_strName.c_str(), &m_bOpen);

			render_update();

			for (size_t i = 0; i < m_vecChildUI.size(); ++i)
			{
				m_vecChildUI[i]->render();
				ImGui::Separator();	//������ ��Ÿ���� �Լ�
			}

			ImGui::End();
		}
	}

	else
	{
		if (m_bOpen)
		{
			ImGui::BeginChild(m_strName.c_str(), m_vSize);

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

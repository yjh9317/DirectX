#include "pch.h"
#include "TreeUI.h"


TreeUI::TreeUI()
	: UI("##TreeUI")
{
}

TreeUI::~TreeUI()
{
}

void TreeUI::update()
{
}

void TreeUI::render_update()
{
	if (ImGui::TreeNodeEx("ParentNode", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected))
	{



		ImGui::TreePop();
	}
}

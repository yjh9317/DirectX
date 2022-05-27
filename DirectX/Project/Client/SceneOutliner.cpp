#include "pch.h"
#include "SceneOutliner.h"

#include "TreeUI.h"

SceneOutliner::SceneOutliner()
	: UI("SceneOutliner")
{
	m_TreeUI = new TreeUI;					 // �����ڿ��� Ʈ��UI ����
	m_TreeUI->SetTitle("SceneOutliner");	 // Ʈ��UI�� Ÿ��Ʋ
	AddChild(m_TreeUI);						 // SceneOutlinerUI�� TreeUI
}

SceneOutliner::~SceneOutliner()
{
}



void SceneOutliner::update()
{
}

void SceneOutliner::render_update()
{
}

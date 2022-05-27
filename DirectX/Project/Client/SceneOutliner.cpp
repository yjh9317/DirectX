#include "pch.h"
#include "SceneOutliner.h"

#include "TreeUI.h"

SceneOutliner::SceneOutliner()
	: UI("SceneOutliner")
{
	m_TreeUI = new TreeUI;					 // 생성자에서 트리UI 생성
	m_TreeUI->SetTitle("SceneOutliner");	 // 트리UI의 타이틀
	AddChild(m_TreeUI);						 // SceneOutlinerUI의 TreeUI
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

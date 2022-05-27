#include "pch.h"
#include "ResourceUI.h"

#include "TreeUI.h"

ResourceUI::ResourceUI()
	: UI("Resource")
{
	m_TreeUI = new TreeUI;			// 생성자에서 트리UI 생성
	m_TreeUI->SetTitle("Resource");	// 트리UI의 타이틀
	AddChild(m_TreeUI);				// ResourceUI의 TreeUI
}

ResourceUI::~ResourceUI()
{
}

void ResourceUI::update()
{
	m_TreeUI;
}

void ResourceUI::render_update()
{
}


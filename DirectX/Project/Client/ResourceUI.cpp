#include "pch.h"
#include "ResourceUI.h"

#include "TreeUI.h"

ResourceUI::ResourceUI()
	: UI("Resource")
{
	m_TreeUI = new TreeUI;			// �����ڿ��� Ʈ��UI ����
	m_TreeUI->SetTitle("Resource");	// Ʈ��UI�� Ÿ��Ʋ
	AddChild(m_TreeUI);				// ResourceUI�� TreeUI
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


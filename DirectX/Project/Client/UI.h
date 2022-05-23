#pragma once


#include "ImGui/imgui.h"

class UI
{
private:
	const string	m_strName;		// UI는 각자 Name을 받아 이름이자 키값으로 사용
	vector<UI*>		m_vecChildUI;
	UI* m_pParentUI;

	Vec2			m_vSize;
	bool			m_bOpen;

public:
	const string& GetName() { return m_strName; }
	void SetSize(Vec2 _vSize) { m_vSize = _vSize; }

	void AddChild(UI* _pChildUI)
	{
		_pChildUI->m_pParentUI = this;
		m_vecChildUI.push_back(_pChildUI);
	}

	void Activate() { m_bOpen = true; }
	void Deactivate() { m_bOpen = false; };

	bool IsActive() { return m_bOpen; }


public:
	virtual void update() = 0;
	void render();
	virtual void render_update() = 0;

public:
	UI(const string& _strName);
	~UI();
};


#pragma once


#include "ImGui/imgui.h"

class UI
{
private:
	const string	m_strName;		// UI는 각자 Name을 받아 이름이자 키값으로 사용
	string			m_strTitle;		// UI에서 사용할 제목
	vector<UI*>		m_vecChildUI;
	UI*				m_pParentUI;

	Vec2			m_vPos;
	Vec2			m_vSize;
	bool			m_bOpen;	// 창이 열리는지
	bool			m_bModal;	// 모달 체크

public:
	const string& GetName() { return m_strName; }
	void SetSize(Vec2 _vSize) { m_vSize = _vSize; }
	Vec2 GetSize() { return m_vSize; }

	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	Vec2 GetPos() { return m_vPos; }

	void SetTitle(const string& _strTitle) { m_strTitle = _strTitle; }
	const string& GetTitle() { return m_strTitle; }

	void SetModalUI(bool _bTrue) { m_bModal = _bTrue; }
	bool IsModal() { return m_bModal; }

	UI* GetParentUI() { return m_pParentUI; }


	void AddChild(UI* _pChildUI)
	{
		_pChildUI->m_pParentUI = this;
		m_vecChildUI.push_back(_pChildUI);
	}

	virtual void Activate() { m_bOpen = true; }
	virtual void Deactivate() { m_bOpen = false; };

	bool IsActive() { return m_bOpen; }

	virtual void DropCheck() {};

public:
	virtual void update();
	virtual void render();
	virtual void render_update() = 0;

public:
	UI(const string& _strName);
	virtual ~UI();
};


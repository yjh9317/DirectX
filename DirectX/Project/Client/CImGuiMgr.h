#pragma once

#include "UI.h"

typedef void (UI::* PARAM_1)(DWORD_PTR);

struct tUIDelegate
{
	UI* pInst;
	PARAM_1		pFunc;
	DWORD_PTR	dwParam;
};

class CImGuiMgr
	: public CSingleton<CImGuiMgr>
{
	SINGLE(CImGuiMgr)
private:
	map<string, UI*>	m_mapUI;
	vector<tUIDelegate>	m_vecDelegate;	// Delegate Event를 모아놓는 벡터

	HANDLE				m_hNotify;		// 파일의 변경을 감지하기 위한 핸들

public:
	void init(HWND _hwnd);
	void progress();
	void render();
	void clear();

private:
	void CreateUI();
	void ObserveContent();

public:
	UI* FindUI(const string& _strKey);
	void AddDelegate(tUIDelegate _del) { m_vecDelegate.push_back(_del); }
};


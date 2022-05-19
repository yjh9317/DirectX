#pragma once

class CImGuiMgr
	: public CSingleton<CImGuiMgr>
{
private:

public:
	void init(HWND _hwnd);
	void progress();
	void render();
	void clear();
};


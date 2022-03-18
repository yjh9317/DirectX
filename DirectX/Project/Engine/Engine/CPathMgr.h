#pragma once
class CPathMgr
{
	SINGLE(CPathMgr);
private:
	wchar_t m_strContentPath[256];

public:
	void init();


public:
	const wchar_t* GetContentPath() { return m_strContentPath; }


};


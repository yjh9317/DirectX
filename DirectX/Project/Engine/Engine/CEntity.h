#pragma once


class CEntity	//모든 오브젝트의 상위클래스
{
private:
	static UINT g_iNextID;

private:
	const UINT	m_iID;
	wstring		m_strName;

public:
	void SetName(const wstring& _name) { m_strName = _name; }
	const wstring& GetName() { return m_strName; }
	UINT GetID() { return m_iID; }

public:
	virtual void UpdateData() {}	//파이프라인에 전달

public:
	CEntity();
	CEntity(const CEntity& _origin);
	virtual ~CEntity();
};


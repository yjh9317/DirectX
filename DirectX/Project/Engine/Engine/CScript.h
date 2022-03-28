#pragma once
#include "CComponent.h"

//컴포넌트 기반이지만 오브젝트에 따라 다르게 사용할 수 있도록 상속형태
// ex PlayerScript, MonsterScript

class CScript :         //모든 스크립트의 부모
  public CComponent
{
private:
	const int	m_iScriptID;		// 스크립트 구별용도(스크립트 타입)
	
public:
	virtual void start() {}
	virtual void update() {}	
	virtual void lateupdate() {}
	virtual void finalupdate() final {}	// 더이상 상속되지 않도록 final


public:
	CScript();
	~CScript();
};


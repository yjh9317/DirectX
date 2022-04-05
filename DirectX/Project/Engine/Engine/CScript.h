#pragma once
#include "CComponent.h"

#include "CTransform.h"
#include "CMeshRender.h"

#include "CResMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CLayer.h"

//������Ʈ ��������� ������Ʈ�� ���� �ٸ��� ����� �� �ֵ��� �������
// ex PlayerScript, MonsterScript

class CScript :         //��� ��ũ��Ʈ�� �θ�
	public CComponent
{
private:
	const int	m_iScriptID;		// ��ũ��Ʈ �����뵵(��ũ��Ʈ Ÿ��)

public:
	virtual void start() {}
	virtual void update() {}
	virtual void lateupdate() {}
	virtual void finalupdate() final {}	// ���̻� ��ӵ��� �ʵ��� final

	CScript* Clone() = 0;

public:
	CScript();
	~CScript();
};


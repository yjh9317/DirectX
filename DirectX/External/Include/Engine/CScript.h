#pragma once
#include "CComponent.h"

//������Ʈ ��������� ������Ʈ�� ���� �ٸ��� ����� �� �ֵ��� ���

class CScript :         //��� ��ũ��Ʈ�� �θ�
  public CComponent
{
private:
	const int	m_iScriptID;		// ��ũ��Ʈ �����뵵(��ũ��Ʈ Ÿ��)
	
public:
	virtual void start() {}
	virtual void update() {}		//GameObject�� �����Ǿ� ������
	virtual void lateupdate() {}
	virtual void finalupdate() final {}	// ���̻� ��ӵ��� �ʵ��� final


public:
	CScript();
	~CScript();
};


#pragma once

class CScene;

class CSceneMgr			//��(��������)�� �����ϴ� �Ŵ���
{
	SINGLE(CSceneMgr)
private:
	CScene* m_pCurScene;

public:
	void init();
	void progress();
	void render();



};


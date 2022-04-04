#pragma once

class CScene;

class CSceneMgr			//��(��������)�� �����ϴ� �Ŵ���
	: public CSingleton<CSceneMgr>
{
	SINGLE(CSceneMgr)

private:
	CScene* m_pCurScene;

public:
	CScene* GetCurScene() { return m_pCurScene; }

public:
	void init();
	void progress();
	void render();



};


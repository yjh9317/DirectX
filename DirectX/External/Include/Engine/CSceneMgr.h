#pragma once

class CScene;

class CSceneMgr			//씬(스테이지)를 관리하는 매니저
{
	SINGLE(CSceneMgr)
private:
	CScene* m_pCurScene;

public:
	void init();
	void progress();
	void render();



};


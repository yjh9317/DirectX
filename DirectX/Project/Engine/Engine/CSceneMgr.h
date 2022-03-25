#pragma once

class CScene;

class CSceneMgr
{
	SINGLE(CSceneMgr)
private:
	CScene* m_pCurScene;

public:
	void init();
	void progress();
	void render();



};


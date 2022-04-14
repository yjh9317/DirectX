#pragma once
#include "singleton.h"

class CCamera;

class CRenderMgr :
	 public CSingleton<CRenderMgr>
{
	SINGLE(CRenderMgr);
private:
	vector<CCamera*>	m_vecCam;		// Scene�� �ִ� Cameara ��, ù��° ī�޶� ����
	CCamera*			m_pEditorCam;	// Editor ���� ī�޶�


public:
	void init();
	void update();
	void render();

public:
	void RegisterCamera(CCamera* _pCam);
	void ClearCamera() { m_vecCam.clear(); }
	void SwapCameraIndex(CCamera* _pCam, int _iChangeIdx); // _pCam�� Idx�� �ش��ϴ� ī�޶� Swap
};


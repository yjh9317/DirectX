#pragma once
#include "singleton.h"

class CCamera;

class CRenderMgr :
	 public CSingleton<CRenderMgr>
{
	SINGLE(CRenderMgr);
private:
	vector<CCamera*>	m_vecCam;		// Scene에 있는 Cameara 들, 첫번째 카메라가 메인
	CCamera*			m_pEditorCam;	// Editor 시점 카메라


public:
	void init();
	void update();
	void render();

public:
	void RegisterCamera(CCamera* _pCam);
	void ClearCamera() { m_vecCam.clear(); }
	void SwapCameraIndex(CCamera* _pCam, int _iChangeIdx); // _pCam과 Idx에 해당하는 카메라 Swap
};


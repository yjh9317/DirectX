#pragma once
#include "singleton.h"

class CCamera;
class CLight2D;

class CRenderMgr :
	public CSingleton<CRenderMgr>
{
	SINGLE(CRenderMgr);
private:
	vector<CCamera*>	m_vecCam;		// Scene에 있는 Cameara 들, 첫번째 카메라가 메인
	CCamera*			m_pEditorCam;	// Editor 시점 카메라

	vector<CLight2D*>   m_vecLight2D;		// 하나의 물체가 여러개의 광원한테 동시에 영향을 받을 수 있으므로 Shader는 모든 광원을 알고있어야 하므로 벡터로 관리
	CStructuredBuffer* m_pLight2DBuffer;    // Scene 의 2D 광원 버퍼

	//	Scene의 상태가 Stop일 때 메인카메라(m_vecCam)도 Scene에 들어가있는 오브젝트이므로 이 카메라로 렌더링하는건 어긋남.
	//	그러므로 Tool카메라는 Scene에 추가하지않고 별도로 logic을 돌려서 사용
	//	m_pEditorCam는 CRenderMgr에 세팅만 하고 Client에서 카메라를 생성해서 링크하고 사용하는 방식을 할 예정

public:
	void init();
	void update();
	void render();



public:
	void RegisterCamera(CCamera* _pCam);
	void ClearCamera() { m_vecCam.clear(); }
	void SwapCameraIndex(CCamera* _pCam, int _iChangeIdx); // _pCam과 Idx에 해당하는 카메라 Swap
	void CopyTargetToPostProcess();

	int RegisterLight2D(CLight2D* _pLight2D)
	{
		m_vecLight2D.push_back(_pLight2D);
		return (int)m_vecLight2D.size() - 1;
	}
private:
	void UpdateLight2D();
};


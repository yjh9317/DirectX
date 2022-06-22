#pragma once
#include "singleton.h"

class CCamera;
class CLight2D;

class CRenderMgr :
	public CSingleton<CRenderMgr>
{
	SINGLE(CRenderMgr);
private:
	vector<CCamera*>	m_vecCam;		// Scene�� �ִ� Cameara ��, ù��° ī�޶� ����
	CCamera*			m_pEditorCam;	// Editor ���� ī�޶�

	vector<CLight2D*>   m_vecLight2D;		// �ϳ��� ��ü�� �������� �������� ���ÿ� ������ ���� �� �����Ƿ� Shader�� ��� ������ �˰��־�� �ϹǷ� ���ͷ� ����
	CStructuredBuffer* m_pLight2DBuffer;    // Scene �� 2D ���� ����

	//	Scene�� ���°� Stop�� �� ����ī�޶�(m_vecCam)�� Scene�� ���ִ� ������Ʈ�̹Ƿ� �� ī�޶�� �������ϴ°� ��߳�.
	//	�׷��Ƿ� Toolī�޶�� Scene�� �߰������ʰ� ������ logic�� ������ ���
	//	m_pEditorCam�� CRenderMgr�� ���ø� �ϰ� Client���� ī�޶� �����ؼ� ��ũ�ϰ� ����ϴ� ����� �� ����

public:
	void init();
	void update();
	void render();



public:
	void RegisterCamera(CCamera* _pCam);
	void ClearCamera() { m_vecCam.clear(); }
	void SwapCameraIndex(CCamera* _pCam, int _iChangeIdx); // _pCam�� Idx�� �ش��ϴ� ī�޶� Swap
	void CopyTargetToPostProcess();

	int RegisterLight2D(CLight2D* _pLight2D)
	{
		m_vecLight2D.push_back(_pLight2D);
		return (int)m_vecLight2D.size() - 1;
	}
private:
	void UpdateLight2D();
};


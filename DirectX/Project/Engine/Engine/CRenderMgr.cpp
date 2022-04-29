#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CCamera.h"


CRenderMgr::CRenderMgr()
	: m_pEditorCam(nullptr)
{

}

CRenderMgr::~CRenderMgr()
{

}

void CRenderMgr::init()
{

}

void CRenderMgr::update()
{

}

void CRenderMgr::render()
{
	if (m_vecCam.empty())
		return;

	// Global ��� ������Ʈ
	static CConstBuffer* pGlobalCB = CDevice::GetInst()->GetCB(CB_TYPE::GLOBAL);
	pGlobalCB->SetData(&g_global, sizeof(tGlobal));
	pGlobalCB->UpdateData();
	pGlobalCB->UpdateData_CS();

	// Rendering ����
	CDevice::GetInst()->ClearTarget();

	// ���� ī�޶� �������� ������
	CCamera* pMainCam = m_vecCam[0];

	// Camera �� ��� Layer �� ������Ʈ���� Shader Domain �� ���� �з��ص�
	pMainCam->SortGameObject();

	g_transform.matView = pMainCam->GetViewMat();
	g_transform.matProj = pMainCam->GetProjMat();

	// Foward ��ü ������
	pMainCam->render_forward();

	// Masked ��ü ������
	pMainCam->render_masked();

	// Alpha ��ü ������
	pMainCam->render_opaque();


	// Sub ī�޶� �������� ������
	for (int i = 1; i < m_vecCam.size(); ++i)
	{
		if (nullptr == m_vecCam[i])
			continue;

		m_vecCam[i]->SortGameObject();

		g_transform.matView = m_vecCam[i]->GetViewMat();
		g_transform.matProj = m_vecCam[i]->GetProjMat();

		// Foward ��ü ������
		m_vecCam[i]->render_forward();

		// Masked ��ü ������
		m_vecCam[i]->render_masked();

		// Alpha ��ü ������
		m_vecCam[i]->render_opaque();
	}



	CDevice::GetInst()->Present();
}

void CRenderMgr::RegisterCamera(CCamera* _pCam)
{
	// ī�޶� RenderMgr�� ���� ��� �Ǵ� ���
	if (-1 == _pCam->m_iCamIdx)
	{
		// ī�޶� ���
		m_vecCam.push_back(_pCam);

		// Ǫ������ ī�޶��� �ε����� ����
		int iIdx = (int)m_vecCam.size() - 1;
		_pCam->m_iCamIdx = iIdx;
	}
	// ��ϵ� ���� �ִٸ�
	else
	{
		// ������ ũ�Ⱑ �ε������� �۴ٸ� resize
		if (m_vecCam.size() <= _pCam->m_iCamIdx)
		{
			m_vecCam.resize((size_t)_pCam->m_iCamIdx + 1);
		}

		m_vecCam[_pCam->m_iCamIdx] = _pCam;
	}
}

void CRenderMgr::SwapCameraIndex(CCamera* _pCam, int _iChangeIdx)
{
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		if (_pCam == m_vecCam[i])
		{
			if (nullptr != m_vecCam[_iChangeIdx])
			{
				m_vecCam[_iChangeIdx]->m_iCamIdx = (int)i;
				_pCam->m_iCamIdx = _iChangeIdx;

				return;
			}
		}
	}

	assert(nullptr);
}

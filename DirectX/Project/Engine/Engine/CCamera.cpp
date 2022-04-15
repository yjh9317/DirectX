#include "pch.h"
#include "CCamera.h"

#include "CDevice.h"
#include "CRenderMgr.h"
#include "CEventMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"

#include "CResMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"


CCamera::CCamera()
	:CComponent(COMPONENT_TYPE::CAMERA)
	, m_eProjType(PROJ_TYPE::ORTHOGRAPHIC)
	, m_fWidth(0.f)
	, m_fAspectRatio(1.f)
	, m_fFOV(XM_PI/4.f)
	, m_fFar(10000.f)
	, m_iLayerMask(0)
	, m_iCamIdx(-1)
{
	m_fWidth = CDevice::GetInst()->GetRenderResolution().x;
	m_fAspectRatio = CDevice::GetInst()->GetRenderResolution().x / CDevice::GetInst()->GetRenderResolution().y;
}

CCamera::CCamera(const CCamera& _origin)
	:CComponent(_origin)
	, m_eProjType(_origin.m_eProjType)
	, m_fWidth(_origin.m_fWidth)
	, m_fAspectRatio(_origin.m_fAspectRatio)
	, m_fFOV(_origin.m_fFOV)
	, m_fFar(_origin.m_fFar)
	, m_iLayerMask(_origin.m_iLayerMask)
	, m_iCamIdx(-1)
{
}

CCamera::~CCamera()
{
}

void CCamera::finalupdate()
{

	// m_fWidth�� �̿��Ͽ� ��Ⱦ��,����,���θ� �������� ī�޶�� �����̴� ȿ�� ����(CameraScript���� ����)
	
	//world�� �ִ� ��ü���� view space�� ������ ��ȯ����� ���ϴ� �۾�

	// View ��� ���

	Vec3 vCamPos = Transform()->GetPos(); //ī�޶� ������Ʈ�� ��ǥ
	// ī�޶� ������ �ǵ��� ī�޶� �̵��� ��ŭ �ٸ� ������Ʈ�鵵 ī�޶�� ���� �������� �̵��ؾ� �ȴ�.
	m_matView = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);


	// ���� ��� ���

	// ��������
	if (PROJ_TYPE::ORTHOGRAPHIC == m_eProjType) {
		// XMMatrixOrthographicLH�� ���� ���, �������� ����,����,z��ǥ ó�� ,z��ǥ ��
		float fHeight = m_fWidth / m_fAspectRatio; // ����
		m_matProj = XMMatrixOrthographicLH(m_fWidth, fHeight, 0.f, 5000.f);
	}
	// ��������
	else
	{
		m_matProj = XMMatrixPerspectiveFovLH(m_fFOV,m_fAspectRatio,1.f,m_fFar); //�޼ձ��� ,�þ߰�,��Ⱦ�� ,near ,far
	}
	

	g_transform.matView = m_matView;
	g_transform.matProj = m_matProj;


	CRenderMgr::GetInst()->RegisterCamera(this);
}



void CCamera::SortGameObject()
{
	m_vecForward.clear();
	m_vecMasked.clear();
	m_vecOpaque.clear();

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// ī�޶� ���� ��� ���̾ �ƴϸ� continue
		if (!(m_iLayerMask & (1 << i)))
			continue;

		CLayer* pLayer = pCurScene->GetLayer(i);
		vector<CGameObject*>& vecObj = pLayer->GetObjects();

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			CMeshRender* pMeshRender = vecObj[j]->MeshRender();

			// MeshRender�� ���� ���,Mesh�� ������ ���� �������� ���� ���,���͸����� ���°��, ���̴��� ���°��
			if (nullptr == pMeshRender 
				|| nullptr == pMeshRender->GetMesh()
				|| nullptr == pMeshRender->GetMaterial()
				|| nullptr == pMeshRender->GetMaterial()->GetShader())
			{
				continue;
			}

			Ptr<CGraphicsShader>pShader = pMeshRender->GetMaterial()->GetShader();

			switch (pShader->GetShaderDomain())
			{
			case SHADER_DOMAIN::DOMAIN_FORWARD:
				m_vecForward.push_back(vecObj[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_MASKED:
				m_vecMasked.push_back(vecObj[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_OPAQUE:
				m_vecOpaque.push_back(vecObj[j]);
				break;
			}
			
		}
	}

}

void CCamera::render_forward()
{
	for (size_t i = 0; i < m_vecForward.size(); ++i)
	{
		m_vecForward[i]->render();
	}
}

void CCamera::render_masked()
{
	for (size_t i = 0; i < m_vecMasked.size(); ++i)
	{
		m_vecMasked[i]->render();
	}
}

void CCamera::render_opaque()
{
	for (size_t i = 0; i < m_vecOpaque.size(); ++i)
	{
		m_vecOpaque[i]->render();
	}
}



void CCamera::SetCameraAsMain()
{
	tEventInfo tEvent = {};
	tEvent.eType = EVENT_TYPE::SET_CAMERA_INDEX;
	tEvent.lParam = (DWORD_PTR)this;
	tEvent.wParam = 0;
	CEventMgr::GetInst()->AddEvent(tEvent);
}


void CCamera::CheckLayerMask(int _iLayerIdx)
{
	if (m_iLayerMask & 1 << _iLayerIdx)
	{
		m_iLayerMask &= ~(1 << _iLayerIdx);
	}
	else
	{
		m_iLayerMask |= 1 << _iLayerIdx;
	}
}

void CCamera::CheckLayerMask(const wstring& _strLayerName)
{
	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pScene->GetLayer(_strLayerName);
	CheckLayerMask(pLayer->GetLayerIdx());
}

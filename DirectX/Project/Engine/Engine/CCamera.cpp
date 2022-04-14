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


CCamera::CCamera()
	:CComponent(COMPONENT_TYPE::CAMERA)
	, m_iLayerMask(0)
	, m_iCamIdx(-1)
{
}

CCamera::CCamera(const CCamera& _origin)
	:CComponent(_origin)
	,m_iLayerMask(_origin.m_iLayerMask)
	,m_iCamIdx(-1)
{
}

CCamera::~CCamera()
{
}

void CCamera::finalupdate()
{
	//world�� �ִ� ��ü���� view space�� ������ ��ȯ����� ���ϴ� �۾�

	// View ��� ���

	Vec3 vCamPos = Transform()->GetPos(); //ī�޶� ������Ʈ�� ��ǥ
	// ī�޶� ������ �ǵ��� ī�޶� �̵��� ��ŭ �ٸ� ������Ʈ���� �ݴ����(����)�� �̵��ؾ� �ȴ�.
	m_matView = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);


	// ���� ��� ���

	Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();
	//XMMatrixOrthographicLH�� ���� ���, �������� ����,����,z��ǥ ó�� ,z��ǥ ��
	m_matProj = XMMatrixOrthographicLH(vRenderResolution.x, vRenderResolution.y, 0.f, 5000.f);

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

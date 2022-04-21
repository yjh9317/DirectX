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
#include "CRenderComponent.h"

#include "CResMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"



CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_eProjType(PROJ_TYPE::ORTHOGRAPHIC)
	, m_fWidth(0.f)
	, m_fAspectRatio(1.f)
	, m_fFOV(XM_PI / 4.f)
	, m_fFar(10000.f)
	, m_iLayerMask(0)
	, m_iCamIdx(-1)
{
	m_fWidth = CDevice::GetInst()->GetRenderResolution().x;
	m_fAspectRatio = CDevice::GetInst()->GetRenderResolution().x / CDevice::GetInst()->GetRenderResolution().y;
}

CCamera::CCamera(const CCamera& _origin)
	: CComponent(_origin)
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

	// m_fWidth를 이용하여 종횡비,가로,세로를 움직여서 카메라로 움직이는 효과 가능(CameraScript에서 구현)

	// world에 있는 물체들이 view space로 오도록 변환행렬을 구하는 작업


	Vec3 vCamPos = Transform()->GetRelativePos(); //카메라 오브젝트의 좌표

	// 카메라가 원점이 되도록 카메라가 이동한 만큼 다른 오브젝트들도 카메라와 같은 방향으로 이동해야 된다.
	// View 이동 행렬
	Matrix matViewTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);

	// View Space에서는 카메라의 방향은 전방방향이다.
	// 카메라의 오른쪽, 위, 앞 방향 행렬에 곱해서 단위행렬로 만들면 되는데
	// 카메라 회전 행렬에 역행렬을 곱하면 단위 행렬이 된다.
	// 그런데 각 x,y,z축은 서로 수직이므로 단위벡터에서 역행렬을 곱하면 같은 성분끼리의 곱말고는 cos90도 이므로 0이된다.

	// View 회전 행렬	
	Matrix matViewRot = XMMatrixIdentity(); //단위행렬

	// Right, Up, Front를 가져온다.
	Vec3 vRight = Transform()->GetWorldRightDir();
	Vec3 vUp = Transform()->GetWorldUpDir();
	Vec3 vFront = Transform()->GetWorldFrontDir();

	matViewRot._11 = vRight.x;	matViewRot._12 = vUp.x; matViewRot._13 = vFront.x;
	matViewRot._21 = vRight.y;	matViewRot._22 = vUp.y;	matViewRot._23 = vFront.y;
	matViewRot._31 = vRight.z;	matViewRot._32 = vUp.z;	matViewRot._33 = vFront.z;

	// 이동 후 회전 (순서 중요) ,카메라와 같이 View 스페이스로 이동하고 난후 회전
	m_matView = matViewTrans * matViewRot;

	// r= right, u = up, f= front , T=transform

	// ( 1, 0, 0, 0)			( r.x,	u.x,  f.x,  0)				( r.x  u.x,  f.x,   0)
	// ( 0, 1, 0, 0)     *		( r.y,	u.y,  f.y,  0)				( r.y  u.y,  f.y,   0)
	// ( 0, 0, 1, 0)			( r.z,	u.z,  f.z,  0)		=		( r.z  u.z,  f.z,   0)
	// ( -x,-y,-z,1)			(  0,	 0,	   0,   1)				(-T*R , -T*U, -T*F ,1)


	// 직교투영
	if (PROJ_TYPE::ORTHOGRAPHIC == m_eProjType) {
		// XMMatrixOrthographicLH는 직영 행렬, 윈도우의 가로,세로,z좌표 처음 ,z좌표 끝
		float fHeight = m_fWidth / m_fAspectRatio; // 세로
		m_matProj = XMMatrixOrthographicLH(m_fWidth, fHeight, 0.f, 5000.f);
	}
	// 원근투영
	else
	{
		m_matProj = XMMatrixPerspectiveFovLH(m_fFOV, m_fAspectRatio, 1.f, m_fFar); //왼손기준 ,시야각,종횡비 ,near ,far
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
		// 카메라가 찍을 대상 레이어가 아니면 continue
		if (!(m_iLayerMask & (1 << i)))
			continue;

		CLayer* pLayer = pCurScene->GetLayer(i);
		vector<CGameObject*>& vecObj = pLayer->GetObjects();

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			CRenderComponent* pRenderCom = vecObj[j]->GetRenderComponent();

			// MeshRender가 없는 경우,Mesh는 있지만 아직 참조하지 않은 경우,매터리얼이 없는경우, 쉐이더가 없는경우
			if (nullptr == pRenderCom
				|| nullptr == pRenderCom->GetMesh()
				|| nullptr == pRenderCom->GetMaterial()
				|| nullptr == pRenderCom->GetMaterial()->GetShader())
			{
				continue;
			}

			Ptr<CGraphicsShader>pShader = pRenderCom->GetMaterial()->GetShader();

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
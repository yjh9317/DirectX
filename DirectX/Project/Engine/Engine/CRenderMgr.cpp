#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CCamera.h"
#include "CResMgr.h"
#include "CLight2D.h"

#include "CStructuredBuffer.h"
#include "CSceneMgr.h"
#include "CScene.h"

CRenderMgr::CRenderMgr()
	: m_pEditorCam(nullptr)
	, m_pLight2DBuffer(nullptr)
{
	m_pLight2DBuffer = new CStructuredBuffer;
	m_pLight2DBuffer->Create(sizeof(tLightInfo), 2, SB_TYPE::READ_ONLY, true, nullptr);
}

CRenderMgr::~CRenderMgr()
{

}


void CRenderMgr::update()
{

}

void CRenderMgr::render()
{
	// Rendering 시작
	// ImGui가 윈도우창 밖으로 나가면 RenderTarget이 새로운 윈도우로 이동하면서 현재 윈도우에 화면이 들어오지 않는다.
	// 이유는 Device가 초기화하면서 OMsetRenderTarget으로 현재 윈도우에 화면을 출력했었는데 ImGui의 새로운 윈도우쪽의 렌더타겟이 생성되면서 그쪽에다가 render하기 때문
	// 그래서 render전에 SetRenderTarget으로 Engine Device의 RenderTarget으로 설정해야 Engine device의 RenderTarget에 들어옴
	CDevice::GetInst()->SetRenderTarget();
	CDevice::GetInst()->ClearTarget();

	// Light 업데이트
	UpdateLight2D();

	// Global 상수 업데이트
	static CConstBuffer* pGlobalCB = CDevice::GetInst()->GetCB(CB_TYPE::GLOBAL);
	pGlobalCB->SetData(&g_global, sizeof(tGlobal));
	pGlobalCB->UpdateData();
	pGlobalCB->UpdateData_CS();

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	if (pCurScene->GetSceneState() == SCENE_STATE::PLAY)
	{
		render_play();
	}

	else
	{
		render_editor();
	}
}
void CRenderMgr::render_play()
{
	if (m_vecCam.empty())
		return;

	// 메인 카메라 시점으로 렌더링
	CCamera* pMainCam = m_vecCam[0];

	// Camera 가 찍는 Layer 의 오브젝트들을 Shader Domain 에 따라 분류해둠
	pMainCam->SortGameObject();

	g_transform.matView = pMainCam->GetViewMat();
	g_transform.matProj = pMainCam->GetProjMat();

	// Foward 물체 렌더링
	pMainCam->render_forward();

	// Masked 물체 렌더링
	pMainCam->render_masked();

	// Alpha 물체 렌더링
	pMainCam->render_translucent();

	// PostProcess 물체 렌더링
	pMainCam->render_postprocess();


	// Sub 카메라 시점으로 렌더링
	for (int i = 1; i < m_vecCam.size(); ++i)
	{
		if (nullptr == m_vecCam[i])
			continue;

		m_vecCam[i]->SortGameObject();

		g_transform.matView = m_vecCam[i]->GetViewMat();
		g_transform.matProj = m_vecCam[i]->GetProjMat();

		// Foward 물체 렌더링
		m_vecCam[i]->render_forward();

		// Masked 물체 렌더링
		m_vecCam[i]->render_masked();

		// Alpha 물체 렌더링
		m_vecCam[i]->render_translucent();
	}
}

void CRenderMgr::render_editor()
{
	if (nullptr == m_pEditorCam)
		return;

	// 에디터 카메라 시점으로 렌더링
	// Camera 가 찍는 Layer 의 오브젝트들을 Shader Domain 에 따라 분류해둠
	m_pEditorCam->SortGameObject();

	g_transform.matView = m_pEditorCam->GetViewMat();
	g_transform.matProj = m_pEditorCam->GetProjMat();

	// Foward 물체 렌더링
	m_pEditorCam->render_forward();

	// Masked 물체 렌더링
	m_pEditorCam->render_masked();

	// Alpha 물체 렌더링
	m_pEditorCam->render_translucent();

	// PostProcess 물체 렌더링
	m_pEditorCam->render_postprocess();
}

void CRenderMgr::RegisterCamera(CCamera* _pCam)
{
	// 카메라가 RenderMgr에 최초 등록 되는 경우
	if (-1 == _pCam->m_iCamIdx)
	{
		// 카메라 등록
		m_vecCam.push_back(_pCam);

		// 푸쉬백한 카메라의 인덱스를 세팅
		int iIdx = (int)m_vecCam.size() - 1;
		_pCam->m_iCamIdx = iIdx;
	}
	// 등록된 적이 있다면
	else
	{
		// 벡터의 크기가 인덱스보다 작다면 resize
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

void CRenderMgr::CopyTargetToPostProcess()
{
	// 후처리 효과를 넣기위해 현재 렌더타겟을 복사해서 생성된 복사본으로 후처리효과를 넣어준다.

	Ptr<CTexture> pRenderTarget = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pPostProcess = CResMgr::GetInst()->FindRes<CTexture>(L"PostProcessTex");

	CONTEXT->CopyResource(pPostProcess->GetTex2D().Get(), pRenderTarget->GetTex2D().Get());
}


void CRenderMgr::UpdateLight2D()
{
	if (m_pLight2DBuffer->GetElementCount() < m_vecLight2D.size())	//광원의 사이즈가 커지면 사이즈만큼 Create
	{
		m_pLight2DBuffer->Create(sizeof(tLightInfo), m_vecLight2D.size(), SB_TYPE::READ_ONLY, true, nullptr);
	}

	static vector<tLightInfo> vecLight2DInfo;
	vecLight2DInfo.clear();

	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		vecLight2DInfo.push_back(m_vecLight2D[i]->GetLightInfo());
	}

	// 데이터를 레지스터에 보냄
	m_pLight2DBuffer->SetData(vecLight2DInfo.data(), (UINT)vecLight2DInfo.size());
	m_pLight2DBuffer->UpdateData(PIPELINE_STAGE::PS, 60);							

	g_global.Light2DCount = m_vecLight2D.size();	// 글로벌 상수에 광원의 개수를 저장

	m_vecLight2D.clear();
}

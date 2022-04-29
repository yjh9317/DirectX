#include "pch.h"
#include "CSceneMgr.h"

#include "CEventMgr.h"
#include "CCollisionMgr.h"

#include "CResMgr.h"
#include "CMesh.h"
#include "CGraphicsShader.h"

#include "CDevice.h"
#include "CPathMgr.h"

#include "CScene.h"
#include "CLayer.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CCamera.h"
#include "CCollider2D.h"
#include "CAnimator2D.h"
#include "CTileMap.h"
#include "CParticleSystem.h"

#include "CPlayerScript.h"
#include "CCameraMoveScript.h"
#include "CMissileScript.h"

#include "CTexture.h"
#include "CPrefab.h"

#include "CTestShader.h"


CSceneMgr::CSceneMgr()
	: m_pCurScene(nullptr)
{

}

CSceneMgr::~CSceneMgr()
{
	SAFE_DELETE(m_pCurScene);
}


void CSceneMgr::init()
{
	m_pCurScene = new CScene;
	m_pCurScene->SetLayerName(0, L"Tile");
	m_pCurScene->SetLayerName(1, L"Default");
	m_pCurScene->SetLayerName(2, L"Player");
	m_pCurScene->SetLayerName(3, L"Monster");

	// Texture 한장 로딩해보기
	CResMgr::GetInst()->Load<CTexture>(L"PlayerTexture", L"texture\\Player.bmp");
	CResMgr::GetInst()->Load<CTexture>(L"MagicCircle", L"texture\\MagicCircle.png");

	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"MagicCircle");

	// Texture Create 하기
	Ptr<CTexture> pTestTex = CResMgr::GetInst()->CreateTexture(L"TestTexture", 1024, 1024
		, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);

	// ComputeShader 실행하기
	Ptr<CTestShader> pCS = (CTestShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"TestCS").Get();
	pCS->SetOutputTexture(pTestTex);
	pCS->SetColor(Vec4(0.f, 1.f, 0.f, 1.f));
	pCS->Excute();


	// Prefab 제작
	CGameObject* pMissileObj = new CGameObject;
	pMissileObj->AddComponent(new CTransform);
	pMissileObj->AddComponent(new CMeshRender);
	pMissileObj->AddComponent(new CMissileScript);

	pMissileObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));
	pMissileObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh"));
	pMissileObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	CResMgr::GetInst()->AddRes<CPrefab>(L"MissilePrefab", new CPrefab(pMissileObj));


	// Camera Object 추가
	CGameObject* pCamObj = new CGameObject;
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraMoveScript);

	pCamObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pCamObj->Camera()->SetCameraAsMain();
	pCamObj->Camera()->CheckLayerMaskAll();

	m_pCurScene->AddObject(pCamObj, L"Default");

	// Particle Object 
	CGameObject* pParticleObj = new CGameObject;
	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);

	m_pCurScene->AddObject(pParticleObj, L"Default");





	CCollisionMgr::GetInst()->CollisionCheck(L"Player", L"Monster");

	m_pCurScene->start();
}

void CSceneMgr::progress()
{
	m_pCurScene->update();
	m_pCurScene->lateupdate();
	m_pCurScene->finalupdate();
}


void CSceneMgr::SpawnObject(CGameObject* _pSpawnObject, Vec3 _vWorldPos, wstring _strName, UINT _iLayerIdx)
{
	tEventInfo info = {};
	info.eType = EVENT_TYPE::CREATE_OBJ;
	info.lParam = (DWORD_PTR)_pSpawnObject;
	info.wParam = (DWORD_PTR)_iLayerIdx;

	_pSpawnObject->Transform()->SetRelativePos(_vWorldPos);
	_pSpawnObject->SetName(_strName);

	CEventMgr::GetInst()->AddEvent(info);
}

void CSceneMgr::SpawnObject(CGameObject* _pSpawnObject, UINT _iLayerIdx)
{
	tEventInfo info = {};
	info.eType = EVENT_TYPE::CREATE_OBJ;
	info.lParam = (DWORD_PTR)_pSpawnObject;
	info.wParam = (DWORD_PTR)_iLayerIdx;

	CEventMgr::GetInst()->AddEvent(info);
}

void CSceneMgr::AddChild(CGameObject* _pParentObject, CGameObject* _pChildObject)
{
	tEventInfo info = {};
	info.eType = EVENT_TYPE::ADD_CHILD;
	info.lParam = (DWORD_PTR)_pParentObject;
	info.wParam = (DWORD_PTR)_pChildObject;

	CEventMgr::GetInst()->AddEvent(info);
}

void CSceneMgr::ClearLayer()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_pCurScene->GetLayer(i)->Clear();
	}
}

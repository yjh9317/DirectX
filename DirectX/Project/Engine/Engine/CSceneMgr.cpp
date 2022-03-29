#include "pch.h"
#include "CSceneMgr.h"

#include "CResMgr.h"
#include "CMesh.h"
#include "CGraphicsShader.h"


#include "CScene.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CPlayerScript.h"
#include "CCameraMoveScript.h"

#include "CDevice.h"
#include "CCamera.h"

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
	m_pCurScene->SetLayerName(0, L"Default");		//특정 레이어 네이밍
	m_pCurScene->SetLayerName(1, L"Player");
	m_pCurScene->SetLayerName(2, L"Monster");

	// Camera Object 추가
	CGameObject* pCamObj = new CGameObject;
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraMoveScript);

	m_pCurScene->AddObject(pCamObj, L"Default");

	// Scene 에 GameObject 추가
	CGameObject* pObject = new CGameObject;

	pObject->SetName(L"Player");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CPlayerScript);

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetShader(CResMgr::GetInst()->FindRes<CGraphicsShader>(L"TestShader"));


	m_pCurScene->AddObject(pObject, L"Default"); //현재 씬에 pObject를 Default 레이어에 추가


	m_pCurScene->start();
}

void CSceneMgr::progress()
{
	m_pCurScene->update();
	m_pCurScene->lateupdate();
	m_pCurScene->finalupdate();
}

void CSceneMgr::render()
{
	CDevice::GetInst()->ClearTarget();
	
	m_pCurScene->render();

	CDevice::GetInst()->Present();
}

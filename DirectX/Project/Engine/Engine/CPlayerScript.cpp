#include "pch.h"
#include "CPlayerScript.h"

#include "CTransform.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"

#include "CMesh.h"
#include "CMaterial.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CLayer.h"

#include "CMeshRender.h"
#include "CMissileScript.h"

CPlayerScript::CPlayerScript()
	:m_fSpeed(0.5f)
{
}

CPlayerScript::~CPlayerScript()
{
}


void CPlayerScript::start()
{
}

void CPlayerScript::update()
{
	if (KEY_PRESSED(KEY::LEFT))
	{
		Vec3 vPos = Transform()->GetPos();
		vPos.x -= DT * 0.5f;
		Transform()->SetPos(vPos);
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		Vec3 vPos = Transform()->GetPos();
		vPos.x += DT * 0.5f;
		Transform()->SetPos(vPos);
	}

	if (KEY_PRESSED(KEY::Z))
	{
		Vec3 vRot = Transform()->GetRotation();
		vRot.z += DT * XM_2PI; // XM_2PI는 3.14
		Transform()->SetRotation(vRot);
	}


	// 축의 범위를 넘어서거나 CULL_BACK인데 반대방향으로 본다면
	// 보이지 않을 수도 있음.

	if (KEY_TAP(KEY::SPACE))
	{
		CGameObject* pMissileObj = new CGameObject;
		pMissileObj->AddComponent(new CTransform);
		pMissileObj->AddComponent(new CMeshRender);
		pMissileObj->AddComponent(new CMissileScript);

		pMissileObj->Transform()->SetPos(Transform()->GetPos() + Vec3(0.f, 50.f, 0.f));
		pMissileObj->Transform()->SetScale(Vec3(50.f, 50.f, 1.f));
		pMissileObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh"));
		pMissileObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		pCurScene->AddObject(pMissileObj, 0);
	}

}

void CPlayerScript::lateupdate()
{
}


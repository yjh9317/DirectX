#include "pch.h"
#include "CCameraMoveScript.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"

#include "CTransform.h"

CCameraMoveScript::CCameraMoveScript()
	: m_fCamSpeed(200.f)
{
}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::update()
{
	Vec3 vPos = Transform()->GetRelativePos();
	float fSpeed = m_fCamSpeed;

	if (KEY_PRESSED(KEY::LSHFT))
	{
		fSpeed *= 2.f;
	}

	if (KEY_PRESSED(KEY::W))
	{
		Vec3 vFront = Transform()->GetWorldFrontDir();

		vPos += DT * vFront * fSpeed;
	}

	if (KEY_PRESSED(KEY::S))
	{
		Vec3 vFront = Transform()->GetWorldFrontDir();

		vPos -= DT * vFront * fSpeed;
	}

	if (KEY_PRESSED(KEY::A))
	{
		Vec3 vRight = Transform()->GetWorldRightDir();

		vPos -= DT * vRight* fSpeed;
	}

	if (KEY_PRESSED(KEY::D))
	{
		Vec3 vRight = Transform()->GetWorldRightDir();

		vPos += DT * vRight * fSpeed;
	}

	if (KEY_PRESSED(KEY::RBTN))
	{
		Vec3 vRot = Transform()->GetRelativeRotation();

		Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();
		vRot.y += DT * vMouseDir.x * XM_PI;
		
		vRot.x -= DT * vMouseDir.y * XM_PI;

		Transform()->SetRelativeRotation(vRot);
	}

	Transform()->SetRelativePos(vPos);
}

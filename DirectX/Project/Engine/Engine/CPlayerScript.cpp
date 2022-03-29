#include "pch.h"
#include "CPlayerScript.h"

#include "CTransform.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"

CPlayerScript::CPlayerScript()
	:m_fSpeed(0.5f)
{
}

CPlayerScript::~CPlayerScript()
{
}


void CPlayerScript::start()
{
	Vec3 vPos = Vec3(0.f, 0.f, 0.5f);
	Transform()->SetPos(vPos);
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
		vRot.z += DT * XM_2PI; // XM_2PI�� 3.14
		Transform()->SetRotation(vRot);
	}


	// ���� ������ �Ѿ�ų� CULL_BACK�ε� �ݴ�������� ���ٸ�
	// ������ ���� ���� ����.

}

void CPlayerScript::lateupdate()
{
}


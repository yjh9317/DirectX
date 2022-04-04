
#include "pch.h"
#include "CMissileScript.h"

#include "CTimeMgr.h"
#include "CTransform.h"


CMissileScript::CMissileScript()
	: m_fSpeed(300.f)
{
}

CMissileScript::~CMissileScript()
{
}

void CMissileScript::update()
{
	Vec3 vPos = Transform()->GetPos();

	vPos.y += DT * m_fSpeed;

	Transform()->SetPos(vPos);
}


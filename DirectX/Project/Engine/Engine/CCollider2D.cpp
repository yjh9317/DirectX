#include "pch.h"
#include "CCollider2D.h"



CCollider2D::CCollider2D()
	:CComponent(COMPONENT_TYPE::COLLIDER2D)
	,m_eColliderType(COLLIDER2D_TYPE::BOX)
	,m_vOffsetPos(Vec2(0.f,0.f))
	,m_vOffsetScale(Vec2(1.f,1.f))
{

}

CCollider2D::~CCollider2D()
{

}

void CCollider2D::finalupdate()
{

}

void CCollider2D::render()
{

}
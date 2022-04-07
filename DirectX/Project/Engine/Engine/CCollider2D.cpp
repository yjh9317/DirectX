#include "pch.h"
#include "CCollider2D.h"

#include "CResMgr.h"


#include "CDevice.h"
#include "CConstBuffer.h"

#include "CTransform.h"


CCollider2D::CCollider2D()
	:CComponent(COMPONENT_TYPE::COLLIDER2D)
	,m_eColliderType(COLLIDER2D_TYPE::BOX)
	,m_vOffsetPos(Vec2(0.f,0.f))
	,m_vOffsetScale(Vec2(1.f,1.f))
{
	// Collider2D 모양에 맞는 메쉬 참조
	SetCollider2DType(m_eColliderType);


	// Collider2D 전용 재질 참조
	m_pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"Collider2DMtrl");
}

CCollider2D::~CCollider2D()
{

}

void CCollider2D::SetCollider2DType(COLLIDER2D_TYPE _type)
{
	m_eColliderType = _type;

	if (COLLIDER2D_TYPE::BOX == m_eColliderType)
	{
		m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_LineStrip");
	}
	else
	{
		m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh");
	}
}



void CCollider2D::finalupdate()
{

	//2차원 충돌이므로 z는 항등값 , 이동행렬 , 크기행렬
	Matrix matTrans = XMMatrixTranslation(m_vOffsetPos.x, m_vOffsetPos.y, 0.f);
	Matrix matScale = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, 1.f);
	m_matColWorld = matScale * matTrans;

	Vec3 vObjScale = Transform()->GetWorldScale();
	Matrix matObjScaleInv = XMMatrixInverse(nullptr, XMMatrixScaling(vObjScale.x, vObjScale.y, vObjScale.z));

	// 충돌체 상대행렬 * 오브젝트 월드 크기 역행렬(크기^-1) * 오브젝트 월드 행렬(크기 * 회전 * 이동)
	m_matColWorld = m_matColWorld * matObjScaleInv * Transform()->GetWorldMat();

}

void CCollider2D::UpdateData()
{
	g_transform.matWorld = m_matColWorld;
	g_transform.matWV = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;

	CConstBuffer* pCB = CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);
	pCB->SetData(&g_transform, sizeof(tTransform));
	pCB->UpdateData();
}

void CCollider2D::render()
{
	UpdateData();

	m_pMtrl->UpdateData();

	m_pMesh->render();
}
// 충돌체와 소유주가 월드행렬이 완전같음. 하지만 실수값이라 보일수도,안보일수도 있음
// 그러므로 깊이판정 상관없이 무조건 그리기 위해 Depth Stencil에서 무조건 통과시키도록 한다.
// 이 과정을 Shader에서 적용
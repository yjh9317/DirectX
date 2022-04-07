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
	// Collider2D ��翡 �´� �޽� ����
	SetCollider2DType(m_eColliderType);


	// Collider2D ���� ���� ����
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

	//2���� �浹�̹Ƿ� z�� �׵ , �̵���� , ũ�����
	Matrix matTrans = XMMatrixTranslation(m_vOffsetPos.x, m_vOffsetPos.y, 0.f);
	Matrix matScale = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, 1.f);
	m_matColWorld = matScale * matTrans;

	Vec3 vObjScale = Transform()->GetWorldScale();
	Matrix matObjScaleInv = XMMatrixInverse(nullptr, XMMatrixScaling(vObjScale.x, vObjScale.y, vObjScale.z));

	// �浹ü ������ * ������Ʈ ���� ũ�� �����(ũ��^-1) * ������Ʈ ���� ���(ũ�� * ȸ�� * �̵�)
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
// �浹ü�� �����ְ� ��������� ��������. ������ �Ǽ����̶� ���ϼ���,�Ⱥ��ϼ��� ����
// �׷��Ƿ� �������� ������� ������ �׸��� ���� Depth Stencil���� ������ �����Ű���� �Ѵ�.
// �� ������ Shader���� ����
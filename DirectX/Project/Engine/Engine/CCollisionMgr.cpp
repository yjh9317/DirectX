#include "pch.h"
#include "CCollisionMgr.h"

#include "CResMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CLayer.h"

#include "CGameObject.h"
#include "CCollider2D.h"


CCollisionMgr::CCollisionMgr()
	: m_arrCheck{}
{
}

CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::update()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		for (UINT j = i; j < MAX_LAYER; ++j)
		{
			if (m_arrCheck[i] & (1 << j))
			{
				// i �� �ش��ϴ� EOBJ_TYPE ������Ʈ���, j �� �ش��ϴ�EOBJ_TYPE ������Ʈ�� ���� �浹 ����
				CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

				CollisionBetweenLayer(pCurScene->GetLayer(i)->GetObjects()
					, pCurScene->GetLayer(j)->GetObjects());
			}
		}
	}
}

void CCollisionMgr::CollisionCheck(const wstring& _strLeftName, const wstring& _strRightName)
{
}

void CCollisionMgr::CollisionOff(const wstring& _strLeftName, const wstring& _strRightName)
{
}

void CCollisionMgr::CollisionBetweenLayer(const vector<CGameObject*>& _left, const vector<CGameObject*>& _right)
{
	CCollider2D* pLeftCol = nullptr;
	CCollider2D* pRightCol = nullptr;

	for (size_t i = 0; i < _left.size(); ++i)
	{
		pLeftCol = _left[i]->Collider2D();

		if (nullptr == pLeftCol)
			continue;

		for (size_t j = 0; j < _right.size(); ++j)
		{
			pRightCol = _right[j]->Collider2D();

			if (nullptr == pRightCol)
				continue;


			// �� �浹ü�� ������ �浹�߾����� Ȯ��
			// �� �浹ü�� ���� ���̵� ����
			COLLIDER_ID ID;
			ID.iLeftID = pLeftCol->GetID();
			ID.iRightID = pRightCol->GetID();
			map<long long, bool>::iterator iter = m_mapColInfo.find(ID.id);

			// �� �浹ü�� ó�� �˻縦 �����ϰ� �ִ�.
			if (iter == m_mapColInfo.end())
			{
				m_mapColInfo.insert(make_pair(ID.id, false));
				iter = m_mapColInfo.find(ID.id);
			}

			// �� �浹ü�� �ϳ��� ���� �������� 
			bool bDead = pLeftCol->GetOwner()->IsDead() || pRightCol->GetOwner()->IsDead();

			// �浹 �˻�
			// �浹 ���̴�.
			if (IsCollision(pLeftCol, pRightCol))
			{
				// ���� �����ӿ��� �浹 ���̾���.
				if (iter->second)
				{
					// �浹 ���̴�
					if (bDead)
					{
						// ���� �ϳ��� ���� �����̶��(������ ���� �浹 ������ �߻��ϴ� ������ ����)
						pLeftCol->OnCollisionExit(pRightCol);
						pRightCol->OnCollisionExit(pLeftCol);
						m_mapColInfo.erase(iter);
					}
					else
					{
						pLeftCol->OnCollision(pRightCol);
						pRightCol->OnCollision(pLeftCol);
					}
				}

				// �������� �浹���� �ʰ� �־���.
				else
				{
					// �̹��� �� �浹 �����ߴ�.
					if (!bDead)
					{
						// �� �� �ϳ��� ���� �����̶�� �浹 ���� ��ü�� ���� �ʰ� �Ѵ�.
						pLeftCol->OnCollisionEnter(pRightCol);
						pRightCol->OnCollisionEnter(pLeftCol);
						iter->second = true;
					}
				}
			}

			// �浹���� �ʰ� �ִ�.
			else
			{
				// ���� �����ӿ����� �浹 ���̾���.
				if (iter->second)
				{
					pLeftCol->OnCollisionExit(pRightCol);
					pRightCol->OnCollisionExit(pLeftCol);
					iter->second = false;
				}
			}
		}
	}
}

bool CCollisionMgr::IsCollision(CCollider2D* _pLeftCol, CCollider2D* _pRightCol)
{
	// �浹ü�� ����ϴ� �⺻ ����(�簢��) ���� ������ġ�� �˾Ƴ���, �ý��� �޸����� ���۸� �������� ���ý����̽��� ��ǥ�� �� �� �ִ�.
	// 0 -- 1
	// | \  |
	// 3 -- 2	
	static CMesh* pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh").Get();
	static Vtx* pVtx = pRectMesh->GetVtxSysMem();
	static Vec3 vLocalPos[4] = { pVtx[0].vPos, pVtx[1].vPos, pVtx[2].vPos, pVtx[3].vPos };

	Matrix matLeft = _pLeftCol->GetWorldMat();
	Matrix matRight = _pRightCol->GetWorldMat();

	// Local �����̽��� �װ��� ������ �� �浹ü ���� ��ġ�� ������.
	Vec3 vAsix[4] = {};

	// (vector3, 1.f)���� 1.f�� �̵���ȯ�� ��ȯ��Ű�ڴ�, 0.f�� �̵���ȯ�� �����Ű�� �ʰڴٶ���ǹ�.
	// ��4���� �ƴ϶� 3���� ����ؼ� ����.(�ݴ��ʵ� ���� ���⺤���̹Ƿ�)
	// ���ý����̽��� �߽���(0, 0)�� ������ ������ ������ǥ�� ����.
	// Normalize(����ȭ)�� ���� �ʴ� ������ �ٸ� �࿡ ������ų ���� ���� ���̸� ����� ���� �ִ�.
	// ������ ����� ���� ���̰� �������.

	// (Vector3, 0.f) X Matirx(4x4)
	// XMVector3TransformNormal();

	// (Vector3, 1.f) X Matirx(4x4)
	// ����� ���� ������ ���ؼ� �� ���� �� �̸鼭 ������ų ���� ���� 4���� ���Ѵ�.
	vAsix[0] = XMVector3TransformCoord(vLocalPos[1], matLeft) - XMVector3TransformCoord(vLocalPos[0], matLeft);
	vAsix[1] = XMVector3TransformCoord(vLocalPos[3], matLeft) - XMVector3TransformCoord(vLocalPos[0], matLeft);
	vAsix[2] = XMVector3TransformCoord(vLocalPos[1], matRight) - XMVector3TransformCoord(vLocalPos[0], matRight);
	vAsix[3] = XMVector3TransformCoord(vLocalPos[3], matRight) - XMVector3TransformCoord(vLocalPos[0], matRight);

	// ���忡 ��ġ�� �� �浹ü�� �߽��� ���� ����
	//Vec3 vCenter = XMVector3TransformCoord(Vec3::Zero, matRight) - XMVector3TransformCoord(Vec3::Zero, matLeft);	
	Vec3 vCenter = _pRightCol->GetWorldPos() - _pLeftCol->GetWorldPos();


	return true;
}


void CCollisionMgr::CollisionCheck(int _iLayerLeftIdx, int _iLayerRightIdx)
{
	UINT iCol = 0, iRow = 0;

	if ((UINT)_iLayerLeftIdx > (UINT)_iLayerRightIdx)
	{
		iCol = (UINT)_iLayerLeftIdx;
		iRow = (UINT)_iLayerRightIdx;
	}
	else
	{
		iCol = (UINT)_iLayerRightIdx;
		iRow = (UINT)_iLayerLeftIdx;
	}

	m_arrCheck[iRow] |= (1 << iCol);
}

void CCollisionMgr::CollisionOff(int _iLayerLeftIdx, int _iLayerRightIdx)
{
	UINT iCol = 0, iRow = 0;

	if ((UINT)_iLayerLeftIdx > (UINT)_iLayerRightIdx)
	{
		iCol = (UINT)_iLayerLeftIdx;
		iRow = (UINT)_iLayerRightIdx;
	}
	else
	{
		iCol = (UINT)_iLayerRightIdx;
		iRow = (UINT)_iLayerLeftIdx;
	}

	m_arrCheck[iRow] &= ~(1 << iCol);
}

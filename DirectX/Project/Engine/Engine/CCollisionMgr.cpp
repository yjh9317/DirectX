#include "pch.h"
#include "CCollisionMgr.h"

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
				// i 에 해당하는 EOBJ_TYPE 오브젝트들과, j 에 해당하는EOBJ_TYPE 오브젝트들 끼리 충돌 진행
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


			// 두 충돌체가 이전에 충돌했었는지 확인
			// 두 충돌체의 조합 아이디 생성
			COLLIDER_ID ID;
			ID.iLeftID = pLeftCol->GetID();
			ID.iRightID = pRightCol->GetID();
			map<long long, bool>::iterator iter = m_mapColInfo.find(ID.id);

			// 두 충돌체는 처음 검사를 진행하고 있다.
			if (iter == m_mapColInfo.end())
			{
				m_mapColInfo.insert(make_pair(ID.id, false));
				iter = m_mapColInfo.find(ID.id);
			}

			// 두 충돌체중 하나라도 데드 상태인지 
			bool bDead = pLeftCol->GetOwner()->IsDead() || pRightCol->GetOwner()->IsDead();

			// 충돌 검사
			// 충돌 중이다.
			if (IsCollision(pLeftCol, pRightCol))
			{
				// 이전 프레임에도 충돌 중이었다.
				if (iter->second)
				{
					// 충돌 중이다
					if (bDead)
					{
						// 둘중 하나라도 삭제 예정이라면(삭제를 통한 충돌 해제가 발생하는 것으로 본다)
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

				// 이전에는 충돌하지 않고 있었다.
				else
				{
					// 이번에 막 충돌 진입했다.
					if (!bDead)
					{
						// 둘 중 하나라도 삭제 예정이라면 충돌 시작 자체를 하지 않게 한다.
						pLeftCol->OnCollisionEnter(pRightCol);
						pRightCol->OnCollisionEnter(pLeftCol);
						iter->second = true;
					}
				}
			}

			// 충돌하지 않고 있다.
			else
			{
				// 이전 프레임에서는 충돌 중이었다.
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
	Vec3 vLeftPos = _pLeftCol->GetWorldPos();
	Vec3 vRightPos = _pRightCol->GetWorldPos();

	Vec3 vLeftScale = _pLeftCol->GetWorldScale();
	Vec3 vRightScal = _pRightCol->GetWorldScale();

	float fDist = 0.f;

	// x 축 테스트
	fDist = abs(vLeftPos.x - vRightPos.x);
	if (fDist > (vLeftScale.x / 2.f) + (vRightScal.x / 2.f))
		return false;

	// y 축 테스트
	fDist = abs(vLeftPos.y - vRightPos.y);
	if (fDist > (vLeftScale.y / 2.f) + (vRightScal.y / 2.f))
		return false;

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

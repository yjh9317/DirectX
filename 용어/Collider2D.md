Collider2D
=============
오브젝트의 컴포넌트 요소중 하나.  
2D게임을 만들면서 각 오브젝트마다 충돌 했을 때의 상호작용을 위해 충돌체를 설정해준다.    
오브젝트끼리 가지고 있는 충돌체끼리 부딪혔을 때 그에 맞는 코드를 작성해서 오브젝트에 변화를 줄 수 있다.  
원,네모 모양의 충돌체가 있고 다른 모양끼리의 충돌체는 지원하지 않음.  

Collider2D 헤더
================

        #pragma once
        #include "CComponent.h"
        
        #include "CMesh.h"
        #include "CMaterial.h"
        
        
        enum class COLLIDER2D_TYPE
        {
            BOX,        // 네모 모양의 충돌체
            CIRCLE,     // 원 모양의 충돌체
        };
        
        class CCollider2D :
            public CComponent
        {
        private:
            Matrix              m_matColWorld;      // 월드행렬
        
            COLLIDER2D_TYPE     m_eColliderType;    // 충돌체 타입을 정하는 변수
        
            Vec2                m_vOffsetPos;       // 오브젝트로 부터의 충돌체의 상대 거리
            Vec2                m_vOffsetScale;     // 오브젝트 크기로 부터 충돌체의 배율
        
            Ptr<CMesh>          m_pMesh;            // 충돌체 메쉬
            Ptr<CMaterial>      m_pMtrl;            // 충돌체 랜더링 재질
        
            int                 m_iCollisionCount;  // 충돌 횟수
        
        
        public:
            void SetCollider2DType(COLLIDER2D_TYPE _type);
        
            void SetOffsetPos(Vec2 _vOffsetPos) { m_vOffsetPos = _vOffsetPos; }
            void SetOffsetPos(float _x, float _y) { m_vOffsetPos = Vec2(_x, _y); }
        
            void SetOffsetScale(Vec2 _vOffsetScale);
            void SetOffsetScale(float _x, float _y);
        
            COLLIDER2D_TYPE GetCollider2DType() { return m_eColliderType; }
            Vec3 GetWorldPos() { return m_matColWorld.Translation(); }
            Vec3 GetWorldScale() { return Vec3(m_vOffsetScale); }   //충돌체는 행렬에서 부모의 크기를 역행렬을 곱하기 때문에 오프셋이 곧 충돌체의 크기.
        
            Matrix GetWorldMat() { return m_matColWorld; }
        
        
        public:
            virtual void finalupdate() override;
            virtual void UpdateData() override;
            void render();
        
        public:
            // 밑에는 각각 충돌체가 부딪힐 때, 부딪히는 도중, 충돌이 끝난 시점을 호출하는 
            
            // 이전 프레임에는 충돌 X, 이번 프레임에 충돌 시작일 때
            void OnCollisionEnter(CCollider2D* _Other);
        
            // 이전 프레임에도 충돌 중, 지금도 충돌 중일 때
            void OnCollision(CCollider2D* _Other);
        
            // 이전 프레임에 충돌 중, 지금은 충돌 X 일때
            void OnCollisionExit(CCollider2D* _Other);
        
        
            CLONE(CCollider2D)
        public:
            CCollider2D();
            CCollider2D(const CCollider2D& _Origin);
            ~CCollider2D();
        };






Collider2D Cpp
====================
        #include "pch.h"
        #include "CCollider2D.h"
        
        #include "CResMgr.h"
        
        
        #include "CDevice.h"
        #include "CConstBuffer.h"
        
        #include "CTransform.h"
        #include "CScript.h"
        
        CCollider2D::CCollider2D()
        	:CComponent(COMPONENT_TYPE::COLLIDER2D)
        	, m_eColliderType(COLLIDER2D_TYPE::BOX)
        	, m_vOffsetPos(Vec2(0.f, 0.f))
        	, m_vOffsetScale(Vec2(1.f, 1.f))
        {
        	// Collider2D 모양에 맞는 메쉬 참조
        	SetCollider2DType(m_eColliderType);
        
        
        	// Collider2D 전용 재질 참조
        	m_pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"Collider2DMtrl");
        }
        
        CCollider2D::CCollider2D(const CCollider2D& _Origin)
        	: CComponent(_Origin)
        	, m_eColliderType(_Origin.m_eColliderType)
        	, m_vOffsetPos(_Origin.m_vOffsetPos)
        	, m_vOffsetScale(_Origin.m_vOffsetScale)
        	, m_iCollisionCount(0)
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
        	m_eColliderType = _type;  // 타입 설정
        
        	if (COLLIDER2D_TYPE::BOX == m_eColliderType) //네모 모양이라면
        	{
        		m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_LineStrip");
        	}
        	else    // 원 모양 이라면
        	{
        		m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh_LineStrip");
        	}
        }
        
        
        void CCollider2D::SetOffsetScale(Vec2 _vOffsetScale)
        {
        	m_vOffsetScale = _vOffsetScale;
        
        	if (COLLIDER2D_TYPE::CIRCLE == m_eColliderType) 
        	{
        		m_vOffsetScale.y = m_vOffsetScale.x; //원은 x,y길이(반지름)가 서로 같음
        	}
        }
        
        void CCollider2D::SetOffsetScale(float _x, float _y)
        {
        	m_vOffsetScale = Vec2(_x, _y);
        
        	if (COLLIDER2D_TYPE::CIRCLE == m_eColliderType)
        	{
        		m_vOffsetScale.y = m_vOffsetScale.x;  //원은 x,y길이(반지름)가 서로 같음
        	}
        }
        
        void CCollider2D::finalupdate()
        {
        
        	//2차원 충돌이므로 z는 항등값 , 이동행렬 , 크기행렬
        	Matrix matTrans = XMMatrixTranslation(m_vOffsetPos.x, m_vOffsetPos.y, 0.f);
        	Matrix matScale = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, 1.f);
        	m_matColWorld = matScale * matTrans; //월드 행렬 계산
        
        	Vec3 vObjScale = Transform()->GetWorldScale();
        	Matrix matObjScaleInv = XMMatrixInverse(nullptr, XMMatrixScaling(vObjScale.x, vObjScale.y, vObjScale.z)); // 크기의 역행렬
        
        	// 충돌체 상대행렬 * 오브젝트 월드 크기 역행렬(크기^-1) * 오브젝트 월드 행렬(크기 * 회전 * 이동)
          // 크기 역행렬을 곱해줌으로써 크기는 1이 되고 거기서 월드행렬의 크기가 곱해지므로 월드행렬의 크기가 곧 오브젝트의 크기가 된다.
        	m_matColWorld = m_matColWorld * matObjScaleInv * Transform()->GetWorldMat();
        
        }
        
        void CCollider2D::UpdateData()
        {
          // 월드행렬 , 뷰 행렬 , 직영 행렬
        	g_transform.matWorld = m_matColWorld;
        	g_transform.matWV = g_transform.matWorld * g_transform.matView;
        	g_transform.matWVP = g_transform.matWV * g_transform.matProj;
        
          // 상수버퍼에 충돌체의 값들을 복사후 레지스터에 바인딩
        	CConstBuffer* pCB = CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);
        	pCB->SetData(&g_transform, sizeof(tTransform)); 
        	pCB->UpdateData();
        }
        
        void CCollider2D::render()
        {
        	UpdateData();
        
        	m_pMtrl->SetScalarParam(L"IsCollision", &m_iCollisionCount);
        	m_pMtrl->UpdateData();
        
        	m_pMesh->render();
        }
        // 충돌체와 소유주가 월드행렬이 완전같음. 하지만 실수값이라 매 프레임마다 바뀌므로 보일수도,안보일수도 있음
        // 그러므로 깊이판정 상관없이 무조건 그리기 위해 Depth Stencil에서 무조건 통과시키도록 한다.
        // 이 과정을 Shader에서 적용
        
        void CCollider2D::OnCollisionEnter(CCollider2D* _Other)
        {
        	++m_iCollisionCount; // 한 오브젝트에 여러개 오브젝트 충돌이 일어 날 수 있으므로 충돌 개수로 확인
        
        	// 스크립트에서 오브젝트의 상태를 조정하기 위해 스크립트에서 충돌체크
        	CScript* pScript = GetOwner()->GetScript();
        
        	if (nullptr != pScript) //부딪힌 오브젝트가 스크립트를 가지고 있는 오브젝트라면 충돌시작
        		pScript->OnCollisionEnter(_Other->GetOwner());
        }
        
        void CCollider2D::OnCollision(CCollider2D* _Other)
        {
        	CScript* pScript = GetOwner()->GetScript();
        
        	if (nullptr != pScript)
        		pScript->OnCollision(_Other->GetOwner());
        }
        
        void CCollider2D::OnCollisionExit(CCollider2D* _Other)
        {
        	--m_iCollisionCount;
        
        	CScript* pScript = GetOwner()->GetScript();
        
        	if (nullptr != pScript)
        		pScript->OnCollisionExit(_Other->GetOwner());
        }

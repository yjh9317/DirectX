#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"


// 충돌체의 모양
enum class COLLIDER2D_TYPE
{
    BOX,
    CIRCLE,
};

class CCollider2D :
    public CComponent
{
private:
    Matrix              m_matColWorld;          // 월드 매트릭스

    COLLIDER2D_TYPE     m_eColliderType;

    Vec2                m_vOffsetPos;           // 오브젝트로부터의 충돌체의 상대거리
    Vec2                m_vOffsetScale;         // 오브젝트 크기로부터 충돌체의 배율

    Ptr<CMesh>          m_pMesh;                 // 충돌체 모양
    Ptr<CMaterial>      m_pMtrl;                // 충돌체 렌더링 재질

public:
    void SetCollider2DType(COLLIDER2D_TYPE _type);
    void SetOffsetPos(Vec2 _vOffsetPos) { m_vOffsetPos = _vOffsetPos; }
    void SetOffsetScale(Vec2 _vOffsetScale) { m_vOffsetScale = _vOffsetScale; }


public:
    virtual void finalupdate() override;
    virtual void UpdateData() override;
    void render();

    CLONE(CCollider2D)

public:
    CCollider2D();
    ~CCollider2D();
};


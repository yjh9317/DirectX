#pragma once
#include "CComponent.h"

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
    COLLIDER2D_TYPE     m_eColliderType;

    Vec2                m_vOffsetPos;           // 오브젝트로부터의 충돌체의 상대거리
    Vec2                m_vOffsetScale;         // 오브젝트 크기로부터 충돌체의 배율

public:
    void SetCollider2DType(COLLIDER2D_TYPE _type) { m_eColliderType = _type; }
    void SetOffsetPos(Vec2 _vOffsetPos) { m_vOffsetPos = _vOffsetPos; }
    void SetOffsetScale(Vec2 _vOffsetScale) { m_vOffsetScale = _vOffsetScale; }

public:
    virtual void finalupdate() override;
    void render();

public:
    CCollider2D();
    ~CCollider2D();
};


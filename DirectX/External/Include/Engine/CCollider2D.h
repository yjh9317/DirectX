#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"


// �浹ü�� ���
enum class COLLIDER2D_TYPE
{
    BOX,
    CIRCLE,
};

class CCollider2D :
    public CComponent
{
private:
    Matrix              m_matColWorld;          // ���� ��Ʈ����

    COLLIDER2D_TYPE     m_eColliderType;

    Vec2                m_vOffsetPos;           // ������Ʈ�κ����� �浹ü�� ���Ÿ�
    Vec2                m_vOffsetScale;         // ������Ʈ ũ��κ��� �浹ü�� ����

    Ptr<CMesh>          m_pMesh;                 // �浹ü ���
    Ptr<CMaterial>      m_pMtrl;                // �浹ü ������ ����

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


#pragma once
#include "CComponent.h"
class CTransform :      //������Ʈ�� Location(��ġ,ũ��,ȸ��) ���
    public CComponent
{
private:
    Vec3        m_vRelativePos;
    Vec3        m_vRelativeScale;
    Vec3        m_vRelativeRot;     // �ະ ȸ����

    Matrix      m_matWorld;         //��ġ��ȯ ���� ��� 

    // �θ��� ũ�⿡ ������ ���� �ʱ� ���� ����
    bool        m_bIgnoreParentScale;

public:
    void SetPos(const Vec3& _vPos) { m_vRelativePos = _vPos; }
    void SetScale(const Vec3& _vScale) { m_vRelativeScale = _vScale; }
    void SetRotation(const Vec3 _vRot) { m_vRelativeRot = _vRot; }

    const Vec3& GetPos() { return m_vRelativePos; }
    const Vec3& GetScale() { return m_vRelativeScale; }
    const Vec3& GetRotation() { return m_vRelativeRot; }

    Vec3 GetWorldScale();

    const Matrix& GetWorldMat() { return m_matWorld; }

    void SetIgnoreParentScale(bool _bSet)  { m_bIgnoreParentScale = _bSet;  }

    virtual void UpdateData() override; //�ʿ��� �����͸� gpu�޸𸮷� �̵�

    virtual void finalupdate() override;

    CLONE(CTransform)

public:
    CTransform();
    ~CTransform();
};


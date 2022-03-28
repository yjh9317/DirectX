#pragma once
#include "CComponent.h"
class CTransform :      //������Ʈ�� Location(��ġ,ũ��,ȸ��) ���
    public CComponent
{
private:
    Vec3        m_vRelativePos;
    Vec3        m_vRelativeScale;
    Vec3        m_vRelativeRot;

    Matrix      m_matWorld;         //��ġ��ȯ ���� ���, ��ķ� ��ġ,ũ��,ȸ������ ���Ѵ�.

public:
    void SetPos(const Vec3& _vPos) { m_vRelativePos = _vPos; }
    void SetScale(const Vec3& _vScale) { m_vRelativeScale = _vScale; }
    void SetRotation(const Vec3 _vRot) { m_vRelativeRot = _vRot; }

    const Vec3& GetPos() { return m_vRelativePos; }
    const Vec3& GetScale() { return m_vRelativeScale; }
    const Vec3& GetRotation() { return m_vRelativeRot; }

    virtual void UpdateData() override; //�ʿ��� �����͸� gpu�޸𸮷� �̵�

    virtual void finalupdate() override;

public:
    CTransform();
    ~CTransform();
};


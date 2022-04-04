#pragma once
#include "CComponent.h"
class CTransform :      //오브젝트의 Location(위치,크기,회전) 담당
    public CComponent
{
private:
    Vec3        m_vRelativePos;
    Vec3        m_vRelativeScale;
    Vec3        m_vRelativeRot;     // 축별 회전량

    Matrix      m_matWorld;         //위치변환 정보 행렬 

public:
    void SetPos(const Vec3& _vPos) { m_vRelativePos = _vPos; }
    void SetScale(const Vec3& _vScale) { m_vRelativeScale = _vScale; }
    void SetRotation(const Vec3 _vRot) { m_vRelativeRot = _vRot; }

    const Vec3& GetPos() { return m_vRelativePos; }
    const Vec3& GetScale() { return m_vRelativeScale; }
    const Vec3& GetRotation() { return m_vRelativeRot; }

    virtual void UpdateData() override; //필요한 데이터를 gpu메모리로 이동

    virtual void finalupdate() override;

    CLONE(CTransform)

public:
    CTransform();
    ~CTransform();
};


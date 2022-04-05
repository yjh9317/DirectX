#pragma once
#include "CComponent.h"

// 카메라는 하나의 오브젝트를 만들어서 카메라역할을 수행
// View Space(Camera Space): 카메라의 위치,방향을 기준으로 물체들의 Transform값을 적용
// View Space에서는 카메라의 위치가 원점, 카메라가 바라보는 정면이 z축이다.

class CCamera :
    public CComponent
{
private:
    Matrix m_matView;   // View행렬, 이 행렬을 곱하면 View Space로 넘어온다.
    Matrix m_matProj;   // 투영 행렬(View Space로 넘어온 물체를 -1 ~ 1범위로 압축할 때 필요한 행렬)

public:
    virtual void finalupdate() override;

    CLONE(CCamera)
public:
    CCamera();
    ~CCamera();
};


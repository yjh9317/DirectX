#pragma once
#include "CComponent.h"

// 카메라는 하나의 오브젝트를 만들어서 카메라역할을 수행
// View Space(Camera Space): 카메라의 위치,방향을 기준으로 물체들의 Transform값을 적용
// View Space에서는 카메라의 위치가 원점, 카메라가 바라보는 정면이 z축이다.


// Camera의 시점에 맞게 물체를 배치시키려면 World Space에 있는 물체들을
// Camera Object가 World의 원점 기준으로 바뀐 Transform값(회전,이동,크기)만큼 
// 똑같이 적용하면 된다.


class CCamera :         
    public CComponent
{
private:
    Matrix m_matView;   //View행렬을 곱하면 View Space로 넘어온다.

public:
    virtual void finalupdate() override;

public:
    CCamera();
    ~CCamera();
};


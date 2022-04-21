#pragma once
#include "CRenderComponent.h"

#include "CComponent.h"




// SharedMtrl,DynamicMtrl

// 오브젝트별로 자기만의 재질을 가지고 있는 형태로 만들순 있지만(메모리도 그렇게 크진 않음) 최적화를 위해서 그렇게 만들지 않음.
// 렌더링파이프라인에서 발생하는 Draw call을 줄이기위해(cpu와 gpu간의 메모리 이전비용이 큼)
// 렌더링할 때 연산비용이 많은 이유는 IA단계에서 셰이더나 여러 버퍼, Material(상수버퍼를 옮기고 레지스터 바인딩)등  오브젝트 하나 호출할 때마다 준비해줘야함.
// 
// 2D에서는 같은 매터리얼로 묶었을 때 이점을 볼 수 없다.
// 3D에서는 instancing을 통해 A와 B는 게임오브젝트를 그릴때 렌더링 파이프라인과정이 있는데 같은 매터리얼을 사용하고있으면
// 중간에 같은재질을 사용하는 오브젝트끼리 묶어서 일괄처리하는 것이 instancing(메쉬도 같아야함)


class CMeshRender :
    public CRenderComponent
{

public:

public:
    virtual void finalupdate() override;
    virtual void render() override;

    CLONE(CMeshRender)

public:
    CMeshRender();
    ~CMeshRender();
};


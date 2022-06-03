#pragma once
#include "UI.h"

#include <Engine/CGameObject.h>

class ComponentUI;
class ResInfoUI;
class CRes;


class InspectorUI :
    public UI
{
private:
    CGameObject*    m_pTargetObject;                       // UI를 띄울 오브젝트
    CRes*           m_pTargetRes;                          // UI를 띄울 리소스


    ComponentUI* m_arrComUI[(UINT)COMPONENT_TYPE::END]; // 타겟의 Component를 담는 배열
    ResInfoUI* m_arrResUI[(UINT)RES_TYPE::END];         // 리소스의 정보를 담고있는 배열


public:
    void SetTargetObject(CGameObject* _pTarget);
    void SetTargetResource(CRes* _pTargetRes);

public:
    virtual void update() override;
    virtual void render_update() override;




public:
    InspectorUI();
    ~InspectorUI();
};


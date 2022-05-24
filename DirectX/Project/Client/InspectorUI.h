#pragma once
#include "UI.h"

#include <Engine/CGameObject.h>

class ComponentUI;

class InspectorUI :
    public UI
{
private:
    CGameObject* m_pTargetObject;                       // UI�� ��� Ÿ��

    ComponentUI* m_arrComUI[(UINT)COMPONENT_TYPE::END]; // Ÿ���� Component�� ��� �迭



public:
    void SetTargetObject(CGameObject* _pTarget);

public:
    virtual void update() override;
    virtual void render_update() override;




public:
    InspectorUI();
    ~InspectorUI();
};


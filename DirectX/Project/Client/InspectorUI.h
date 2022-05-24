#pragma once
#include "UI.h"

#include <Engine/CGameObject.h>

class ComponentUI;

class InspectorUI :
    public UI
{
private:
    CGameObject* m_pTargetObject;                       // UI¸¦ ¶ç¿ï Å¸°Ù

    ComponentUI* m_arrComUI[(UINT)COMPONENT_TYPE::END]; // Å¸°ÙÀÇ Component¸¦ ´ã´Â ¹è¿­



public:
    void SetTargetObject(CGameObject* _pTarget);

public:
    virtual void update() override;
    virtual void render_update() override;




public:
    InspectorUI();
    ~InspectorUI();
};


#pragma once
#include "UI.h"

#include <Engine/CGameObject.h>

class ComponentUI :
    public UI
{
private:
    CGameObject*        m_pTargetObject;    // 타겟오브젝트
    COMPONENT_TYPE      m_eComType;         // 타겟오브젝트의 타입
    bool                m_bActive;          

public:
    void SetTargetObject(CGameObject* _pTarget) { m_pTargetObject = _pTarget; }
    CGameObject* GetTargetObject() { return m_pTargetObject; }

protected:
    bool IsComponentActive() { return m_bActive; }

public:
    virtual void update() override;


public:
    ComponentUI(const string& _strName, COMPONENT_TYPE _eComType);  // 컴포넌트를 UI로 만들 때 Name(키값)과 타입을 넣어줘야 한다.
    ~ComponentUI();
};


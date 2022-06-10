#pragma once
#include "UI.h"

#include <Engine/CGameObject.h>

class ScriptUI :
    public UI
{
private:
    CGameObject* m_pTargetObject;   // 스크립트를 가진 오브젝트
    CScript* m_pTargetScript;       // 이 스크립트UI의 타겟 스크립트
    COMPONENT_TYPE  m_eComType;

    bool            m_bActive;

public:
    void SetTargetObject(CGameObject* _pTarget) { m_pTargetObject = _pTarget; }
    CGameObject* GetTargetObject() { return m_pTargetObject; }

    void SetTargetScript(CScript* _pScript) { m_pTargetScript = _pScript; }
    CScript* GetTargetScript() { return m_pTargetScript; }

protected:
    bool IsComponentActive() { return m_bActive; }

public:
    virtual void update() override;
    virtual void render_update() override;


public:
    ScriptUI();
    ~ScriptUI();
};


#pragma once
#include "UI.h"

#include <Engine/CGameObject.h>

class ComponentUI :
    public UI
{
private:
    CGameObject*        m_pTargetObject;    // Ÿ�ٿ�����Ʈ
    COMPONENT_TYPE      m_eComType;         // Ÿ�ٿ�����Ʈ�� Ÿ��
    bool                m_bActive;          

public:
    void SetTargetObject(CGameObject* _pTarget) { m_pTargetObject = _pTarget; }
    CGameObject* GetTargetObject() { return m_pTargetObject; }

protected:
    bool IsComponentActive() { return m_bActive; }

public:
    virtual void update() override;


public:
    ComponentUI(const string& _strName, COMPONENT_TYPE _eComType);  // ������Ʈ�� UI�� ���� �� Name(Ű��)�� Ÿ���� �־���� �Ѵ�.
    ~ComponentUI();
};


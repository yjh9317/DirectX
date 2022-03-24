#pragma once
#include "CEntity.h"

class CGameObject;

class CComponent :
    public CEntity
{
private:
    const COMPONENT_TYPE     m_eComType;        //��ü�� ������Ʈ Ÿ��
    CGameObject*            m_pOwner;

public:
    virtual void update() {}
    virtual void lateupdate() {}
    virtual void finalupdate() = 0; //���� ������Ʈ�̹Ƿ� ���������Լ�
    virtual void render() {}

public:
    COMPONENT_TYPE GetType() { return m_eComType; }


public:
    CComponent(COMPONENT_TYPE _eType); //������ƮŸ���� ������ ������ �� �ְ� �⺻������ X
    virtual ~CComponent();

    friend class CGameObject;
};


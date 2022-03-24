#pragma once
#include "CEntity.h"

class CGameObject;

class CComponent :
    public CEntity
{
private:
    const COMPONENT_TYPE     m_eComType;        //물체의 컴포넌트 타입
    CGameObject*            m_pOwner;

public:
    virtual void update() {}
    virtual void lateupdate() {}
    virtual void finalupdate() = 0; //최종 업데이트이므로 순수가상함수
    virtual void render() {}

public:
    COMPONENT_TYPE GetType() { return m_eComType; }


public:
    CComponent(COMPONENT_TYPE _eType); //컴포넌트타입이 무조건 세팅할 수 있게 기본생성자 X
    virtual ~CComponent();

    friend class CGameObject;
};


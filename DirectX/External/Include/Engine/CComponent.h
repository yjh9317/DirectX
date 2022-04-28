#pragma once
#include "CEntity.h"

#include "CGameObject.h"

#define GET_OTHER_COMPONENT(Type) C##Type* Type() { return GetOwner()->Type(); }


class CComponent :
    public CEntity
{
private:
    const COMPONENT_TYPE     m_eComType;        //물체의 컴포넌트 타입
    CGameObject* m_pOwner;

public:
    virtual void start() {}
    virtual void update() {}
    virtual void lateupdate() {}
    virtual void finalupdate() = 0; //최종 업데이트이므로 순수가상함수
    

public:
    COMPONENT_TYPE GetType() { return m_eComType; }
    CGameObject* GetOwner() { return m_pOwner; }



    // 컴포넌트끼리 서로 상호작용을 하므로 CComponent(부모클래스)에서 작성
    GET_OTHER_COMPONENT(Transform)
    GET_OTHER_COMPONENT(MeshRender)
    GET_OTHER_COMPONENT(Collider2D)
    GET_OTHER_COMPONENT(Animator2D)
    GET_OTHER_COMPONENT(TileMap)
    GET_OTHER_COMPONENT(ParticleSystem)

        CComponent* Clone() = 0;



public:
    CComponent(COMPONENT_TYPE _eType); //컴포넌트타입이 무조건 세팅할 수 있게 기본생성자 X
    virtual ~CComponent();

    friend class CGameObject;
};


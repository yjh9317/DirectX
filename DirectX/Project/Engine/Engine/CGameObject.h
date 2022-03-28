#pragma once
#include "CEntity.h"

#define GET_COMPONENT(type, TYPE)  class C##type* type() { return (C##type*)m_arrCom[(UINT)COMPONENT_TYPE::TYPE]; }
// C##type -> Ctype  , 위에서 class를 붙이면 전방선언도 같이된다.


class CComponent;   //서로의 헤더에서 #include를 서로 참조하면 오류가 생긴다, 그래서 한쪽은 전방선언으로해야된다.


class CGameObject : //오브젝트는 컴포넌트 기반으로 실행
    public CEntity
{
private:
    CComponent* m_arrCom[(UINT)COMPONENT_TYPE::END];

public:
    void start();
    void update();
    void lateupdate();
    void finalupdate();
    void render();



public:
    void    AddComponent(CComponent* _component);
    CComponent* GetComponent(COMPONENT_TYPE _eType) { return m_arrCom[(UINT)_eType]; }

  
    // 컴포넌트를 매크로형식으로 작성
    GET_COMPONENT(Transform, TRANSFORM)
    GET_COMPONENT(MeshRender, MESHRENDER)


public:
    CGameObject();
    ~CGameObject();
};


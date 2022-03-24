#pragma once
#include "CEntity.h"

#define GET_COMPONENT(type, TYPE)  class C##type* type() { return (C##type*)m_arrCom[(UINT)COMPONENT_TYPE::TYPE]; }
// C##type -> Ctype  , ������ class�� ���̸� ���漱�� ���̵ȴ�.


class CComponent;


class CGameObject : //������ư�� ������Ʈ ������� ����
    public CEntity
{
private:
    CComponent* m_arrCom[(UINT)COMPONENT_TYPE::END];

public:
    void update();
    void lateupdate();
    void finalupdate();
    void render();



public:
    void    AddComponent(CComponent* _component);
    CComponent* GetComponent(COMPONENT_TYPE _eType) { return m_arrCom[(UINT)_eType]; }

  
    // ������Ʈ�� ��ũ���������� �ۼ�
    GET_COMPONENT(Transform, TRANSFORM)
    GET_COMPONENT(MeshRender, MESHRENDER)


public:
    CGameObject();
    ~CGameObject();
};

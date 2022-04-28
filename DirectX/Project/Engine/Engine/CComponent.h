#pragma once
#include "CEntity.h"

#include "CGameObject.h"

#define GET_OTHER_COMPONENT(Type) C##Type* Type() { return GetOwner()->Type(); }


class CComponent :
    public CEntity
{
private:
    const COMPONENT_TYPE     m_eComType;        //��ü�� ������Ʈ Ÿ��
    CGameObject* m_pOwner;

public:
    virtual void start() {}
    virtual void update() {}
    virtual void lateupdate() {}
    virtual void finalupdate() = 0; //���� ������Ʈ�̹Ƿ� ���������Լ�
    

public:
    COMPONENT_TYPE GetType() { return m_eComType; }
    CGameObject* GetOwner() { return m_pOwner; }



    // ������Ʈ���� ���� ��ȣ�ۿ��� �ϹǷ� CComponent(�θ�Ŭ����)���� �ۼ�
    GET_OTHER_COMPONENT(Transform)
    GET_OTHER_COMPONENT(MeshRender)
    GET_OTHER_COMPONENT(Collider2D)
    GET_OTHER_COMPONENT(Animator2D)
    GET_OTHER_COMPONENT(TileMap)
    GET_OTHER_COMPONENT(ParticleSystem)

        CComponent* Clone() = 0;



public:
    CComponent(COMPONENT_TYPE _eType); //������ƮŸ���� ������ ������ �� �ְ� �⺻������ X
    virtual ~CComponent();

    friend class CGameObject;
};


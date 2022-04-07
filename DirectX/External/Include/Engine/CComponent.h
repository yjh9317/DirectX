#pragma once
#include "CEntity.h"

#include "CGameObject.h"

#define GET_OTHER_COMPONENT(Type) C##Type* Type() { return GetOwner()->Type(); }

class CTransform;
class CMeshRender;

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
    virtual void render() {}

public:
    COMPONENT_TYPE GetType() { return m_eComType; }
    CGameObject* GetOwner() { return m_pOwner; }



    GET_OTHER_COMPONENT(Transform) // ������Ʈ���� ���� ��ȣ�ۿ��� �ϹǷ� CComponent(�θ�Ŭ����)���� �ۼ�
    GET_OTHER_COMPONENT(MeshRender)
    GET_OTHER_COMPONENT(Collider2D)

    CComponent* Clone() = 0;



public:
    CComponent(COMPONENT_TYPE _eType); //������ƮŸ���� ������ ������ �� �ְ� �⺻������ X
    virtual ~CComponent();

    friend class CGameObject;
};


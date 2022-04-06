#pragma once
#include "CEntity.h"

#define GET_COMPONENT(type, TYPE)  class C##type* type() { return (C##type*)m_arrCom[(UINT)COMPONENT_TYPE::TYPE]; }

class CComponent;
class CTransform;
class CMeshRender;


class CGameObject :
    public CEntity
{
private:
    vector<CGameObject*>    m_vecChild;
    CComponent*             m_arrCom[(UINT)COMPONENT_TYPE::END];

    CGameObject*            m_pParent;

    int                     m_iLayerIdx; // ���� ������Ʈ �Ҽ� ���̾� �ε���

    bool                    m_bActive;
    bool                    m_bDead;

public:
    void start();
    void update();
    void lateupdate();
    void finalupdate();
    void render();

public:
    CGameObject* GetParent() { return m_pParent; }
    const vector<CGameObject*>& GetChild() { return m_vecChild; }

    // Deregister ==> ��� ���(���->�̵��), Unregister ==> ��� �ȵ�(��� ->�̵��, ���ʿ� ���X)
    void Deregister();

    void DisconnectBetweenParent();

    bool IsDead() { return m_bDead; }
    bool IsActive() { return m_bActive; }


public:
    void AddChild(CGameObject* _pChild);
    void AddComponent(CComponent* _component);
    CComponent* GetComponent(COMPONENT_TYPE _eType){ return m_arrCom[(UINT)_eType];  }
        
    void Destroy();


    GET_COMPONENT(Transform, TRANSFORM)
    GET_COMPONENT(MeshRender, MESHRENDER)


public: 
    CLONE(CGameObject)

public:
    CGameObject();
    CGameObject(const CGameObject& _origin);
    ~CGameObject();

    friend class CEventMgr;
    friend class CScene;
    friend class CLayer;
};


#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer :
    public CEntity
{
private:
    vector<CGameObject*>    m_vecRoot;  // ���̾ ���� �ֻ��� �θ� ������Ʈ
    vector<CGameObject*>    m_vecObj;   // �� ������ ���� �ľǵǴ� ���̾� �Ҽ� ��� ������Ʈ

public:
    void start();
    void update();
    void lateupdate();
    void finalupdate();
    void render();

public:
    void AddObject(CGameObject* _pObj);
    void RegisterObject(CGameObject* _pObj){m_vecObj.push_back(_pObj);}
    void DeregisterObject(CGameObject* _pObj);
    void Clear() { m_vecObj.clear(); }


    vector<CGameObject*>& GetRootObjects() { return m_vecRoot; }
    vector<CGameObject*>& GetObjects() { return m_vecObj; }

    

    CLONE(CLayer)

public:
    CLayer();
    ~CLayer();
};


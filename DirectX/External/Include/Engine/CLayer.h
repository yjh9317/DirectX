#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer :          //Stage 의 Group
    public CEntity
{
private:
    vector<CGameObject*>        m_vecRoot;  // 오브젝트 관리

public:
    void start();
    void update();
    void lateupdate();
    void finalupdate();
    void render();

public:
    void AddObject(CGameObject* _pObj);     // 오브젝트에 추가

public:
    CLayer();
    ~CLayer();
};


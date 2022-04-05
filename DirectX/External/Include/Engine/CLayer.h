#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer :
    public CEntity
{
private:
    vector<CGameObject*>    m_vecRoot;  // 레이어에 속한 최상위 부모 오브젝트
    vector<CGameObject*>    m_vecObj;   // 매 프레임 마다 파악되는 레이어 소속 모든 오브젝트

public:
    void start();
    void update();
    void lateupdate();
    void finalupdate();
    void render();

public:
    void AddObject(CGameObject* _pObj);
    void RegisterObject(CGameObject* _pObj){m_vecObj.push_back(_pObj);}

    void Clear(){m_vecObj.clear();}

    CLONE(CLayer)

public:
    CLayer();
    ~CLayer();
};


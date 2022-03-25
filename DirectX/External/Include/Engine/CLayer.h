#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer :          //Stage �� Group
    public CEntity
{
private:
    vector<CGameObject*>        m_vecRoot;  // ������Ʈ ����

public:
    void start();
    void update();
    void lateupdate();
    void finalupdate();
    void render();

public:
    void AddObject(CGameObject* _pObj);     // ������Ʈ�� �߰�

public:
    CLayer();
    ~CLayer();
};


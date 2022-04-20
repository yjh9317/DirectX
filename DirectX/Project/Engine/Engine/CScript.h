#pragma once
#include "CComponent.h"

#include "CTransform.h"
#include "CMeshRender.h"

#include "CResMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CLayer.h"


class CScript :
    public CComponent
{
private:
    const int  m_iScriptID;    // ��ũ��Ʈ �����뵵(��ũ��Ʈ Ÿ��)

public:
    virtual void start() {}
    virtual void update() {}
    virtual void lateupdate() {}
    virtual void finalupdate() final {}

    virtual void OnCollisionEnter(CGameObject* _OtherObject) {};
    virtual void OnCollision(CGameObject* _OtherObject) {};
    virtual void OnCollisionExit(CGameObject* _OtherObject) {};



    virtual CScript* Clone() = 0;


    

public:
    CScript();
    ~CScript();
};


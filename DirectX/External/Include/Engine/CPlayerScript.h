#pragma once
#include "CScript.h"
class CPlayerScript :       //플레이어의 스크립트
    public CScript
{
private:
    float m_fSpeed;

public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    CLONE(CPlayerScript)

public:
    CPlayerScript();
    ~CPlayerScript();

};


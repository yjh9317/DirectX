#pragma once
#include "CScript.h"
class CPlayerScript :       //�÷��̾��� ��ũ��Ʈ
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


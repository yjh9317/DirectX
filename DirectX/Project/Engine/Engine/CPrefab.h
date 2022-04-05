#pragma once
#include "CRes.h"

class CGameObject;

class CPrefab :     // 다른 리소스를 참조하는 클래스
    public CRes
{
private:
    CGameObject* m_pProtoObj;


public:
    CGameObject* Instantiate(); // 원본 리소스를 객체화 하는 함수

    virtual int Load(const wstring& _strFilePath)
    {
        return S_OK;
    }

public:
    CPrefab();
    CPrefab(CGameObject* _pProtoObj);
    ~CPrefab();
};


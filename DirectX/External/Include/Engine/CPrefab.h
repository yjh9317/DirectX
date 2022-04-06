#pragma once
#include "CRes.h"

class CGameObject;

class CPrefab :     // �ٸ� ���ҽ��� �����ϴ� Ŭ����
    public CRes
{
private:
    CGameObject* m_pProtoObj;


public:
    CGameObject* Instantiate(); // ���� ���ҽ��� ��üȭ �ϴ� �Լ�

    virtual int Load(const wstring& _strFilePath)
    {
        return S_OK;
    }

public:
    CPrefab();
    CPrefab(CGameObject* _pProtoObj);
    ~CPrefab();
};


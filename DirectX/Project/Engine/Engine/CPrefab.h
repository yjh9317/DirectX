#pragma once
#include "CRes.h"

class CGameObject;

class CPrefab :     // 다른 리소스를 참조하는 클래스
    public CRes
{
private:
    CGameObject* m_pProtoObj;

public:
    typedef void (*SaveFunc) (CPrefab*, const wstring&);
    typedef int (*LoadFunc) (CPrefab*, const wstring&);
    static SaveFunc        m_pSaveFunc;
    static LoadFunc        m_pLoadFunc;

public:
    CGameObject* Instantiate();     // 원본 리소스를 객체화 하는 함수
    CGameObject* GetProto() { return m_pProtoObj; }
    void SetProto(CGameObject* _pProto) { m_pProtoObj = _pProto; }

public:
    virtual int Save(const wstring& _strFilePath);
    virtual int Load(const wstring& _strFilePath);

public:
    CPrefab();
    CPrefab(CGameObject* _pProtoObj);
    ~CPrefab();
};


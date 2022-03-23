#pragma once
#include "CEntity.h"
class CRes :
    public CEntity
{
private:
    wstring m_strKey;
    wstring m_strRelativePath;

public:
    void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
    void SetRelativePath(const wstring& _strRelativePath) { m_strRelativePath = _strRelativePath; }


public:
    const wstring& GetKey() { return m_strKey; }
    const wstring& GetRelativePath() { return m_strRelativePath; }

public:
    virtual int Load(const wstring& _strFilePath) = 0;
    

public:
    CRes();
    ~CRes();

    friend class CResMgr;
};


#pragma once
#include "UI.h"

class TreeUI;

class ResourceUI :
    public UI
{
private:
    TreeUI* m_TreeUI;       // 리소스를 관리하기 위한 트리UI의 주소

    vector<wstring> m_vecResPath;   // 리소스 경로 문자열 벡터
public:
    virtual void update() override;
    virtual void render_update() override;

public:
    void Reset();

private:
    void Reload();
    void Renew();
    void FindFileName(const wstring& _strFolderPath);

private:
    void ItemClicked(DWORD_PTR _dwNode);
    void ItemDBClicked(DWORD_PTR _dwNode);

    RES_TYPE GetResTypeFromExt(const wstring& _strExt);
public:
    ResourceUI();
    ~ResourceUI();
};


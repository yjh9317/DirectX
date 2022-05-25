#pragma once
#include "UI.h"
class ListUI :
    public UI
{
private:
    vector<string>  m_vecList;  

public:
    void AddList(const string& _str) { m_vecList.push_back(_str); }
    void AddList(vector<string>& _vecStr) { m_vecList.insert(m_vecList.cend(), _vecStr.begin(), _vecStr.end()); }
    void Clear() { m_vecList.clear(); }


public:
    virtual void update() override;
    virtual void render_update() override;
    virtual void Activate() override;

public:
    ListUI();
    ~ListUI();
};


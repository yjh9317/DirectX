#pragma once
#include "UI.h"

typedef void (UI::* DBCLKED)(DWORD_PTR _param);
//delegate , DWORD_PTR(8����Ʈ ����)�� �Լ� �����͸� �޾Ƽ� ����Ѵ�.����Ŭ���ϸ� �����ͷ� ���� �Լ��� �̺�Ʈ�� �߻��ϴ� ���
// �Ϲ� �����Լ��� ����� CallBack, ��ü�� �̿��Ͽ� ����� Delegate

class ListUI :
    public UI
{
private:
    vector<string>  m_vecList;

    // ����Ŭ�� �̺�Ʈ   
    UI*             m_Inst;     // �����ͷ� ���� �Լ��� ȣ���� ��ü
    DBCLKED         m_DBCEvent; // �Լ��� ������ ������

public:
    void AddList(const string& _str) { m_vecList.push_back(_str); }
    void AddList(vector<string>& _vecStr) { m_vecList.insert(m_vecList.cend(), _vecStr.begin(), _vecStr.end()); }
    void Clear() { m_vecList.clear(); }

    void SetDBCEvent(UI* _pUI, DBCLKED _func)   // ȣ���� ��ü�� �Լ��� ����
    {
        m_Inst = _pUI;
        m_DBCEvent = _func;
    }


public:
    virtual void update() override;
    virtual void render_update() override;
    virtual void Activate() override;

public:
    ListUI();
    ~ListUI();
};


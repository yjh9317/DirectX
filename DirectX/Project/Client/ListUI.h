#pragma once
#include "UI.h"

typedef void (UI::* DBCLKED)(DWORD_PTR _param);
//delegate , DWORD_PTR(8바이트 정수)로 함수 포인터를 받아서 사용한다.더블클릭하면 포인터로 들어온 함수가 이벤트로 발생하는 방식
// 일반 전역함수로 만들면 CallBack, 객체를 이용하여 만들면 Delegate

class ListUI :
    public UI
{
private:
    vector<string>  m_vecList;

    // 더블클릭 이벤트   
    UI*             m_Inst;     // 포인터로 받은 함수를 호출할 객체
    DBCLKED         m_DBCEvent; // 함수를 저장할 포인터

public:
    void AddList(const string& _str) { m_vecList.push_back(_str); }
    void AddList(vector<string>& _vecStr) { m_vecList.insert(m_vecList.cend(), _vecStr.begin(), _vecStr.end()); }
    void Clear() { m_vecList.clear(); }

    void SetDBCEvent(UI* _pUI, DBCLKED _func)   // 호출할 객체와 함수를 세팅
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


#include "pch.h"
#include "CRes.h"

CRes::CRes(RES_TYPE _eType)
    : m_eResType(_eType)
{
}

CRes::~CRes()
{
}

bool CRes::CheckFail(HRESULT _hr)
{
    if (FAILED(_hr))
    {
        int iErr = GetLastError();
        wchar_t szText[256] = {};
        wsprintf(szText, L"�����ڵ� : %d", iErr);
        MessageBox(nullptr, szText, L"�ؽ��� �ε� ����!!", MB_OK);

        return true;
    }

    return false;
}

